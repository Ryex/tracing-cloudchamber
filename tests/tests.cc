#include "cloudchamber/tests/tests.h"
#include "cloudchamber/tracing.h"
namespace tests {
void emit_spaned_event() {
  tchEvent(::cloudchamber::level::INFO, "tests.emit_spaned_event");
}
} // namespace tests
