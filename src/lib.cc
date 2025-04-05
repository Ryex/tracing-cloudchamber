
#include "lib.h"
#include "rust/cxx.h"
#include "tracing-cloudchamber/src/lib.rs.h"
#include <atomic>
#include <cassert>
#include <iostream>

namespace cloudchamber {

Interest Callsite::register_site() const {
  uint8_t expected = callsite::UNREGISTERED;
  if (registration.compare_exchange_strong(expected, callsite::REGISTERING,
                                           std::memory_order_acq_rel,
                                           std::memory_order_acquire)) {
    registration.store(callsite::REGISTERED, std::memory_order_release);
  } else {
    auto val = registration.load(std::memory_order_acquire);
    if (val != callsite::REGISTERED) {
      assert(val == callsite::REGISTERING);
      return {::cloudchamber::detail::InterestKind::SOMERTIMES};
    }
  }
  switch (interest.load(std::memory_order_relaxed)) {
  case callsite::INTEREST_NEVER:
    return {::cloudchamber::detail::InterestKind::NEVER};
  case callsite::INTEREST_ALWAYS:
    return {::cloudchamber::detail::InterestKind::ALWAYS};
  default:
    return {::cloudchamber::detail::InterestKind::SOMERTIMES};
  }
}

void Callsite::store_interest(const cloudchamber::Interest &intrst) const {
  uint8_t val;
  switch (intrst.value) {
  case ::cloudchamber::detail::InterestKind::ALWAYS:
    val = cloudchamber::callsite::INTEREST_ALWAYS;
    break;
  case ::cloudchamber::detail::InterestKind::NEVER:
    val = cloudchamber::callsite::INTEREST_NEVER;
    break;
  case ::cloudchamber::detail::InterestKind::SOMERTIMES:
  default:
    val = cloudchamber::callsite::INTEREST_SOMETIMES;
  }
  std::cout << "storing interest " << val << std::endl;
  interest.store(val, std::memory_order_seq_cst);
}

const rust::Box<RustMetadata> &Callsite::get_meta() const {
  if (!rust_meta.has_value()) {
    rust_meta.emplace(new_rust_metadata(*meta));
  }
  return rust_meta.value();
}

bool Callsite::is_enabled() const {
  return default_enabled_for_meta(get_interest(), get_meta());
}

Interest Callsite::get_interest() const {
  switch (interest.load(std::memory_order_relaxed)) {
  case ::cloudchamber::callsite::INTEREST_NEVER:
    return {::cloudchamber::detail::InterestKind::NEVER};
  case ::cloudchamber::callsite::INTEREST_ALWAYS:
    return {::cloudchamber::detail::InterestKind::ALWAYS};
  default:
    return register_site();
  }
}

} // namespace cloudchamber
