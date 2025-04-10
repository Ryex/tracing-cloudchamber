#pragma once

#ifndef TRACING_CLOUDCHAMBER_LIB_H
#define TRACING_CLOUDCHAMBER_LIB_H

#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

#include "rust/cxx.h"

namespace cloudchamber {

struct Metadata;
struct Interest;
struct RustMetadata;
struct DisplayValue;

namespace callsite {

const uint8_t UNREGISTERED(0);
const uint8_t REGISTERING(1);
const uint8_t REGISTERED(2);

const uint8_t INTEREST_NEVER(0);
const uint8_t INTEREST_SOMETIMES(1);
const uint8_t INTEREST_ALWAYS(2);
const uint8_t INTEREST_EMPTY(0xFF);

} // namespace callsite

/**
 * @class Callsite
 * @brief a event call site, holds interst and registation information alonside
 * the callsite metadata
 *
 */
struct Callsite {
  mutable std::atomic_uint8_t interest;
  mutable std::atomic_uint8_t registration;
  mutable ::cloudchamber::Metadata const *meta;
  mutable std::optional<rust::Box<RustMetadata>> rust_meta;

  Callsite()
      : interest(std::atomic_uint8_t(callsite::INTEREST_NEVER)),
        registration(std::atomic_uint8_t(callsite::UNREGISTERED)),
        meta(nullptr) {};

  ::cloudchamber::Interest register_site() const;
  void store_interest(const cloudchamber::Interest &intrst) const;
  ::cloudchamber::Interest get_interest() const;
  /**
   * @brief Get the metadata used by tracing
   */
  rust::Box<RustMetadata> const &get_meta() const;
  bool is_enabled() const;
  void set_metadata_ptr(::cloudchamber::Metadata const *ptr) const;
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
  template <typename T> T const &rcast() const {
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
  EMPTY,
};

} // namespace detail

/**
 * @class FieldEmpty
 * @brief placeholder value for a event or span field, will not be printed.
 *
 */
struct FieldEmpty {};

namespace util {
template <typename...> using try_to_instantiate = void;

using disregard_this = void;

template <template <typename...> class Expression, typename Attempt,
          typename... Ts>
struct is_detected_impl : std::false_type {};

template <template <typename...> class Expression, typename... Ts>
struct is_detected_impl<Expression, try_to_instantiate<Expression<Ts...>>,
                        Ts...> : std::true_type {};

template <template <typename...> class Expression, typename... Ts>
constexpr bool is_detected =
    is_detected_impl<Expression, disregard_this, Ts...>::value;

template <typename T>
using std_to_string_expression = decltype(std::to_string(std::declval<T>()));

template <typename T>
constexpr bool has_std_to_string = is_detected<std_to_string_expression, T>;

template <typename T>
using to_string_expression = decltype(to_string(std::declval<T>()));

template <typename T>
constexpr bool has_to_string = is_detected<to_string_expression, T>;

template <typename T>
using ostringstream_expression =
    decltype(std::declval<std::ostringstream &>() << std::declval<T>());

template <typename T>
constexpr bool has_ostringstream = is_detected<ostringstream_expression, T>;

template <typename T,
          typename std::enable_if<has_std_to_string<T>, int>::type = 0>
std::string toString(T const &t) {
  return std::to_string(t);
}

template <typename T,
          typename std::enable_if<!has_std_to_string<T> && has_to_string<T>,
                                  int>::type = 0>
std::string toString(T const &t) {
  return to_string(t);
}

template <typename T,
          typename std::enable_if<!has_std_to_string<T> && !has_to_string<T> &&
                                      has_ostringstream<T>,
                                  int>::type = 0>
std::string toString(T const &t) {
  std::ostringstream out;
  out << t;
  return out.str();
}

template <> std::string toString(std::string const &s);
template <> std::string toString(std::string_view const &s);
std::string toString(const char *s);
} // namespace util

template <typename T> std::string field_format(T const &value) {
  return ::cloudchamber::util::toString(value);
}

template <typename T> std::string field_format(std::vector<T> const &value) {
  std::ostringstream out;
  out << "std::vector[";
  const std::size_t size = value.size();
  for (std::size_t i = 0; i < size; ++i) {
    if (i > 0) {
      out << ", ";
    }
    out << ::cloudchamber::util::toString(value[i]);
  }
  out << "]";
  return out.str();
}

template <std::size_t N, typename T>
std::string field_format(std::array<T, N> const &value) {
  std::ostringstream out;
  out << "std::array<" << N << ">[";
  for (std::size_t i = 0; i < N; ++i) {
    if (i > 0) {
      out << ", ";
    }
    out << ::cloudchamber::util::toString(value[i]);
  }
  out << "]";
  return out.str();
}

template <typename K, typename V>
std::string field_format(std::map<K, V> const &value) {
  std::ostringstream out;
  out << "std::map{";
  std::size_t index = 0;
  for (const auto &n : value) {
    if (index > 0) {
      out << ", ";
    }
    out << ::cloudchamber::util::toString(n.first) << ": "
        << ::cloudchamber::util::toString(n.second);
    ++index;
  }
  out << "}";
  return out.str();
}

/**
 * @class FieldValue
 * @brief a rust like "trait" for values that can be attached to fields, records
 * printing information while holding a refrence
 *
 */
struct FieldValue : public ::cloudchamber::detail::FieldTrait {
  template <typename T>
  FieldValue(T &t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::DEBUG),
        debug_formater(
            [this]() { return ::cloudchamber::field_format(rcast<T>()); }) {}
  FieldValue(FieldEmpty &t)
      : FieldTrait(std::move(t)),
        _tag(::cloudchamber::detail::FieldValueKind::EMPTY) {}
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

  std::uint8_t const &get_u8() const noexcept;
  std::uint16_t const &get_u16() const noexcept;
  std::uint32_t const &get_u32() const noexcept;
  std::uint64_t const &get_u64() const noexcept;
  std::int8_t const &get_i8() const noexcept;
  std::int16_t const &get_i16() const noexcept;
  std::int32_t const &get_i32() const noexcept;
  std::int64_t const &get_i64() const noexcept;
  float const &get_f32() const noexcept;
  double const &get_f64() const noexcept;
  bool const &get_bool() const noexcept;
  rust::String const &get_string() const noexcept;
  rust::String const &get_str() const noexcept;
  rust::Box<::cloudchamber::DisplayValue> const &get_debug() const;

private:
  ::cloudchamber::detail::FieldValueKind _tag;
  std::function<std::string()> debug_formater;
  mutable rust::String _string;
  mutable std::optional<rust::Box<::cloudchamber::DisplayValue>> _display;
};

/**
 * @class ScopeLambda
 * @brief Holds a lambda / function to pass to rust. Used by `in_scope`.
 *
 */
struct ScopeLambda {
  ScopeLambda(std::function<void()> f);
  void call() const;

private:
  std::function<void()> f;
};

} // namespace cloudchamber

#endif
