
#include "lib.h"
#include "rust/cxx.h"
#include "tracing-cloudchamber/src/lib.rs.h"
#include <atomic>
#include <cassert>
#include <iomanip>
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

rust::Box<RustMetadata> const &Callsite::get_meta() const {
  if (!rust_meta.has_value()) {
    rust_meta.emplace(::cloudchamber::new_rust_metadata(*meta));
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
void Callsite::set_metadata_ptr(const ::cloudchamber::Metadata *ptr) const {
  meta = ptr;
}

std::uint8_t const &FieldValue::get_u8() const noexcept {
  return rcast<std::uint8_t>();
}
std::uint16_t const &FieldValue::get_u16() const noexcept {
  return rcast<std::uint16_t>();
}
std::uint32_t const &FieldValue::get_u32() const noexcept {
  return rcast<std::uint32_t>();
}
std::uint64_t const &FieldValue::get_u64() const noexcept {
  return rcast<std::uint64_t>();
}
std::int8_t const &FieldValue::get_i8() const noexcept {
  return rcast<std::int8_t>();
}
std::int16_t const &FieldValue::get_i16() const noexcept {
  return rcast<std::int16_t>();
}
std::int32_t const &FieldValue::get_i32() const noexcept {
  return rcast<std::int32_t>();
}
std::int64_t const &FieldValue::get_i64() const noexcept {
  return rcast<std::int64_t>();
}
float const &FieldValue::get_f32() const noexcept { return rcast<float>(); }
double const &FieldValue::get_f64() const noexcept { return rcast<double>(); }
bool const &FieldValue::get_bool() const noexcept { return rcast<bool>(); }
rust::String const &FieldValue::get_string() const noexcept {
  _string = ::rust::String::lossy(cast<std::string>());
  return _string;
}
rust::String const &FieldValue::get_str() const noexcept {
  _string = rust::String::lossy(cast<std::string_view>().cbegin());
  return _string;
}

rust::Box<DisplayValue> const &FieldValue::get_debug() const {
  if (debug_formater != nullptr) {
    _display.emplace(::cloudchamber::string_to_display_value(debug_formater()));
    return _display.value();
  } else {
    throw std::runtime_error("No debug formatter stored for this type.");
  }
}

ScopeLambda::ScopeLambda(std::function<void()> f) : f(f) {}

void ScopeLambda::call() const { f(); }

namespace util {

template <> std::string toString(std::string const &s) {
  std::ostringstream out;
  out << std::quoted(s);
  return out.str();
}

template <> std::string toString(std::string_view const &s) {
  std::ostringstream out;
  out << std::quoted(s);
  return out.str();
}

std::string toString(const char *s) {
  std::ostringstream out;
  out << std::quoted(s);
  return out.str();
}

} // namespace util

} // namespace cloudchamber
