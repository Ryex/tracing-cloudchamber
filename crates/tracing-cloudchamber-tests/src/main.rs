use tracing::{info, span, Level};
use tracing_subscriber::{fmt, layer::SubscriberExt as _};

#[cxx::bridge(namespace = "tchtests")]
/// Tests FFI
pub mod test_ffi {
    unsafe extern "C++" {
        include!("tracing-cloudchamber-tests/src/lib.h");
        fn emit_event();
        fn emit_events();
        fn emit_event_with_msg();
        fn emit_events_in_span();
    }
}

mod _using {
    /// prevent symbol elision
    #[allow(unused_imports)]
    pub use tracing_cloudchamber::*;
}

fn main() {
    let subscriber = tracing_subscriber::registry().with(
        fmt::Layer::new()
            .with_thread_names(true)
            .pretty()
            .with_writer(std::io::stdout),
    );
    tracing::subscriber::set_global_default(subscriber).expect("Unable to set a global collector");

    {
        let v: usize = 5;
        let _span = span!(Level::TRACE, "calling_cpp1", v).entered();
        test_ffi::emit_event();
        info!("a rust event now");
    }
    {
        let v: usize = 42;
        let _span = span!(Level::TRACE, "calling_cpp2", v).entered();
        test_ffi::emit_event_with_msg();
        info!("another rust event now");
    }
    {
        test_ffi::emit_events();
    }
    {
        test_ffi::emit_events_in_span();
    }
}

#[cfg(test)]
mod tests {
    use super::test_ffi;
    use tracing::subscriber::with_default;
    use tracing_mock::{expect, subscriber};

    #[test]
    fn emit_event() {
        let (subscriber, handle) = subscriber::mock()
            .event(expect::event().named("tests.emit_event"))
            .only()
            .run_with_handle();

        with_default(subscriber, || {
            test_ffi::emit_event();
        });
        handle.assert_finished();
    }
    #[test]
    fn emit_event_with_msg() {
        let (subscriber, handle) = subscriber::mock()
            .event(
                expect::event()
                    .named("tests.emit_event_message")
                    .with_fields(
                        expect::field("message")
                            .with_value(&"message from event")
                            .and(
                                expect::field("my_test").with_value(&tracing::field::display(
                                    "MyTestStruct { val: 105, str: \"str in struct\" }",
                                )),
                            )
                            .only(),
                    ),
            )
            .only()
            .run_with_handle();

        with_default(subscriber, || {
            test_ffi::emit_event_with_msg();
        });
        handle.assert_finished();
    }

    #[test]
    fn emit_events_in_span() {
        let span1 = expect::span()
            .named("events_in_span")
            .at_level(tracing::Level::INFO);
        let span2 = expect::span()
            .named("a_trace_span")
            .at_level(tracing::Level::TRACE);

        let (subscriber, handle) =
            subscriber::mock()
                .new_span(
                    span1.clone().with_fields(
                        expect::field("span_struct")
                            .with_value(&tracing::field::display(
                                "MyTestStruct { val: 999, str: \"a\" }",
                            ))
                            .only(),
                    ),
                )
                .event(
                    expect::event().with_fields(
                        expect::field("message")
                            .with_value(&"a trace message after span construction but before enter")
                            .only(),
                    ),
                )
                .enter(span1.clone())
                .event(expect::event().named("tests.emit_event"))
                .exit(span1.clone())
                .event(
                    expect::event().at_level(tracing::Level::ERROR).with_fields(
                        expect::field("message")
                            .with_value(&"a fake error msg")
                            .and(expect::field("vector").with_value(&tracing::field::display(
                                "std::vector[10, 9, 43, 1000]",
                            )))
                            .only(),
                    ),
                )
                .new_span(
                    span2.clone().at_level(tracing::Level::TRACE).with_fields(
                        expect::field("an_array")
                            .with_value(&tracing::field::display(
                                "std::array<4>[\"an\", \"array\", \"of\", \"strings\"]",
                            ))
                            .and(
                                expect::field("answers").with_value(&tracing::field::display(
                                    "std::map{\"everything\": 42, \"life\": 42, \"universe\": 42}",
                                )),
                            ),
                    ),
                )
                .enter(span2.clone())
                .event(
                    expect::event().at_level(tracing::Level::INFO).with_fields(
                        expect::field("message")
                            .with_value(&"in_scope info msg")
                            .only(),
                    ),
                )
                .event(
                    expect::event()
                        .named("tests.emit_event_message")
                        .with_fields(
                            expect::field("message")
                                .with_value(&"message from event")
                                .and(
                                    expect::field("my_test").with_value(&tracing::field::display(
                                        "MyTestStruct { val: 105, str: \"str in struct\" }",
                                    )),
                                )
                                .only(),
                        ),
                )
                .exit(span2.clone())
                .only()
                .run_with_handle();

        with_default(subscriber, || {
            test_ffi::emit_events_in_span();
        });
        handle.assert_finished();
    }
}
