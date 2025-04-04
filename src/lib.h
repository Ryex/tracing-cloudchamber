#pragma once

#include <atomic>
#include <optional>

#include "rust/cxx.h"

namespace cloudchamber {

struct Metadata;
struct Interest;
struct RustMetadata;

namespace callsite {

const uint8_t UNREGISTERED(0);
const uint8_t REGISTERING(1);
const uint8_t REGISTERED(2);

const uint8_t INTEREST_NEVER(0);
const uint8_t INTEREST_SOMETIMES(1);
const uint8_t INTEREST_ALWAYS(2);
const uint8_t INTEREST_EMPTY(0xFF);

} // namespace callsite

struct Callsite {
  mutable std::atomic_uint8_t interest;
  mutable std::atomic_uint8_t registration;
  const ::cloudchamber::Metadata &meta;
  mutable std::optional<rust::Box<RustMetadata>> rust_meta;

  Callsite(const ::cloudchamber::Metadata &meta)
      : interest(std::atomic_uint8_t(callsite::INTEREST_NEVER)),
        registration(std::atomic_uint8_t(callsite::UNREGISTERED)),
        meta(meta) {};

  ::cloudchamber::Interest register_site() const;
  void store_interest(const cloudchamber::Interest &intrst) const;
  ::cloudchamber::Interest get_interest() const;
  const rust::Box<RustMetadata> &get_meta() const;
  bool is_enabled() const;
};

} // namespace cloudchamber
