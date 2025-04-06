#pragma once

#include <iostream>
#include <string>
#include <type_traits>

struct MyTestStruct {
  int val;
  std::string str;
};

template <typename T,
          std::enable_if_t<std::is_same_v<T, MyTestStruct>, bool> = true>
std::string tcc_field_format(const T &value);

namespace tchtests {
void emit_events();
void emit_event();
void emit_event_with_msg();
void emit_events_in_span();
} // namespace tchtests
