#include "cloudchamber/tests/tests.h"
#include "cloudchamber/tracing.h"
namespace tests {
void emit_event() {
  tch_event(::cloudchamber::level::INFO, "tests.emit_event");
}
void emit_event_with_msg() {
  tch_event_msg(::cloudchamber::level::INFO, "tests.emit_event_message",
                "message from event");
}
} // namespace tests
