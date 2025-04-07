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

#define _TCC_TOKENPASTE2(x, y) x##y
#define _TCC_TOKENPASTE(x, y) _TCC_TOKENPASTE2(x, y)

#define _TCC_GET_ARG_0(...)
#define _TCC_GET_ARG_1(N, ...) N
#define _TCC_GET_ARG_2(_1, N, ...) N
#define _TCC_GET_ARG_3(_1, _2, N, ...) N
#define _TCC_GET_ARG_4(_1, _2, _3, N, ...) N
#define _TCC_GET_ARG_5(_1, _2, _3, _4, N, ...) N
#define _TCC_GET_ARG_6(_1, _2, _3, _4, _5, N, ...) N
#define _TCC_GET_ARG_7(_1, _2, _3, _4, _5, _6, N, ...) N
#define _TCC_GET_ARG_8(_1, _2, _3, _4, _5, _6, _7, N, ...) N
#define _TCC_GET_ARG_9(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#define _TCC_GET_ARG_10(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define _TCC_GET_ARG_11(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define _TCC_GET_ARG_12(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, N, ...) N
#define _TCC_GET_ARG_13(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, N,  \
                        ...)                                                   \
  N
#define _TCC_GET_ARG_14(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,     \
                        _13, N, ...)                                           \
  N
#define _TCC_GET_ARG_15(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,     \
                        _13, _14, N, ...)                                      \
  N
#define _TCC_GET_ARG_16(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,     \
                        _13, _14, _15, N, ...)                                 \
  N
#define _TCC_GET_ARG_17(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12,     \
                        _13, _14, _15, _17, N, ...)                            \
  N
#define _TCC_GET_ARG_N(N, ...) _TCC_TOKENPASTE(_TCC_GET_ARG_, N)(__VA_ARGS__)
#define _TCC_COUNT_VARARGS(...)                                                \
  _TCC_GET_ARG_17("ignored", __VA_ARGS__, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,  \
                  5, 4, 3, 2, 1, 0)

#define _TCC_GET_PAIR_0(...)
#define _TCC_GET_PAIR_1(A, ...) A, __GET_ARG_N(1, ##__VA_ARGS__)
#define _TCC_GET_PAIR_2(_1, _2, A, ...) A, __GET_ARG_N(1, ##__VA_ARGS__)
#define _TCC_GET_PAIR_3(_1, _2, _3, _4, A, ...) A, __GET_ARG_N(1, ##__VA_ARGS__)
#define _TCC_GET_PAIR_4(_1, _2, _3, _4, _5, _6, A, ...)                        \
  A, __GET_ARG_N(1, ##__VA_ARGS__)
#define _TCC_GET_PAIR_5(_1, _2, _3, _4, _5, _6, _7, _8, A, ...)                \
  A, __GET_ARG_N(1, ##__VA_ARGS__)
#define _TCC_GET_PAIR_N(N, ...) _TCC_TOKENPASTE(_TCC_GET_PAIR_, N)(__VA_ARGS__)
#define _TCC_COUNT_VARARGS_PAIRS(...)                                          \
  _TCC_GET_ARG_17("ignored", __VA_ARGS__, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2,  \
                  1, 1, 0, 0)

#define _TCC_GET_PAIRS_FIRST_1(_1, _2) _1
#define _TCC_GET_PAIRS_FIRST_2(_1, _2, _3, _4) _1, _3
#define _TCC_GET_PAIRS_FIRST_3(_1, _2, _3, _4, _5, _6) _1, _3, _5
#define _TCC_GET_PAIRS_FIRST_4(_1, _2, _3, _4, _5, _6, _7, _8) _1, _3, _5, _7
#define _TCC_GET_PAIRS_FIRST_5(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10)        \
  _1, _3, _5, _7, _9
#define _TCC_GET_PAIRS_FIRST_6(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11,   \
                               _12)                                            \
  _1, _3, _5, _7, _9, _11
#define _TCC_GET_PAIRS_FIRST_7(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11,   \
                               _12, _13, _14)                                  \
  _1, _3, _5, _7, _9, _11, _13
#define _TCC_GET_PAIRS_FIRST_N(N, ...)                                         \
  _TCC_TOKENPASTE(_TCC_GET_PAIRS_FIRST_, N)(__VA_ARGS__)
#define _TCC_GET_PAIRS_FIRST(...)                                              \
  _TCC_GET_PAIRS_FIRST_N(_TCC_COUNT_VARARGS_PAIRS(__VA_ARGS__), __VA_ARGS__)

#define _TCC_GET_PAIRS_SECOND_1(_1, _2) _2
#define _TCC_GET_PAIRS_SECOND_2(_1, _2, _3, _4) _2, _4
#define _TCC_GET_PAIRS_SECOND_3(_1, _2, _3, _4, _5, _6) _2, _4, _6
#define _TCC_GET_PAIRS_SECOND_4(_1, _2, _3, _4, _5, _6, _7, _8) _2, _4, _6, _8
#define _TCC_GET_PAIRS_SECOND_5(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10)       \
  _2, _4, _6, _8, _10
#define _TCC_GET_PAIRS_SECOND_6(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11,  \
                                _12)                                           \
  _2, _4, _6, _8, _10, _12
#define _TCC_GET_PAIRS_SECOND_7(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11,  \
                                _12, _13, _14)                                 \
  _2, _4, _6, _8, _10, _12, _14
#define _TCC_GET_PAIRS_SECOND_N(N, ...)                                        \
  _TCC_TOKENPASTE(_TCC_GET_PAIRS_SECOND_, N)(__VA_ARGS__)
#define _TCC_GET_PAIRS_SECOND(...)                                             \
  _TCC_GET_PAIRS_SECOND_N(_TCC_COUNT_VARARGS_PAIRS(__VA_ARGS__), __VA_ARGS__)

#define _TCC_LIST_WRAP_0(CALL)
#define _TCC_LIST_WRAP_1(CALL, VAR) CALL(VAR)
#define _TCC_LIST_WRAP_2(CALL, VAR, ...)                                       \
  CALL(VAR), _TCC_LIST_WRAP_1(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_3(CALL, VAR, ...)                                       \
  CALL(VAR), _TCC_LIST_WRAP_2(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_4(CALL, VAR, ...)                                       \
  CALL(VAR), _TCC_LIST_WRAP_3(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_5(CALL, VAR, ...)                                       \
  CALL(VAR), _TCC_LIST_WRAP_4(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_6(CALL, VAR, ...)                                       \
  CALL(VAR), _TCC_LIST_WRAP_5(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_7(CALL, VAR, ...)                                       \
  CALL(VAR), _TCC_LIST_WRAP_6(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_8(CALL, VAR, ...)                                       \
  CALL(VAR), _TCC_LIST_WRAP_7(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_9(CALL, VAR, ...)                                       \
  CALL(VAR), _TCC_LIST_WRAP_8(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_10(CALL, VAR, ...)                                      \
  CALL(VAR), _TCC_LIST_WRAP_9(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_11(CALL, VAR, ...)                                      \
  CALL(VAR), _TCC_LIST_WRAP_10(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_12(CALL, VAR, ...)                                      \
  CALL(VAR), _TCC_LIST_WRAP_11(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_13(CALL, VAR, ...)                                      \
  CALL(VAR), _TCC_LIST_WRAP_12(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_14(CALL, VAR, ...)                                      \
  CALL(VAR), _TCC_LIST_WRAP_13(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_15(CALL, VAR, ...)                                      \
  CALL(VAR), _TCC_LIST_WRAP_14(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP_N(N, CALL, ...)                                         \
  _TCC_TOKENPASTE(_TCC_LIST_WRAP_, N)(CALL, __VA_ARGS__)
#define _TCC_LIST_WRAP(CALL, ...)                                              \
  _TCC_LIST_WRAP_N(_TCC_COUNT_VARARGS(__VA_ARGS__), CALL, __VA_ARGS__)

#define _TCC_STR(x) _TCC_XSTR(x)
#define _TCC_XSTR(x) #x

#define _tcc_event_name() "event " _TCC_STR(__FILE__) ":" _TCC_STR(__LINE__)

#define _tcc_callsite(callsite, level, name, kind)                             \
  static const ::cloudchamber::Callsite _TCC_TOKENPASTE(callsite,              \
                                                        __LINE__) = {};        \
  static const ::cloudchamber::Metadata _TCC_TOKENPASTE(                       \
      _TCC_TOKENPASTE(callsite, _META), __LINE__){                             \
      ::rust::Str(name),                                                       \
      ::rust::Str(                                                             \
          ::cloudchamber::detail::__QUALIFIED_FUNCTION(__PRETTY_FUNCTION__)    \
              .cbegin()),                                                      \
      level,                                                                   \
      ::rust::Str(__FILE__),                                                   \
      int32_t(__LINE__),                                                       \
      ::rust::Slice<const ::rust::Str>(                                        \
          _TCC_TOKENPASTE(_TCC_TOKENPASTE(callsite, _FIELDS), __LINE__)),      \
      _TCC_TOKENPASTE(callsite, __LINE__),                                     \
      ::cloudchamber::Kind{::cloudchamber::detail::KindValue::kind}};          \
  _TCC_TOKENPASTE(callsite, __LINE__)                                          \
      .set_metadata_ptr(                                                       \
          &_TCC_TOKENPASTE(_TCC_TOKENPASTE(callsite, _META), __LINE__));       \
  bool _TCC_TOKENPASTE(_TCC_TOKENPASTE(callsite, _ENABLED), __LINE__) =        \
      level.is_enabled();                                                      \
  if (_TCC_TOKENPASTE(_TCC_TOKENPASTE(callsite, _ENABLED), __LINE__)) {        \
    _TCC_TOKENPASTE(_TCC_TOKENPASTE(callsite, _ENABLED), __LINE__) =           \
        _TCC_TOKENPASTE(callsite, __LINE__).is_enabled();                      \
  }

#define tcc_callsite_f(callsite, level, name, kind, ...)                       \
  static const std::array<::rust::Str, _TCC_COUNT_VARARGS(__VA_ARGS__)>        \
      _TCC_TOKENPASTE(_TCC_TOKENPASTE(callsite, _FIELDS),                      \
                      __LINE__) = {_TCC_LIST_WRAP(_TCC_STR, __VA_ARGS__)};     \
  _tcc_callsite(callsite, level, name, kind)

#define tcc_callsite(callsite, level, name, kind)                              \
  static const std::array<::rust::Str, 0> _TCC_TOKENPASTE(                     \
      _TCC_TOKENPASTE(callsite, _FIELDS), __LINE__) = {};                      \
  _tcc_callsite(callsite, level, name, kind)

#define tcc_named_event(level, name)                                           \
  tcc_callsite(__CALLSITE, level, name, EVENT);                                \
  if (_TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                         \
    ::cloudchamber::dispatch_tracing_event(                                    \
        _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta());                     \
  }

#define tcc_named_event_f(level, name, ...)                                    \
  tcc_callsite_f(__CALLSITE, level, name, EVENT, __VA_ARGS__);                 \
  if (_TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                         \
    _TCC_TOKENPASTE(::cloudchamber::dispatch_tracing_event,                    \
                    _TCC_COUNT_VARARGS(__VA_ARGS__))(                          \
        _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),                      \
        _TCC_LIST_WRAP(::cloudchamber::FieldValue, __VA_ARGS__));              \
  }

#define tcc_named_event_p(level, name, ...)                                    \
  tcc_callsite_f(__CALLSITE, level, name, EVENT,                               \
                 _TCC_GET_PAIRS_FIRST(__VA_ARGS__));                           \
  if (_TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                         \
    _TCC_TOKENPASTE(::cloudchamber::dispatch_tracing_event,                    \
                    _TCC_COUNT_VARARGS_PAIRS(__VA_ARGS__))(                    \
        _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),                      \
        _TCC_LIST_WRAP(::cloudchamber::FieldValue,                             \
                       _TCC_GET_PAIRS_SECOND(__VA_ARGS__)));                   \
  }

#define tcc_named_event_msg(level, name, msg)                                  \
  tcc_callsite_f(__CALLSITE, level, name, EVENT, message);                     \
  if (_TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                         \
    ::cloudchamber::dispatch_tracing_event1(                                   \
        _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),                      \
        ::cloudchamber::FieldValue(msg));                                      \
  }

#define tcc_named_event_msg_f(level, name, msg, ...)                           \
  tcc_callsite_f(__CALLSITE, level, name, EVENT, message, __VA_ARGS__);        \
  if (_TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                         \
    _TCC_TOKENPASTE(::cloudchamber::dispatch_tracing_event,                    \
                    _TCC_COUNT_VARARGS(msg, __VA_ARGS__))(                     \
        _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),                      \
        ::cloudchamber::FieldValue(msg),                                       \
        _TCC_LIST_WRAP(::cloudchamber::FieldValue, __VA_ARGS__));              \
  }

#define tcc_named_event_msg_p(level, name, msg, ...)                           \
  tcc_callsite_f(__CALLSITE, level, name, EVENT, message,                      \
                 _TCC_GET_PAIRS_FIRST(__VA_ARGS__));                           \
  if (_TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)) {                         \
    _TCC_TOKENPASTE(::cloudchamber::dispatch_tracing_event,                    \
                    _TCC_COUNT_VARARGS_PAIRS(message, msg, __VA_ARGS__))(      \
        _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),                      \
        ::cloudchamber::FieldValue(msg),                                       \
        _TCC_LIST_WRAP(::cloudchamber::FieldValue,                             \
                       _TCC_GET_PAIRS_SECOND(__VA_ARGS__)));                   \
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
      _TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)                            \
          ? ::cloudchamber::new_span(                                          \
                _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta())              \
          : ::cloudchamber::new_disabled_span(                                 \
                _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta());

#define tcc_span_f(ident, level, name, ...)                                    \
  tcc_callsite_f(__CALLSITE, level, name, SPAN, __VA_ARGS__);                  \
  ::rust::Box<::cloudchamber::Span> ident =                                    \
      _TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)                            \
          ? _TCC_TOKENPASTE(::cloudchamber::new_span,                          \
                            _TCC_COUNT_VARARGS(__VA_ARGS__))(                  \
                _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),              \
                _TCC_LIST_WRAP(::cloudchamber::FieldValue, __VA_ARGS__))       \
          : ::cloudchamber::new_disabled_span(                                 \
                _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta());

#define tcc_span_p(ident, level, name, ...)                                    \
  tcc_callsite_f(__CALLSITE, level, name, SPAN,                                \
                 _TCC_GET_PAIRS_FIRST(__VA_ARGS__));                           \
  ::rust::Box<::cloudchamber::Span> ident =                                    \
      _TCC_TOKENPASTE(__CALLSITE_ENABLED, __LINE__)                            \
          ? ident = _TCC_TOKENPASTE(::cloudchamber::new_span,                  \
                                    _TCC_COUNT_VARARGS_PAIRS(__VA_ARGS__))(    \
                _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta(),              \
                _TCC_LIST_WRAP(::cloudchamber::FieldValue,                     \
                               _TCC_GET_PAIRS_SECOND(__VA_ARGS__)))            \
          : ::cloudchamber::new_disabled_span(                                 \
                _TCC_TOKENPASTE(__CALLSITE, __LINE__).get_meta());

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
