#[cxx::bridge(namespace = "cloudchamber")]
pub mod ffi {

    pub struct Metadata<'a> {
        name: &'static str,
        target: &'a str,
        level: Level,
        file: &'static str,
        line: u32,
        fields: &'static [&'static str],
        callsite: &'static Callsite,
        kind: Kind,
    }

    #[namespace = "cloudchamber::detail"]
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

    #[namespace = "cloudchamber::detail"]
    pub enum KindValue {
        EVENT,
        SPAN,
        HINT,
    }

    pub struct Kind {
        value: KindValue,
    }

    #[namespace = "cloudchamber::detail"]
    pub enum InterestKind {
        NEVER,
        SOMERTIMES,
        ALWAYS,
    }

    pub struct Interest {
        value: InterestKind,
    }

    #[namespace = "cloudchamber::detail"]
    pub enum FieldValueKind {
        U8,
        U16,
        U32,
        U64,
        I8,
        I16,
        I32,
        I64,
        F32,
        F64,
        STRING,
        STR,
        BOOL,
        DEBUG,
        EMPTY,
    }

    unsafe extern "C++" {
        include!("tracing-cloudchamber/src/lib.h");

        type Callsite;

        #[rust_name = "register"]
        pub fn register_site(self: &Callsite) -> Interest;
        pub fn store_interest(self: &Callsite, value: &Interest);
        pub fn get_meta<'a>(self: &Callsite) -> &Box<RustMetadata<'a>>;

    }

    extern "C++" {
        include!("tracing-cloudchamber/src/lib.h");

        #[namespace = "cloudchamber::detail"]
        type FieldValueKind;
    }

    unsafe extern "C++" {
        include!("tracing-cloudchamber/src/lib.h");

        type FieldValue;

        fn get_u8(&self) -> &u8;
        fn get_u16(&self) -> &u16;
        fn get_u32(&self) -> &u32;
        fn get_u64(&self) -> &u64;
        fn get_i8(&self) -> &i8;
        fn get_i16(&self) -> &i16;
        fn get_i32(&self) -> &i32;
        fn get_i64(&self) -> &i64;
        fn get_f32(&self) -> &f32;
        fn get_f64(&self) -> &f64;
        fn get_bool(&self) -> &bool;
        fn get_string(&self) -> &String;
        fn get_str(&self) -> &String;
        fn get_debug(&self) -> Result<&Box<DisplayValue>>;

        fn get_type(&self) -> FieldValueKind;
    }

    unsafe extern "C++" {
        type ScopeLambda;
        fn call(&self);
    }

    extern "Rust" {
        type DisplayValue;
        pub fn string_to_display_value(value: String) -> Box<DisplayValue>;
    }

    extern "Rust" {
        fn as_str(self: &Level) -> &'static str;

        type RustMetadata<'a>;
        unsafe fn new_rust_metadata<'a>(meta: &'static Metadata<'a>) -> Box<RustMetadata<'a>>;

        /// Returns `true` if this level is enabled by the current subscriber
        fn is_enabled(self: &Level) -> bool;

        fn default_enabled_for_meta(interest: &Interest, meta: &Box<RustMetadata>) -> bool;
        fn dispatch_tracing_event(meta: &'static Box<RustMetadata>);
        fn dispatch_tracing_event1(meta: &'static Box<RustMetadata>, f1: &FieldValue);
        fn dispatch_tracing_event2(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
        );
        fn dispatch_tracing_event3(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
        );
        fn dispatch_tracing_event4(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
        );
        fn dispatch_tracing_event5(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
        );
        fn dispatch_tracing_event6(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
            f6: &FieldValue,
        );
        #[allow(clippy::too_many_arguments)]
        fn dispatch_tracing_event7(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
            f6: &FieldValue,
            f7: &FieldValue,
        );
        #[allow(clippy::too_many_arguments)]
        fn dispatch_tracing_event8(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
            f6: &FieldValue,
            f7: &FieldValue,
            f8: &FieldValue,
        );
        #[allow(clippy::too_many_arguments)]
        fn dispatch_tracing_event9(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
            f6: &FieldValue,
            f7: &FieldValue,
            f8: &FieldValue,
            f9: &FieldValue,
        );
        #[allow(clippy::too_many_arguments)]
        fn dispatch_tracing_event10(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
            f6: &FieldValue,
            f7: &FieldValue,
            f8: &FieldValue,
            f9: &FieldValue,
            f10: &FieldValue,
        );
    }

    extern "Rust" {

        type Span;
        type Entered<'a>;

        /// genter a span and return a guard that will exit the span when droped
        pub fn enter(self: &Span) -> Box<Entered>;
        /// Returns `true` if this `Span` has a field for the given field name
        ///
        /// * `field`: field name
        pub fn has_field(self: &Span, field: &str) -> bool;
        /// Record that the fied described by `field` has the value `value`
        ///
        /// This may be used with `::cloudchamber::FieldEmpty` from the cpp side
        ///
        /// * `field`: field name
        /// * `value`: field value
        pub unsafe fn record<'a>(self: &'a Span, field: &'a str, value: &'a FieldValue)
            -> &'a Span;
        /// Returns `true` if this span was constructed by `Span::none`
        pub fn is_none(self: &Span) -> bool;
        /// Returns `true` if this span was disabled by the subscriber and does not exist
        pub fn is_disabeld(self: &Span) -> bool;
        /// Execute the given function in the context of this span.
        ///
        /// * `f`: function to execute
        fn in_scope(self: &Span, f: &ScopeLambda);

        fn new_disabled_span(meta: &'static Box<RustMetadata>) -> Box<Span>;
        fn current_span() -> Box<Span>;

        fn new_span(meta: &'static Box<RustMetadata>) -> Box<Span>;
        fn new_span1(meta: &'static Box<RustMetadata>, f1: &FieldValue) -> Box<Span>;
        fn new_span2(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
        ) -> Box<Span>;
        fn new_span3(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
        ) -> Box<Span>;
        fn new_span4(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
        ) -> Box<Span>;
        fn new_span5(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
        ) -> Box<Span>;
        fn new_span6(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
            f6: &FieldValue,
        ) -> Box<Span>;
        #[allow(clippy::too_many_arguments)]
        fn new_span7(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
            f6: &FieldValue,
            f7: &FieldValue,
        ) -> Box<Span>;
        #[allow(clippy::too_many_arguments)]
        fn new_span8(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
            f6: &FieldValue,
            f7: &FieldValue,
            f8: &FieldValue,
        ) -> Box<Span>;
        #[allow(clippy::too_many_arguments)]
        fn new_span9(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
            f6: &FieldValue,
            f7: &FieldValue,
            f8: &FieldValue,
            f9: &FieldValue,
        ) -> Box<Span>;
        #[allow(clippy::too_many_arguments)]
        fn new_span10(
            meta: &'static Box<RustMetadata>,
            f1: &FieldValue,
            f2: &FieldValue,
            f3: &FieldValue,
            f4: &FieldValue,
            f5: &FieldValue,
            f6: &FieldValue,
            f7: &FieldValue,
            f8: &FieldValue,
            f9: &FieldValue,
            f10: &FieldValue,
        ) -> Box<Span>;
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
        tracing::Metadata::new(
            value.name,
            value.target,
            (&value.level).into(),
            Some(value.file),
            Some(value.line),
            None,
            tracing::field::FieldSet::new(
                value.fields,
                tracing::callsite::Identifier(value.callsite),
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
    interest.is_always() || tracing::dispatcher::get_default(|default| default.enabled(meta))
}

#[allow(clippy::borrowed_box)]
fn dispatch_tracing_event(meta: &'static Box<RustMetadata>) {
    tracing::Event::dispatch(&meta.0, &meta.0.fields().value_set(&[]));
}

macro_rules! dispatch_tracing_eventN {
    ($name:ident, ($($fN:ident),*)) => {
        #[allow(clippy::borrowed_box)]
        #[allow(clippy::too_many_arguments)]
        fn $name (meta: &'static Box<RustMetadata> $(, $fN : &ffi::FieldValue )*) {
            let mut iter = meta.0.fields().iter();
            let pairs = [
                $(
                    (
                        &::core::iter::Iterator::next(&mut iter).expect("FieldSet corrupted (this is a bug)"),
                        $fN.as_value(),
                    )
                ),*
            ];
            let val_set = meta.0.fields().value_set(&pairs);
            tracing::Event::dispatch(&meta.0, &val_set);
        }
    };
}

dispatch_tracing_eventN! {dispatch_tracing_event1, (f1)}
dispatch_tracing_eventN! {dispatch_tracing_event2, (f1, f2)}
dispatch_tracing_eventN! {dispatch_tracing_event3, (f1, f2, f3)}
dispatch_tracing_eventN! {dispatch_tracing_event4, (f1, f2, f3, f4)}
dispatch_tracing_eventN! {dispatch_tracing_event5, (f1, f2, f3, f4, f5)}
dispatch_tracing_eventN! {dispatch_tracing_event6, (f1, f2, f3, f4, f5, f6)}
dispatch_tracing_eventN! {dispatch_tracing_event7, (f1, f2, f3, f4, f5, f6, f7)}
dispatch_tracing_eventN! {dispatch_tracing_event8, (f1, f2, f3, f4, f5, f6, f7, f8)}
dispatch_tracing_eventN! {dispatch_tracing_event9, (f1, f2, f3, f4, f5, f6, f7, f8, f9)}
dispatch_tracing_eventN! {dispatch_tracing_event10, (f1, f2, f3, f4, f5, f6, f7, f8, f9, f10)}

pub struct DisplayValue(Box<dyn tracing::Value>);
pub fn string_to_display_value(value: String) -> Box<DisplayValue> {
    Box::new(DisplayValue(Box::new(tracing::field::display(value))))
}

impl DisplayValue {
    pub fn as_value(&self) -> &dyn tracing::Value {
        &self.0
    }
}

impl ffi::FieldValue {
    fn as_value(&self) -> Option<&dyn tracing::field::Value> {
        match self.get_type() {
            ffi::FieldValueKind::U8 => Some(self.get_u8() as &dyn tracing::field::Value),
            ffi::FieldValueKind::U16 => Some(self.get_u16() as &dyn tracing::field::Value),
            ffi::FieldValueKind::U32 => Some(self.get_u32() as &dyn tracing::field::Value),
            ffi::FieldValueKind::U64 => Some(self.get_u64() as &dyn tracing::field::Value),
            ffi::FieldValueKind::I8 => Some(self.get_i8() as &dyn tracing::field::Value),
            ffi::FieldValueKind::I16 => Some(self.get_i16() as &dyn tracing::field::Value),
            ffi::FieldValueKind::I32 => Some(self.get_i32() as &dyn tracing::field::Value),
            ffi::FieldValueKind::I64 => Some(self.get_i64() as &dyn tracing::field::Value),
            ffi::FieldValueKind::F32 => Some(self.get_f32() as &dyn tracing::field::Value),
            ffi::FieldValueKind::F64 => Some(self.get_f64() as &dyn tracing::field::Value),
            ffi::FieldValueKind::STRING => Some(self.get_string() as &dyn tracing::field::Value),
            ffi::FieldValueKind::STR => Some(self.get_str() as &dyn tracing::field::Value),
            ffi::FieldValueKind::BOOL => Some(self.get_bool() as &dyn tracing::field::Value),
            ffi::FieldValueKind::DEBUG => match self.get_debug() {
                Ok(s) => Some(s.as_value()),
                Err(_) => None,
            },
            ffi::FieldValueKind::EMPTY => None, // no printing emptys
            _ => None,
        }
    }
}

pub struct Span(tracing::Span);

#[allow(clippy::borrowed_box)]
fn new_span(meta: &'static Box<RustMetadata>) -> Box<Span> {
    Box::new(Span(tracing::Span::new(
        &meta.0,
        &meta.0.fields().value_set(&[]),
    )))
}
#[allow(clippy::borrowed_box)]
fn new_disabled_span(meta: &'static Box<RustMetadata>) -> Box<Span> {
    Box::new(Span(tracing::Span::new_disabled(&meta.0)))
}

fn current_span() -> Box<Span> {
    Box::new(Span(tracing::Span::current()))
}

macro_rules! new_spanN {
    ($name:ident, ($($fN:ident),*)) => {
        #[allow(clippy::borrowed_box)]
        #[allow(clippy::too_many_arguments)]
        fn $name (meta: &'static Box<RustMetadata> $(, $fN : &ffi::FieldValue )*) -> Box<Span> {
            let mut iter = meta.0.fields().iter();
            let pairs = [
                $(
                    (
                        &::core::iter::Iterator::next(&mut iter).expect("FieldSet corrupted (this is a bug)"),
                        $fN.as_value(),
                    )
                ),*
            ];
            let val_set = meta.0.fields().value_set(&pairs);
            Box::new(Span(tracing::Span::new(
                &meta.0,
                &val_set,
            )))
        }
    };
}

new_spanN! {new_span1, (f1)}
new_spanN! {new_span2, (f1, f2)}
new_spanN! {new_span3, (f1, f2, f3)}
new_spanN! {new_span4, (f1, f2, f3, f4)}
new_spanN! {new_span5, (f1, f2, f3, f4, f5)}
new_spanN! {new_span6, (f1, f2, f3, f4, f5, f6)}
new_spanN! {new_span7, (f1, f2, f3, f4, f5, f6, f7)}
new_spanN! {new_span8, (f1, f2, f3, f4, f5, f6, f7, f8)}
new_spanN! {new_span9, (f1, f2, f3, f4, f5, f6, f7, f8, f9)}
new_spanN! {new_span10, (f1, f2, f3, f4, f5, f6, f7, f8, f9, f10)}

pub struct Entered<'a>(#[allow(dead_code)] tracing::span::Entered<'a>);

impl Span {
    pub fn enter(&self) -> Box<Entered> {
        Box::new(Entered(self.0.enter()))
    }

    pub fn has_field(&self, field: &str) -> bool {
        self.0.has_field(field)
    }

    pub fn record(&self, field: &str, value: &ffi::FieldValue) -> &Self {
        self.0.record(field, value.as_value());
        self
    }
    pub fn is_none(&self) -> bool {
        self.0.is_none()
    }

    pub fn is_disabeld(&self) -> bool {
        self.0.is_disabled()
    }

    fn in_scope(&self, f: &ffi::ScopeLambda) {
        let _ = self.0.enter();
        f.call();
    }
}

/// for cargo expand inspection only
mod unused {
    #[allow(dead_code)]
    fn test_tracing_event() {
        tracing::event!(tracing::Level::INFO, test = 5, "messge in event");
    }
    #[allow(dead_code)]
    fn test_tracing_span() {
        let v = 5;
        let _span = tracing::span!(tracing::Level::TRACE, "name of span", v);
    }
}
