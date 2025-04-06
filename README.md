# Tracing - CloudChamber

[![Crates.io][crates-badge]][crates-url]
[![Documentation][docs-badge]][docs-url]
[![MIT licensed][mit-badge]][mit-url]
[![Build Status][actions-badge]][actions-url]

[crates-badge]: https://img.shields.io/crates/v/tracing-cloudchamber.svg
[crates-url]: https://crates.io/crates/tracing-cloudchamber
[docs-badge]: https://docs.rs/tracing-cloudchamber/badge.svg
[docs-url]: https://docs.rs/tracing-cloudchamber
[docs-master-badge]: https://img.shields.io/badge/docs-master-blue
[mit-badge]: https://img.shields.io/badge/license-MIT-blue.svg
[mit-url]: LICENSE
[actions-badge]: https://github.com/Ryex/tracing-cloudchamber/workflows/CI/badge.svg
[actions-url]: https://github.com/Ryex/tracing-cloudchamber/actions?query=workflow%3ACI

## Overview

[`tracing`](https://crates.io/crates/tracing) is a framework for instrumenting Rust programs to collect
structured, event-based diagnostic information. But sometimes you need to write a C++ ffi
and this beautiful framework is unavailable to you in your C++ code.

Not anymore! `tracing-cloudchanber` extends `tracing` with a ffi via [`cxx`](https://crates.io/crates/cxx)
so you can construct spans and emit events from your C++ code with the same safety as you could in Rust.

## Usage

Assuming you have set up your project correctly and either added `tracing-cloudchamber` as a direct dependency
or your using something like [corrosion](https://github.com/corrosion-rs/corrosion) to link `tracing-cloudchamber` to your C++ code ...

You only need to do two things:

1. add the header

```cpp
#include "cloudchamber/tracing.h"
```

2. prevent the linker from eliding "unused" symbols

```rust
mod _using {
    /// prevent symbol elision
    #[allow(unused_imports)]
    pub use tracing_cloudchamber::*;
}
```

Then you'll be able to do this!

```cpp
void emit_events_in_span() {

  MyTestStruct span_struct{999, "a"};

  tcc_span_f(_span, ::cloudchamber::level::INFO, "events_in_span", span_struct);

  tcc_trace_msg("a trace message after span construction but before enter");

  {
    auto _guard = _span->enter();
    emit_event();
  }

  tcc_error_msg("a fake error msg");
}
```

### Emitting Events

`tracing-cloudchamber`'s ffi is implemented somewhat restrictively and based on
re-implementing how `tracing`'s own macros work.
That is, there are a selection of macros that expand to statically defined call-sites (`::cloudchamber::Callsite`)
with associated static metadata. Just like tracing all field names must be defined up front and field values must either be primitive or
there must be an overload of the `tcc_field_format` function that accepts `const&` to it.

The macros support defining fields in two formats: Paired name and value pairs, or assumed named from the `ident` passed in

e.g.

```cpp
int val = 10;
tcc_info_msg_f(val)
```

will emit a `INFO` level event with a field named `val` and the value `10`

#### Levels

```cpp
::cloudchamber::level::ERROR;
::cloudchamber::level::WARN;
::cloudchamber::level::INFO;
::cloudchamber::level::DEBUG;
::cloudchamber::level::TRACE;
```

#### Available event macros

##### Raw Event

```cpp
/// emit an event named after the call-site (file:line)
tcc_event(level);
/// emit an event with passed fields, names are assumed from `idents` passed
/// e.g, `tcc_event_f(::cloudchamber::level::TRACE, val)`
tcc_event_f(level, <ident>...);
/// emit an event with passed fields; fields are defined as named, value
/// e.g. `tcc_event_p(::cloudchamber::level::TRACE, val, 10)`
/// note that names are not quoted. that's done by the preprocessor.
tcc_event_p(level, <name, value>...);
/// emit an event with a field "message" (tracing's default) using the passed value
tcc_event_msg(level, msg);
/// emit an event with a field "message" (tracing's default) using the passed value
/// adds field using names form the `idents` passed
/// e.g. `tcc_event_msg_f(::cloudchamber::level::TRACE, "shaving a yak", val)`
tcc_event_msg_f(level, msg, <ident>...);
/// emit an event with a field "message" (tracing's default) using the passed value
/// adds field using paired names and values
/// e.g. `tcc_event_msg_p(::cloudchamber::level::TRACE, "shaving a yak", val, 10)`
tcc_event_msg_p(level, msg, <name, value>...);
```

##### Raw Named events

```cpp
tcc_named_event(level, name);
tcc_named_event_f(level, name, <ident>...);
tcc_named_event_p(level, name, <name, value>...);
tcc_named_event_msg(level, name, msg);
tcc_named_event_msg(level, name, msg, <ident>...);
tcc_named_event_msg(level, name, msg, <name, value>...);
```

##### ERROR level Events

```cpp
tcc_error();
tcc_error_f(<ident>...);
tcc_error_p(<name, value>...);
tcc_error_msg(msg);
tcc_error_msg_f(msg, <ident>...);
tcc_error_msg_p(msg, <name, value>...);
```

##### WARN level Events

```cpp
tcc_warn();
tcc_warn_f(<ident>...);
tcc_warn_p(<name, value>...);
tcc_warn_msg(msg);
tcc_warn_msg_f(msg, <ident>...);
tcc_warn_msg_p(msg, <name, value>...);
```

##### INFO level Events

```cpp
tcc_info();
tcc_info_f(<ident>...);
tcc_info_p(<name, value>...);
tcc_info_msg(msg);
tcc_info_msg_f(msg, <ident>...);
tcc_info_msg_p(msg, <name, value>...);
```

##### DEBUG level Events

```cpp
tcc_debug();
tcc_debug_f(<ident>...);
tcc_debug_p(<name, value>...);
tcc_debug_msg(msg);
tcc_debug_msg_f(msg, <ident>...);
tcc_debug_msg_p(msg, <name, value>...);
```

##### TRACE level Events

```cpp
tcc_trace();
tcc_trace_f(<ident>...);
tcc_trace_p(<name, value>...);
tcc_trace_msg(msg);
tcc_trace_msg_f(msg, <ident>...);
tcc_trace_msg_p(msg, <name, value>...);
```

### Using Spans

Spans work almost just like in `tracing`.
There are a set of macros that mirror the event macros to define a span and it's call-site.

You can then use `auto _guard = span->enter();` and `span->in_scope(...);`

Example:

```cpp
tcc_span_f(_span, ::cloudchamber::level::INFO, "events_in_span", span_struct);
{
  auto _guard = _span->enter();
  tcc_trace_msg("a trace message inside a span");
}
tcc_trace_msg("a trace message after and outside a span");
```

The `tcc_span_<...>(ident, ...)` span macros expand to a call-site and a Boxed `::cloudchamber::Span`

```cpp
::rust::Box<::cloudchamber::Span> ident = /* span construction dependant on call-site enable */
```

#### Available span macros

##### Raw Spans

```cpp
/// construct a span held by `ident` for level `level` named `name`
tcc_span(ident, level, name)
tcc_span_f(ident, level, name, <ident>...)
tcc_span_p(ident, level, name, <name, value>...)
```

##### ERROR Spans

```cpp
tcc_error_span(ident, name)
tcc_error_span_f(ident, name, <ident>...)
tcc_error_span_p(ident, name, <name, value>...)
```

##### WARN Spans

```cpp
tcc_warn_span(ident, name)
tcc_warn_span_f(ident, name, <ident>...)
tcc_warn_span_p(ident, name, <name, value>...)
```

##### INFO Spans

```cpp
tcc_info_span(ident, name)
tcc_info_span_f(ident, name, <ident>...)
tcc_info_span_p(ident, name, <name, value>...)
```

##### DEBUG Spans

```cpp
tcc_debug_span(ident, name)
tcc_debug_span_f(ident, name, <ident>...)
tcc_debug_span_p(ident, name, <name, value>...)
```

##### TRACE Spans

```cpp
tINFOcc_trace_span(ident, name)
tcc_trace_span_f(ident, name, <ident>...)
tcc_trace_span_p(ident, name, <name, value>...)
```

### Formatting field values

To attach a field value we need to know how to turn it into a `&dyn tracing::field::Value`.
Fortunately `tracing-cloudchamber` knows how to construct this for all "stringable" types
and some containers there-of.

In this case "stringable" means:

- Any type for which `std::to_string` exists.
- Any type for which a `to_string(T const&)` function exists in scope.
- Any type for which a `std::ostream& operator<<(std::ostream&, Tconst&)` function exists in scope
- Any type for which `std::string ::cloudchamber::field_format(T const&)` exists

To handle containers the following are implemented:

- `std::string ::cloudchamber::field_format(std::array<T, N> const&)` for any "stringable" type `T`
- `std::string ::cloudchamber::field_format(std::vector<T> const&)` for any "stringable" type `T`
- `std::string ::cloudchamber::field_format(std::map<K, V> const&)` for any "stringable" types `K` and `V`

For custom types or containers simple implement one of those functions

e.g.

```cpp
// by a std::string to_string(T const &)
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

// by std::ostring& operator<<
struct MyTestStruct2 {
  int val;
  std::string str;

  friend std::ostream &operator<<(std::ostream &os, MyTestStruct2 const &self) {
    os << "MyTestStruct { val: " << self.val << ", str: \"" << self.str
       << "\" }";
    return os;
  }
};
```
