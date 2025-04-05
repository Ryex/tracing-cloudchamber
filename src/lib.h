#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <optional>
#include <string_view>
#include <sys/types.h>

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
  const ::cloudchamber::Metadata *meta;
  mutable std::optional<rust::Box<RustMetadata>> rust_meta;

  Callsite()
      : interest(std::atomic_uint8_t(callsite::INTEREST_NEVER)),
        registration(std::atomic_uint8_t(callsite::UNREGISTERED)),
        meta(nullptr) {};

  ::cloudchamber::Interest register_site() const;
  void store_interest(const cloudchamber::Interest &intrst) const;
  ::cloudchamber::Interest get_interest() const;
  const rust::Box<RustMetadata> &get_meta() const;
  bool is_enabled() const;
};

namespace detail {

struct FieldTrait {
public:
  FieldTrait(const FieldTrait &trait) = default;
  FieldTrait(FieldTrait &&trait) = default;
  FieldTrait &operator=(const FieldTrait &trait) = default;
  FieldTrait &operator=(FieldTrait &&trait) = default;
  virtual ~FieldTrait() = default;

  template <typename T>
  explicit FieldTrait(T t)
      : container(std::make_shared<Model<T>>(std::move(t))) {}

  template <typename T> T cast() const {
    auto typed_container = std::static_pointer_cast<const Model<T>>(container);
    return typed_container->m_data;
  }
  template <typename T> const T &rcast() const {
    auto typed_container = std::static_pointer_cast<const Model<T>>(container);
    return typed_container->m_data;
  }

private:
  struct Concept {
    // All need to be explicitly defined just to make the destructor virtual
    Concept() = default;
    Concept(const Concept &cncept) = default;
    Concept(Concept &&cncept) = default;
    Concept &operator=(const Concept &cncept) = default;
    Concept &operator=(Concept &&cncept) = default;
    virtual ~Concept() = default;
  };

  template <typename T> struct Model : public Concept {
    Model(T x) : m_data(std::move(x)) {}
    T m_data;
  };

  std::shared_ptr<const Concept> container;
};

enum class FieldValueKind : std::uint8_t {
  U8,
  U16,
  U32,
  U64,
  I8,
  I16,
  I32,
  I64,
  F32,
  F64,
  STRING,
  STR,
  BOOL,
  DEBUG,
};

template <typename... Args>
std::string string_format(const std::string &format, Args... args) {
  int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) +
               1; // Extra space for '\0'
  if (size_s <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  auto size = static_cast<size_t>(size_s);
  std::unique_ptr<char[]> buf(new char[size]);
  std::snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(buf.get(),
                     buf.get() + size - 1); // We don't want the '\0' inside
}

} // namespace detail

template <typename T, std::enable_if_t<std::is_signed<T>::value &&
                                           !std::is_floating_point<T>::value,
                                       bool> = true>
std::string field_print(T value) {
  return ::cloudchamber::detail::string_format("%i", value);
}

template <typename T, std::enable_if_t<std::is_unsigned<T>::value, bool> = true>
std::string field_print(T value) {
  return ::cloudchamber::detail::string_format("%u", value);
}

template <typename T,
          std::enable_if_t<std::is_floating_point<T>::value, bool> = true>
std::string field_print(T value) {
  return ::cloudchamber::detail::string_format("%f", value);
}

struct FieldValue : public ::cloudchamber::detail::FieldTrait {
  template <typename T>
  FieldValue(T &t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::DEBUG),
        debug_formater(
            [this]() { return ::cloudchamber::field_print(cast<T &>()); }) {}
  FieldValue(std::string t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::STRING) {}
  FieldValue(std::string_view t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::STR) {}
  FieldValue(const char *t)
      : FieldTrait(std::string_view(t)),
        _tag(::cloudchamber::detail::FieldValueKind::STR) {}
  FieldValue(std::uint8_t t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::U8) {}
  FieldValue(std::uint16_t t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::U16) {}
  FieldValue(std::uint32_t t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::U32) {}
  FieldValue(std::uint64_t t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::U64) {}
  FieldValue(std::int8_t t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::I8) {}
  FieldValue(std::int16_t t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::I16) {}
  FieldValue(std::int32_t t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::I32) {}
  FieldValue(std::int64_t t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::I64) {}
  FieldValue(float t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::F32) {}
  FieldValue(double t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::F64) {}
  FieldValue(bool t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::BOOL) {}

  ::cloudchamber::detail::FieldValueKind get_type() const { return _tag; }

  const std::uint8_t &get_u8() const noexcept { return rcast<std::uint8_t>(); }
  const std::uint16_t &get_u16() const noexcept {
    return rcast<std::uint16_t>();
  }
  const std::uint32_t &get_u32() const noexcept {
    return rcast<std::uint32_t>();
  }
  const std::uint64_t &get_u64() const noexcept {
    return rcast<std::uint64_t>();
  }
  const std::int8_t &get_i8() const noexcept { return rcast<std::int8_t>(); }
  const std::int16_t &get_i16() const noexcept { return rcast<std::int16_t>(); }
  const std::int32_t &get_i32() const noexcept { return rcast<std::int32_t>(); }
  const std::int64_t &get_i64() const noexcept { return rcast<std::int64_t>(); }
  const float &get_f32() const noexcept { return rcast<float>(); }
  const double &get_f64() const noexcept { return rcast<double>(); }
  const bool &get_bool() const noexcept { return rcast<bool>(); }
  const rust::String &get_string() const noexcept {
    _string = ::rust::String::lossy(cast<std::string>());
    return _string;
  }
  const rust::String &get_str() const noexcept {
    _string = rust::String::lossy(cast<std::string_view>().cbegin());
    return _string;
  }

  const rust::String &get_debug() const {
    if (debug_formater != nullptr) {
      _string = debug_formater();
      return _string;
    } else {
      throw std::runtime_error("No debug formatter stored for this type.");
    }
  }

private:
  ::cloudchamber::detail::FieldValueKind _tag;
  std::function<std::string()> debug_formater;
  mutable rust::String _string;
};

} // namespace cloudchamber
