#pragma once

#ifndef TRACING_CLOUDCHAMBER_H
#define TRACING_CLOUDCHAMBER_H

#include <functional>
#include <iostream>
#include <memory>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "tracing-cloudchamber/src/lib.rs.h"

namespace cloudchamber {

namespace level {
static const Level ERROR = Level{::cloudchamber::detail::LevelValue::ERROR};
static const Level WARN = Level{::cloudchamber::detail::LevelValue::WARN};
static const Level INFO = Level{::cloudchamber::detail::LevelValue::INFO};
static const Level DEBUG = Level{::cloudchamber::detail::LevelValue::DEBUG};
static const Level TRACE = Level{::cloudchamber::detail::LevelValue::TRACE};
} // namespace level
namespace kind {
static const Kind EVENT = Kind{::cloudchamber::detail::KindValue::EVENT};
static const Kind SPAN = Kind{::cloudchamber::detail::KindValue::SPAN};
static const Kind HINT = Kind{::cloudchamber::detail::KindValue::HINT};
} // namespace kind

namespace detail {

template <typename T>
constexpr std::string_view __QUALIFIED_FUNCTION(T &&name) {
  std::string_view name_s(name);
  auto end = name_s.rfind(")");
  while (end != std::string::npos) {
    auto end_c = name_s.find(">", end);
    if (end_c == std::string::npos)
      break;
    end = end_c + 1;
  }
  if (end == std::string::npos)
    return name;

  auto start = name_s.find("(");
  while (start != std::string::npos) {
    auto start_c = name_s.rfind("<", start);
    if (start_c == std::string::npos)
      break;
    start = start_c - 1;
  }
  if (start != std::string::npos) {
    start = name_s.rfind(" ", start);
  }
  if (start == std::string::npos)
    start = 0;
  else
    start += 1;
  auto qual_name = name_s.substr(start, end - start + 1);
  return qual_name;
}

} // namespace detail

} // namespace cloudchamber

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#if !defined(TOKENPASTE_TCH) && !defined(TOKENPASTE_TCH2)
#define TOKENPASTE_TCH(x, y) x##y
#define TOKENPASTE_TCH2(x, y) TOKENPASTE_TCH(x, y)
#endif

#ifndef __tch_callsite
#define __tch_callsite(level, name, ...)                                       \
  static ::cloudchamber::Callsite TOKENPASTE_TCH2(__CALLSITE_, __LINE__) = {}; \
  static std::vector<::rust::Str> TOKENPASTE_TCH2(__FIELDS_,                   \
                                                  __LINE__) = {__VA_ARGS__};   \
  static ::cloudchamber::Metadata TOKENPASTE_TCH2(__META_, __LINE__){          \
      ::rust::Str(name),                                                       \
      ::rust::Str(                                                             \
          ::cloudchamber::detail::__QUALIFIED_FUNCTION(__PRETTY_FUNCTION__)    \
              .cbegin()),                                                      \
      level,                                                                   \
      ::rust::Str(__FILE__),                                                   \
      int32_t(__LINE__),                                                       \
      ::rust::Slice<const ::rust::Str>(TOKENPASTE_TCH2(__FIELDS_, __LINE__)),  \
      TOKENPASTE_TCH2(__CALLSITE_, __LINE__),                                  \
      ::cloudchamber::Kind{::cloudchamber::detail::KindValue::EVENT}};         \
  TOKENPASTE_TCH2(__CALLSITE_, __LINE__).meta =                                \
      &TOKENPASTE_TCH2(__META_, __LINE__);                                     \
  bool TOKENPASTE_TCH2(__CALLSITE_ENABLED_, __LINE__) = level.is_enabled();    \
  if (TOKENPASTE_TCH2(__CALLSITE_ENABLED_, __LINE__)) {                        \
    TOKENPASTE_TCH2(__CALLSITE_ENABLED_, __LINE__) =                           \
        TOKENPASTE_TCH2(__CALLSITE_, __LINE__).is_enabled();                   \
  }
#endif

#ifndef tch_event
#define tch_event(level, name)                                                 \
  __tch_callsite(level, name);                                                 \
  if (TOKENPASTE_TCH2(__CALLSITE_ENABLED_, __LINE__)) {                        \
    /* push fields */                                                          \
    ::cloudchamber::dispatch_tracing_event(                                    \
        TOKENPASTE_TCH2(__CALLSITE_, __LINE__).get_meta());                    \
  }
#endif

#ifndef tch_event_msg
#define tch_event_msg(level, name, msg)                                        \
  __tch_callsite(level, name, "message");                                      \
  if (TOKENPASTE_TCH2(__CALLSITE_ENABLED_, __LINE__)) {                        \
    /* push fields */                                                          \
    ::cloudchamber::dispatch_tracing_event1(                                   \
        TOKENPASTE_TCH2(__CALLSITE_, __LINE__).get_meta(),                     \
        ::cloudchamber::FieldValue(msg));                                      \
  }
#endif

#endif
