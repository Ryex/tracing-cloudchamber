#include "tracing-cloudchamber-tests/src/lib.h"
#include "cloudchamber/tracing.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
struct MyTestStruct {
  int val;
  std::string str;
};
std::string to_string(MyTestStruct const &self) {
  std::ostringstream out;
  out << "MyTestStruct { val: " << self.val << ", str: \"" << self.str
      << "\" }";
  return out.str();
}

struct MyTestStruct2 {
  int val;
  std::string str;

  friend std::ostream &operator<<(std::ostream &os, MyTestStruct2 const &self) {
    os << "MyTestStruct { val: " << self.val << ", str: \"" << self.str
       << "\" }";
    return os;
  }
};

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

  MyTestStruct2 span_struct{999, "a"};

  tcc_span_f(_span, ::cloudchamber::level::INFO, "events_in_span", span_struct);

  tcc_trace_msg("a trace message after span construction but before enter");

  {
    auto _guard = _span->enter();
    emit_event();
  }

  std::vector<int> a_vec = {10, 9, 43, 1000};
  tcc_error_msg_p("a fake error msg", vector, a_vec);

  std::array an_array = {"an", "array", "of", "strings"};
  std::map<std::string, int> answers{
      {"life", 42}, {"universe", 42}, {"everything", 42}};

  tcc_trace_span_f(_t_span, "a_trace_span", an_array, answers);

  _t_span->in_scope(::cloudchamber::ScopeLambda([=]() {
    tcc_info_msg("in_scope info msg");
    emit_event_with_msg();
  }));
}
} // namespace tchtests
