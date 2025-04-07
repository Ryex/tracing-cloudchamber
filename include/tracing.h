#pragma once

#ifndef TRACING_CLOUDCHAMBER_H
#define TRACING_CLOUDCHAMBER_H

#include <string_view>
#include <utility>

#include "tracing-cloudchamber/src/lib.rs.h"

namespace cloudchamber {

namespace level {
const Level ERROR = Level{::cloudchamber::detail::LevelValue::ERROR};
const Level WARN = Level{::cloudchamber::detail::LevelValue::WARN};
const Level INFO = Level{::cloudchamber::detail::LevelValue::INFO};
const Level DEBUG = Level{::cloudchamber::detail::LevelValue::DEBUG};
const Level TRACE = Level{::cloudchamber::detail::LevelValue::TRACE};
} // namespace level
namespace kind {
const Kind EVENT = Kind{::cloudchamber::detail::KindValue::EVENT};
const Kind SPAN = Kind{::cloudchamber::detail::KindValue::SPAN};
const Kind HINT = Kind{::cloudchamber::detail::KindValue::HINT};
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

template <typename... Args>
auto dispatch_tracing_event0(Args &&...args)
    -> decltype(dispatch_tracing_event(std::forward<Args>(args)...)) {
  return dispatch_tracing_event(std::forward<Args>(args)...);
}

template <typename... Args>
auto new_span0(Args &&...args)
    -> decltype(new_span(std::forward<Args>(args)...)) {
  return new_span(std::forward<Args>(args)...);
}

} // namespace cloudchamber

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__) && defined(_MSVC_LANG)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define __TCC_TOKENPASTE2(x, y) x##y
#define __TCC_TOKENPASTE(x, y) __TCC_TOKENPASTE2(x, y)
#define __TCC_CALL(X, Y) X Y
#define __TCC_EXPAND(...) __VA_ARGS__

#define __TCC_GET_ARG_0(...)
#define __TCC_GET_ARG_1(N, ...) N
#define __TCC_GET_ARG_2(x1, N, ...) N
#define __TCC_GET_ARG_3(x1, x2, N, ...) N
#define __TCC_GET_ARG_4(x1, x2, x3, N, ...) N
#define __TCC_GET_ARG_5(x1, x2, x3, x4, N, ...) N
#define __TCC_GET_ARG_6(x1, x2, x3, x4, x5, N, ...) N
#define __TCC_GET_ARG_7(x1, x2, x3, x4, x5, x6, N, ...) N
#define __TCC_GET_ARG_8(x1, x2, x3, x4, x5, x6, x7, N, ...) N
#define __TCC_GET_ARG_9(x1, x2, x3, x4, x5, x6, x7, x8, N, ...) N
#define __TCC_GET_ARG_10(x1, x2, x3, x4, x5, x6, x7, x8, x9, N, ...) N
#define __TCC_GET_ARG_11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, N, ...) N
#define __TCC_GET_ARG_12(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, N, ...) N
#define __TCC_GET_ARG_13(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, N, \
                         ...)                                                  \
  N
#define __TCC_GET_ARG_14(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12,    \
                         x13, N, ...)                                          \
  N
#define __TCC_GET_ARG_15(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12,    \
                         x13, x14, N, ...)                                     \
  N
#define __TCC_GET_ARG_16(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12,    \
                         x13, x14, x15, N, ...)                                \
  N
#define __TCC_GET_ARG_17(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12,    \
                         x13, x14, x15, x17, N, ...)                           \
  N
#define __TCC_GET_ARG_N(N, ...)                                                \
  __TCC_CALL(__TCC_TOKENPASTE(__TCC_GET_ARG_, N), (__VA_ARGS__))
#define __TCC_COUNT_VARARGS(...)                                               \
  __TCC_EXPAND(__TCC_GET_ARG_17("ignored", __VA_ARGS__, 15, 14, 13, 12, 11,    \
                                10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define __TCC_GET_PAIR_0(...)
#define __TCC_GET_PAIR_1(A, ...) A, __TCC_EXPAND(__GET_ARG_N(1, __VA_ARGS__))
#define __TCC_GET_PAIR_2(x1, x2, A, ...)                                       \
  A, __TCC_EXPAND(__GET_ARG_N(1, __VA_ARGS__))
#define __TCC_GET_PAIR_3(x1, x2, x3, x4, A, ...)                               \
  A, __TCC_EXPAND(__GET_ARG_N(1, __VA_ARGS__))
#define __TCC_GET_PAIR_4(x1, x2, x3, x4, x5, x6, A, ...)                       \
  A, __TCC_EXPAND(__GET_ARG_N(1, __VA_ARGS__))
#define __TCC_GET_PAIR_5(x1, x2, x3, x4, x5, x6, x7, x8, A, ...)               \
  A, __TCC_EXPAND(__GET_ARG_N(1, __VA_ARGS__))
#define __TCC_GET_PAIR_N(N, ...)                                               \
  __TCC_CALL(__TCC_TOKENPASTE(__TCC_GET_PAIR_, N), (__VA_ARGS__))
#define __TCC_COUNT_VARARGS_PAIRS(...)                                         \
  __TCC_EXPAND(__TCC_GET_ARG_17("ignored", __VA_ARGS__, 7, 7, 6, 6, 5, 5, 4,   \
                                4, 3, 3, 2, 2, 1, 1, 0, 0))

#define __TCC_GET_PAIRS_FIRST_1(x1, x2) x1
#define __TCC_GET_PAIRS_FIRST_2(x1, x2, x3, x4) x1, x3
#define __TCC_GET_PAIRS_FIRST_3(x1, x2, x3, x4, x5, x6) x1, x3, x5
#define __TCC_GET_PAIRS_FIRST_4(x1, x2, x3, x4, x5, x6, x7, x8) x1, x3, x5, x7
#define __TCC_GET_PAIRS_FIRST_5(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10)       \
  x1, x3, x5, x7, x9
#define __TCC_GET_PAIRS_FIRST_6(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11,  \
                                x12)                                           \
  x1, x3, x5, x7, x9, x11
#define __TCC_GET_PAIRS_FIRST_7(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11,  \
                                x12, x13, x14)                                 \
  x1, x3, x5, x7, x9, x11, x13
#define __TCC_GET_PAIRS_FIRST_N(N, ...)                                        \
  __TCC_CALL(__TCC_TOKENPASTE(__TCC_GET_PAIRS_FIRST_, N), (__VA_ARGS__))
#define __TCC_GET_PAIRS_FIRST(...)                                             \
  __TCC_EXPAND(__TCC_GET_PAIRS_FIRST_N(__TCC_COUNT_VARARGS_PAIRS(__VA_ARGS__), \
                                       __VA_ARGS__))

#define __TCC_GET_PAIRS_SECOND_1(x1, x2) x2
#define __TCC_GET_PAIRS_SECOND_2(x1, x2, x3, x4) x2, x4
#define __TCC_GET_PAIRS_SECOND_3(x1, x2, x3, x4, x5, x6) x2, x4, x6
#define __TCC_GET_PAIRS_SECOND_4(x1, x2, x3, x4, x5, x6, x7, x8) x2, x4, x6, x8
#define __TCC_GET_PAIRS_SECOND_5(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10)      \
  x2, x4, x6, x8, x10
#define __TCC_GET_PAIRS_SECOND_6(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, \
                                 x12)                                          \
  x2, x4, x6, x8, x10, x12
#define __TCC_GET_PAIRS_SECOND_7(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, \
                                 x12, x13, x14)                                \
  x2, x4, x6, x8, x10, x12, x14
#define __TCC_GET_PAIRS_SECOND_N(N, ...)                                       \
  __TCC_CALL(__TCC_TOKENPASTE(__TCC_GET_PAIRS_SECOND_, N), (__VA_ARGS__))
#define __TCC_GET_PAIRS_SECOND(...)                                            \
  __TCC_EXPAND(__TCC_GET_PAIRS_SECOND_N(                                       \
      __TCC_COUNT_VARARGS_PAIRS(__VA_ARGS__), __VA_ARGS__))

#define __TCC_LIST_WRAP_0(macro)
#define __TCC_LIST_WRAP_1(macro, VAR) macro(VAR)
#define __TCC_LIST_WRAP_2(macro, VAR, ...)                                     \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_1(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_3(macro, VAR, ...)                                     \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_2(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_4(macro, VAR, ...)                                     \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_3(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_5(macro, VAR, ...)                                     \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_4(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_6(macro, VAR, ...)                                     \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_5(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_7(macro, VAR, ...)                                     \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_6(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_8(macro, VAR, ...)                                     \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_7(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_9(macro, VAR, ...)                                     \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_8(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_10(macro, VAR, ...)                                    \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_9(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_11(macro, VAR, ...)                                    \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_10(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_12(macro, VAR, ...)                                    \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_11(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_13(macro, VAR, ...)                                    \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_12(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_14(macro, VAR, ...)                                    \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_13(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_15(macro, VAR, ...)                                    \
  macro(VAR), __TCC_EXPAND(__TCC_LIST_WRAP_14(macro, __VA_ARGS__))
#define __TCC_LIST_WRAP_N(N, macro, ...)                                       \
  __TCC_CALL(__TCC_TOKENPASTE(__TCC_LIST_WRAP_, N), (macro, __VA_ARGS__))
#define __TCC_LIST_WRAP(macro, ...)                                            \
  __TCC_LIST_WRAP_N(__TCC_EXPAND(__TCC_COUNT_VARARGS(__VA_ARGS__)), macro,     \
                    __VA_ARGS__)

#define __TCC_STR(x) __TCC_XSTR(x)
#define __TCC_XSTR(x) #x

#define _tcc_event_name() "event " __TCC_STR(__FILE__) ":" __TCC_STR(__LINE__)

#define _tcc_callsite(callsite, level, name, kind)                             \
  static const ::cloudchamber::Callsite __TCC_TOKENPASTE(callsite,             \
                                                         __LINE__) = {};       \
  static const ::cloudchamber::Metadata __TCC_TOKENPASTE(                      \
      __TCC_TOKENPASTE(callsite, _META), __LINE__){                            \
      ::rust::Str(name),                                                       \
      ::rust::Str(                                                             \
          ::cloudchamber::detail::__QUALIFIED_FUNCTION(__PRETTY_FUNCTION__)    \
              .data()),                                                        \
      level,                                                                   \
      ::rust::Str(__FILE__),                                                   \
      int32_t(__LINE__),                                                       \
      ::rust::Slice<const ::rust::Str>(                                        \
          __TCC_TOKENPASTE(__TCC_TOKENPASTE(callsite, _FIELDS), __LINE__)),    \
      __TCC_TOKENPASTE(callsite, __LINE__),                                    \
      ::cloudchamber::Kind{::cloudchamber::detail::KindValue::kind}};          \
  __TCC_TOKENPASTE(callsite, __LINE__)                                         \
      .set_metadata_ptr(                                                       \
          &__TCC_TOKENPASTE(__TCC_TOKENPASTE(callsite, _META), __LINE__));     \
  bool __TCC_TOKENPASTE(__TCC_TOKENPASTE(callsite, _ENABLED), __LINE__) =      \
      level.is_enabled();                                                      \
  if (__TCC_TOKENPASTE(__TCC_TOKENPASTE(callsite, _ENABLED), __LINE__)) {      \
    __TCC_TOKENPASTE(__TCC_TOKENPASTE(callsite, _ENABLED), __LINE__) =         \
        __TCC_TOKENPASTE(callsite, __LINE__).is_enabled();                     \
  }

#define tcc_callsite_f(callsite, level, name, kind, ...)                       \
  static const std::array<::rust::Str, __TCC_COUNT_VARARGS(__VA_ARGS__)>       \
      __TCC_TOKENPASTE(__TCC_TOKENPASTE(callsite, _FIELDS), __LINE__) = {      \
          __TCC_EXPAND(__TCC_LIST_WRAP(__TCC_STR, __VA_ARGS__))};              \
  _tcc_callsite(callsite, level, name, kind)

#define tcc_callsite(callsite, level, name, kind)                              \
  static const std::array<::rust::Str, 0> __TCC_TOKENPASTE(                    \
      __TCC_TOKENPASTE(callsite, _FIELDS), __LINE__) = {};                     \
  _tcc_callsite(callsite, level, name, kind)

#define tcc_named_event(level, name)                                           \
  tcc_callsite(__CALLSITE, level, name, EVENT);                                \
  if (__TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                        \
    ::cloudchamber::dispatch_tracing_event(                                    \
        __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta());                    \
  }

#define tcc_named_event_f(level, name, ...)                                    \
  tcc_callsite_f(__CALLSITE, level, name, EVENT, __VA_ARGS__);                 \
  if (__TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                        \
    __TCC_TOKENPASTE(::cloudchamber::dispatch_tracing_event,                   \
                     __TCC_COUNT_VARARGS(__VA_ARGS__))(                        \
        __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),                     \
        __TCC_LIST_WRAP(::cloudchamber::FieldValue, __VA_ARGS__));             \
  }

#define tcc_named_event_p(level, name, ...)                                    \
  tcc_callsite_f(__CALLSITE, level, name, EVENT,                               \
                 __TCC_GET_PAIRS_FIRST(__VA_ARGS__));                          \
  if (__TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                        \
    __TCC_TOKENPASTE(::cloudchamber::dispatch_tracing_event,                   \
                     __TCC_COUNT_VARARGS_PAIRS(__VA_ARGS__))(                  \
        __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),                     \
        __TCC_LIST_WRAP(::cloudchamber::FieldValue,                            \
                        __TCC_GET_PAIRS_SECOND(__VA_ARGS__)));                 \
  }

#define tcc_named_event_msg(level, name, msg)                                  \
  tcc_callsite_f(__CALLSITE, level, name, EVENT, message);                     \
  if (__TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                        \
    ::cloudchamber::dispatch_tracing_event1(                                   \
        __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),                     \
        ::cloudchamber::FieldValue(msg));                                      \
  }

#define tcc_named_event_msg_f(level, name, msg, ...)                           \
  tcc_callsite_f(__CALLSITE, level, name, EVENT, message, __VA_ARGS__);        \
  if (__TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                        \
    __TCC_TOKENPASTE(::cloudchamber::dispatch_tracing_event,                   \
                     __TCC_COUNT_VARARGS(msg, __VA_ARGS__))(                   \
        __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),                     \
        ::cloudchamber::FieldValue(msg),                                       \
        __TCC_LIST_WRAP(::cloudchamber::FieldValue, __VA_ARGS__));             \
  }

#define tcc_named_event_msg_p(level, name, msg, ...)                           \
  tcc_callsite_f(__CALLSITE, level, name, EVENT, message,                      \
                 __TCC_GET_PAIRS_FIRST(__VA_ARGS__));                          \
  if (__TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                        \
    __TCC_TOKENPASTE(::cloudchamber::dispatch_tracing_event,                   \
                     __TCC_COUNT_VARARGS_PAIRS(message, msg, __VA_ARGS__))(    \
        __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),                     \
        ::cloudchamber::FieldValue(msg),                                       \
        __TCC_LIST_WRAP(::cloudchamber::FieldValue,                            \
                        __TCC_GET_PAIRS_SECOND(__VA_ARGS__)));                 \
  }

#define tcc_event(level) tcc_named_event(level, _tcc_event_name())
#define tcc_event_f(level, ...)                                                \
  tcc_named_event_f(level, _tcc_event_name(), __VA_ARGS__)
#define tcc_event_p(level, ...)                                                \
  tcc_named_event_p(lavel, _tcc_event_name(), __VA_ARGS__)
#define tcc_event_msg(level, msg)                                              \
  tcc_named_event_msg(level, _tcc_event_name(), msg)
#define tcc_event_msg_f(level, msg, ...)                                       \
  tcc_named_event_msg_f(level, _tcc_event_name(), msg, __VA_ARGS__)
#define tcc_event_msg_p(level, msg, ...)                                       \
  tcc_named_event_msg_p(level, _tcc_event_name(), msg, __VA_ARGS__)

#define tcc_error()                                                            \
  tcc_named_event(::cloudchamber::level::ERROR, _tcc_event_name())
#define tcc_error_f(...)                                                       \
  tcc_named_event_f(::cloudchamber::level::ERROR, _tcc_event_name(),           \
                    __VA_ARGS__)
#define tcc_error_p(...)                                                       \
  tcc_named_event_p(::cloudchamber::level::ERROR, _tcc_event_name(),           \
                    __VA_ARGS__)
#define tcc_error_msg(msg)                                                     \
  tcc_named_event_msg(::cloudchamber::level::ERROR, _tcc_event_name(), msg)
#define tcc_error_msg_f(msg, ...)                                              \
  tcc_named_event_msg_f(::cloudchamber::level::ERROR, _tcc_event_name(), msg,  \
                        __VA_ARGS__)
#define tcc_error_msg_p(msg, ...)                                              \
  tcc_named_event_msg_p(::cloudchamber::level::ERROR, _tcc_event_name(), msg,  \
                        __VA_ARGS__)

#define tcc_warn()                                                             \
  tcc_named_event(::cloudchamber::level::WARN, _tcc_event_name())
#define tcc_warn_f(...)                                                        \
  tcc_named_event_f(::cloudchamber::level::WARN, _tcc_event_name(), __VA_ARGS__)
#define tcc_warn_p(...)                                                        \
  tcc_named_event_p(::cloudchamber::level::WARN, _tcc_event_name(), __VA_ARGS__)
#define tcc_warn_msg(msg)                                                      \
  tcc_named_event_msg(::cloudchamber::level::WARN, _tcc_event_name(), msg)
#define tcc_warn_msg_f(msg, ...)                                               \
  tcc_named_event_msg_f(::cloudchamber::level::WARN, _tcc_event_name(), msg,   \
                        __VA_ARGS__)
#define tcc_warn_msg_p(msg, ...)                                               \
  tcc_named_event_msg_p(::cloudchamber::level::WARN, _tcc_event_name(), msg,   \
                        __VA_ARGS__)

#define tcc_info()                                                             \
  tcc_named_event(::cloudchamber::level::INFO, _tcc_event_name())
#define tcc_info_f(...)                                                        \
  tcc_named_event_f(::cloudchamber::level::INFO, _tcc_event_name(), __VA_ARGS__)
#define tcc_info_p(...)                                                        \
  tcc_named_event_p(::cloudchamber::level::INFO, _tcc_event_name(), __VA_ARGS__)
#define tcc_info_msg(msg)                                                      \
  tcc_named_event_msg(::cloudchamber::level::INFO, _tcc_event_name(), msg)
#define tcc_info_msg_f(msg, ...)                                               \
  tcc_named_event_msg_f(::cloudchamber::level::INFO, _tcc_event_name(), msg,   \
                        __VA_ARGS__)
#define tcc_info_msg_p(msg, ...)                                               \
  tcc_named_event_msg_p(::cloudchamber::level::INFO, _tcc_event_name(), msg,   \
                        __VA_ARGS__)

#define tcc_debug()                                                            \
  tcc_named_event(::cloudchamber::level::DEBUG, _tcc_event_name())
#define tcc_debug_f(...)                                                       \
  tcc_named_event_f(::cloudchamber::level::DEBUG, _tcc_event_name(),           \
                    __VA_ARGS__)
#define tcc_debug_p(...)                                                       \
  tcc_named_event_p(::cloudchamber::level::DEBUG, _tcc_event_name(),           \
                    __VA_ARGS__)
#define tcc_debug_msg(msg)                                                     \
  tcc_named_event_msg(::cloudchamber::level::DEBUG, _tcc_event_name(), msg)
#define tcc_debug_msg_f(msg, ...)                                              \
  tcc_named_event_msg_f(::cloudchamber::level::DEBUG, _tcc_event_name(), msg,  \
                        __VA_ARGS__)
#define tcc_debug_msg_p(msg, ...)                                              \
  tcc_named_event_msg_p(::cloudchamber::level::DEBUG, _tcc_event_name(), msg,  \
                        __VA_ARGS__)

#define tcc_trace()                                                            \
  tcc_named_event(::cloudchamber::level::TRACE, _tcc_event_name())
#define tcc_trace_f(...)                                                       \
  tcc_named_event_f(::cloudchamber::level::TRACE, _tcc_event_name(),           \
                    __VA_ARGS__)
#define tcc_trace_p(...)                                                       \
  tcc_named_event_p(::cloudchamber::level::TRACE, _tcc_event_name(),           \
                    __VA_ARGS__)
#define tcc_trace_msg(msg)                                                     \
  tcc_named_event_msg(::cloudchamber::level::TRACE, _tcc_event_name(), msg)
#define tcc_trace_msg_f(msg, ...)                                              \
  tcc_named_event_msg_f(::cloudchamber::level::TRACE, _tcc_event_name(), msg,  \
                        __VA_ARGS__)
#define tcc_trace_msg_p(msg, ...)                                              \
  tcc_named_event_msg_p(::cloudchamber::level::TRACE, _tcc_event_name(), msg,  \
                        __VA_ARGS__)

#define tcc_span(ident, level, name)                                           \
  tcc_callsite(__CALLSITE, level, name, SPAN);                                 \
  ::rust::Box<::cloudchamber::Span> ident =                                    \
      __TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)                           \
          ? ::cloudchamber::new_span(                                          \
                __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta())             \
          : ::cloudchamber::new_disabled_span(                                 \
                __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta());

#define tcc_span_f(ident, level, name, ...)                                    \
  tcc_callsite_f(__CALLSITE, level, name, SPAN, __VA_ARGS__);                  \
  ::rust::Box<::cloudchamber::Span> ident =                                    \
      __TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)                           \
          ? __TCC_TOKENPASTE(::cloudchamber::new_span,                         \
                             __TCC_COUNT_VARARGS(__VA_ARGS__))(                \
                __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),             \
                __TCC_LIST_WRAP(::cloudchamber::FieldValue, __VA_ARGS__))      \
          : ::cloudchamber::new_disabled_span(                                 \
                __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta());

#define tcc_span_p(ident, level, name, ...)                                    \
  tcc_callsite_f(__CALLSITE, level, name, SPAN,                                \
                 __TCC_GET_PAIRS_FIRST(__VA_ARGS__));                          \
  ::rust::Box<::cloudchamber::Span> ident =                                    \
      __TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)                           \
          ? ident = __TCC_TOKENPASTE(::cloudchamber::new_span,                 \
                                     __TCC_COUNT_VARARGS_PAIRS(__VA_ARGS__))(  \
                __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),             \
                __TCC_LIST_WRAP(::cloudchamber::FieldValue,                    \
                                __TCC_GET_PAIRS_SECOND(__VA_ARGS__)))          \
          : ::cloudchamber::new_disabled_span(                                 \
                __TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta());

#define tcc_error_span(ident, name)                                            \
  tcc_span(ident, ::cloudchamber::level::ERROR, name)
#define tcc_error_span_f(ident, name, ...)                                     \
  tcc_span_f(ident, ::cloudchamber::level::ERROR, name, __VA_ARGS__)
#define tcc_error_span_p(ident, name, ...)                                     \
  tcc_span_p(ident, ::cloudchamber::level::ERROR, name, __VA_ARGS__)

#define tcc_warn_span(ident, name)                                             \
  tcc_span(ident, ::cloudchamber::level::WARN, name)
#define tcc_warn_span_f(ident, name, ...)                                      \
  tcc_span_f(ident, ::cloudchamber::level::WARN, name, __VA_ARGS__)
#define tcc_warn_span_p(ident, name, ...)                                      \
  tcc_span_p(ident, ::cloudchamber::level::WARN, name, __VA_ARGS__)

#define tcc_info_span(ident, name)                                             \
  tcc_span(ident, ::cloudchamber::level::INFO, name)
#define tcc_info_span_f(ident, name, ...)                                      \
  tcc_span_f(ident, ::cloudchamber::level::INFO, name, __VA_ARGS__)
#define tcc_info_span_p(ident, name, ...)                                      \
  tcc_span_p(ident, ::cloudchamber::level::INFO, name, __VA_ARGS__)

#define tcc_debug_span(ident, name)                                            \
  tcc_span(ident, ::cloudchamber::level::DEBUG, name)
#define tcc_debug_span_f(ident, name, ...)                                     \
  tcc_span_f(ident, ::cloudchamber::level::DEBUG, name, __VA_ARGS__)
#define tcc_debug_span_p(ident, name, ...)                                     \
  tcc_span_p(ident, ::cloudchamber::level::DEBUG, name, __VA_ARGS__)

#define tcc_trace_span(ident, name)                                            \
  tcc_span(ident, ::cloudchamber::level::TRACE, name)
#define tcc_trace_span_f(ident, name, ...)                                     \
  tcc_span_f(ident, ::cloudchamber::level::TRACE, name, __VA_ARGS__)
#define tcc_trace_span_p(ident, name, ...)                                     \
  tcc_span_p(ident, ::cloudchamber::level::TRACE, name, __VA_ARGS__)

#endif
