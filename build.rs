use cxx_build::CFG;
use std::path::Path;
use std::{env, fs};

fn main() {
    // CFG.include_prefix = "cloudchamber";

    let out_dir = env::var_os("OUT_DIR").unwrap();
    let headers = Path::new(&out_dir).join("headers");
    CFG.exported_header_dirs.push(&headers);

    // We contain `include/tracing.h` locally, but
    // downstream will use `#include "cloudchamber/tracing.h"`
    let cloudchanber = headers.join("cloudchamber");
    fs::create_dir_all(&cloudchanber).unwrap();
    fs::copy("include/tracing.h", cloudchanber.join("tracing.h")).unwrap();

    println!("cargo:rerun-if-changed=include/tracing.h");
    #[cfg(debug_assertions)]
    {
        let tests = cloudchanber.join("tests");
        fs::create_dir_all(&tests).unwrap();
        fs::copy("tests/tests.h", tests.join("tests.h")).unwrap();
        println!("cargo:rerun-if-changed=tests/tests.h");
    }

    let mut sources = vec!["src/lib.rs"];

    // #[cfg(debug_assertions)]
    // sources.push("tests/test.rs");

    let mut cpp_sources = vec!["src/lib.cc"];

    #[cfg(debug_assertions)]
    cpp_sources.push("tests/tests.cc");

    let mut builder = cxx_build::bridges(&sources);
    builder
        .cpp(true)
        .std("c++17")
        .flag_if_supported("/Zc:__cplusplus");

    for file in sources {
        println!("cargo:rerun-if-changed={file}");
    }
    for file in cpp_sources {
        builder.file(file);
        println!("cargo:rerun-if-changed={file}");
    }

    builder.compile("tracing-cloudchamber");
}
