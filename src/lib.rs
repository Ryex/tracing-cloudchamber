#[cxx::bridge(namespace = "cloudchamber")]
pub mod ffi {
    pub struct Metadata<'a> {
        name: &'static str,
        target: &'a str,
        level: Level,
        file: &'static str,
        line: u32,
        fields: &'static [&'static str],
        callsite: *const Callsite,
        kind: Kind,
    }

    pub enum LevelValue {
        ERROR,
        WARN,
        INFO,
        DEBUG,
        TRACE,
    }

    pub struct Level {
        value: LevelValue,
    }

    pub enum KindValue {
        EVENT,
        SPAN,
        HINT,
    }

    pub struct Kind {
        value: KindValue,
    }

    pub enum InterestKind {
        NEVER,
        SOMERTIMES,
        ALWAYS,
    }

    pub struct Interest {
        value: InterestKind,
    }

    unsafe extern "C++" {
        include!("tracing-cloudchamber/src/lib.h");

        type Callsite;

        #[rust_name = "register"]
        pub fn register_site(self: &Callsite) -> Interest;
        pub fn store_interest(self: &Callsite, value: &Interest);
        pub fn get_meta<'a>(self: &Callsite) -> &Box<RustMetadata<'a>>;
    }

    extern "Rust" {
        fn as_str(self: &Level) -> &'static str;

        type RustMetadata<'a>;
        unsafe fn new_rust_metadata<'a>(meta: &'static Metadata<'a>) -> Box<RustMetadata<'a>>;

        fn is_enabled(self: &Level) -> bool;

        fn default_enabled_for_meta(interest: &Interest, meta: &Box<RustMetadata>) -> bool;
        fn dispatch_tracing_event(meta: &'static Box<RustMetadata>);
    }
}

unsafe impl Sync for ffi::Callsite {}
unsafe impl Send for ffi::Callsite {}

impl ffi::Level {
    #[allow(dead_code)]
    const ERROR: Self = Self {
        value: ffi::LevelValue::ERROR,
    };
    #[allow(dead_code)]
    const WARN: Self = Self {
        value: ffi::LevelValue::WARN,
    };
    #[allow(dead_code)]
    const INFO: Self = Self {
        value: ffi::LevelValue::INFO,
    };
    #[allow(dead_code)]
    const DEBUG: Self = Self {
        value: ffi::LevelValue::DEBUG,
    };
    #[allow(dead_code)]
    const TRACE: Self = Self {
        value: ffi::LevelValue::TRACE,
    };

    pub fn as_str(&self) -> &'static str {
        match self.value {
            ffi::LevelValue::ERROR => tracing::Level::ERROR.as_str(),
            ffi::LevelValue::WARN => tracing::Level::WARN.as_str(),
            ffi::LevelValue::INFO => tracing::Level::INFO.as_str(),
            ffi::LevelValue::DEBUG => tracing::Level::DEBUG.as_str(),
            ffi::LevelValue::TRACE => tracing::Level::TRACE.as_str(),
            _ => "unknown",
        }
    }

    pub fn is_enabled(&self) -> bool {
        let tlevel: tracing::Level = self.into();
        tlevel <= tracing::level_filters::STATIC_MAX_LEVEL
            && tlevel <= tracing::level_filters::LevelFilter::current()
    }
}

impl From<&ffi::Level> for tracing::Level {
    fn from(value: &ffi::Level) -> Self {
        match value.value {
            ffi::LevelValue::ERROR => tracing::Level::ERROR,
            ffi::LevelValue::WARN => tracing::Level::WARN,
            ffi::LevelValue::INFO => tracing::Level::INFO,
            ffi::LevelValue::DEBUG => tracing::Level::DEBUG,
            ffi::LevelValue::TRACE => tracing::Level::TRACE,
            _ => tracing::Level::TRACE,
        }
    }
}

impl ffi::Kind {
    #[allow(dead_code)]
    const EVENT: Self = Self {
        value: ffi::KindValue::EVENT,
    };
    #[allow(dead_code)]
    const SPAN: Self = Self {
        value: ffi::KindValue::SPAN,
    };
    #[allow(dead_code)]
    const HINT: Self = Self {
        value: ffi::KindValue::HINT,
    };

    pub fn is_span(&self) -> bool {
        matches!(self.value, ffi::KindValue::SPAN)
    }
    pub fn is_event(&self) -> bool {
        matches!(self.value, ffi::KindValue::EVENT)
    }
    pub fn is_hint(&self) -> bool {
        !matches!(self.value, ffi::KindValue::SPAN) && !matches!(self.value, ffi::KindValue::EVENT)
    }
}

impl From<&ffi::Kind> for tracing::metadata::Kind {
    fn from(value: &ffi::Kind) -> Self {
        match value.value {
            ffi::KindValue::EVENT => tracing::metadata::Kind::EVENT,
            ffi::KindValue::SPAN => tracing::metadata::Kind::SPAN,
            ffi::KindValue::HINT => tracing::metadata::Kind::HINT,
            _ => tracing::metadata::Kind::HINT,
        }
    }
}

impl From<&ffi::Interest> for tracing_core::Interest {
    fn from(value: &ffi::Interest) -> Self {
        match value.value {
            ffi::InterestKind::NEVER => tracing_core::Interest::never(),
            ffi::InterestKind::SOMERTIMES => tracing_core::Interest::sometimes(),
            ffi::InterestKind::ALWAYS => tracing_core::Interest::always(),
            _ => tracing_core::Interest::sometimes(),
        }
    }
}
impl From<&tracing_core::Interest> for ffi::Interest {
    fn from(value: &tracing_core::Interest) -> Self {
        if value.is_never() {
            ffi::Interest {
                value: ffi::InterestKind::NEVER,
            }
        } else if value.is_always() {
            ffi::Interest {
                value: ffi::InterestKind::ALWAYS,
            }
        } else {
            ffi::Interest {
                value: ffi::InterestKind::SOMERTIMES,
            }
        }
    }
}

impl<'a> From<&ffi::Metadata<'a>> for tracing::Metadata<'a> {
    fn from(value: &ffi::Metadata<'a>) -> Self {
        let callsite_ref = unsafe { &*value.callsite };
        tracing::Metadata::new(
            value.name,
            value.target,
            (&value.level).into(),
            Some(value.file),
            Some(value.line),
            None,
            tracing::field::FieldSet::new(
                value.fields,
                tracing::callsite::Identifier(callsite_ref),
            ),
            (&value.kind).into(),
        )
    }
}

pub struct RustMetadata<'a>(pub tracing::Metadata<'a>);

impl<'a> RustMetadata<'a> {
    pub fn new(meta: &'static ffi::Metadata<'a>) -> Box<Self> {
        Box::new(Self(meta.into()))
    }
}

fn new_rust_metadata<'a>(meta: &'static ffi::Metadata<'a>) -> Box<RustMetadata<'a>> {
    RustMetadata::new(meta)
}

pub struct Span(tracing::Span);

impl Span {
    pub fn enter() {}
}

impl ffi::Callsite {
    #[allow(dead_code)]
    const UNREGISTERED: u8 = 0;
    #[allow(dead_code)]
    const REGISTERING: u8 = 1;
    #[allow(dead_code)]
    const REGISTERED: u8 = 2;

    #[allow(dead_code)]
    const INTEREST_NEVER: u8 = 0;
    #[allow(dead_code)]
    const INTEREST_SOMETIMES: u8 = 1;
    #[allow(dead_code)]
    const INTEREST_ALWAYS: u8 = 2;
    #[allow(dead_code)]
    const INTEREST_EMPTY: u8 = 0xFF;
}

impl tracing::Callsite for ffi::Callsite {
    #[inline(always)]
    fn metadata(&self) -> &tracing::Metadata<'_> {
        &self.get_meta().as_ref().0
    }
    fn set_interest(&self, interest: tracing_core::Interest) {
        self.store_interest(&(&interest).into());
    }
}

#[allow(clippy::borrowed_box)]
fn default_enabled_for_meta(interest: &ffi::Interest, meta: &Box<RustMetadata>) -> bool {
    let interest: tracing_core::Interest = interest.into();
    let meta = &meta.as_ref().0;
    let enabled_for_meta = tracing::dispatcher::get_default(|default| default.enabled(meta));
    let interest_is_always = interest.is_always();
    println!(
        "enabled_for: interest_is_always = {interest_is_always}, enabled_for_meta = {enabled_for_meta}"
    );
    interest.is_always() || tracing::dispatcher::get_default(|default| default.enabled(meta))
}

#[allow(clippy::borrowed_box)]
fn dispatch_tracing_event(meta: &'static Box<RustMetadata>) {
    let meta = &meta.0;
    let val_set = meta.fields().value_set(&[]);
    tracing::Event::dispatch(meta, &val_set);
}

#[cfg(debug_assertions)]
#[cxx::bridge(namespace = "tests")]
/// Tests FFI
pub mod test_ffi {
    unsafe extern "C++" {
        include!("cloudchamber/tests/tests.h");
        fn emit_spaned_event();
    }
}
mod unused {
    fn test_tracing_event() {
        tracing::event!(tracing::Level::INFO, test = 5, "messge in event");
    }
}

#[cfg(test)]
mod tests {
    use super::test_ffi;
    use tracing::subscriber::with_default;
    use tracing_mock::{expect, subscriber};

    #[test]
    fn emit_spaned_event() {
        let (subscriber, handle) = subscriber::mock()
            .event(expect::event().named("tests.emit_spaned_event"))
            .only()
            .run_with_handle();

        with_default(subscriber, || {
            test_ffi::emit_spaned_event();
        });
        handle.assert_finished();
    }
}
