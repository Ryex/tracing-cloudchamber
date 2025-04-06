#include "tracing-cloudchamber-tests/src/lib.h"
#include "cloudchamber/tracing.h"

template <typename T, std::enable_if_t<std::is_same_v<T, MyTestStruct>, bool>>
std::string tcc_field_format(const T &value) {
  std::string ret = ::cloudchamber::detail::string_format(
      "MyTestStruct { val: %i, str: \"%s\" }", value.val, value.str.c_str());
  return ret;
}

namespace tchtests {
void emit_event() {
  tcc_named_event(::cloudchamber::level::INFO, "tests.emit_event");
}
void emit_events() { tcc_event(::cloudchamber::level::INFO); }
void emit_event_with_msg() {
  MyTestStruct my_test{105, "str in struct"};
  tcc_named_event_msg_p(::cloudchamber::level::WARN, "tests.emit_event_message",
                        "message from event", my_test, my_test);
}
void emit_events_in_span() {

  MyTestStruct span_struct{999, "a"};

  tcc_span_f(_span, ::cloudchamber::level::INFO, "events_in_span", span_struct);

  tcc_trace_msg("a trace message after span construction but before enter");

  {
    auto _guard = _span->enter();
    emit_event();
  }

  tcc_error_msg("a fake error msg");

  tcc_trace_span(_t_span, "a_trace_span");

  _t_span->in_scope(::cloudchamber::ScopeLambda([=]() {
    tcc_info_msg("in_scope info msg");
    emit_event_with_msg();
  }));
}
} // namespace tchtests
