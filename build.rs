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

    let sources = vec!["src/lib.rs"];

    let mut cpp_sources = vec!["src/lib.cc"];

    #[cfg(debug_assertions)]
    cpp_sources.push("tests/tests.cc");

    let mut cxx = cxx_build::bridges(&sources);
    cxx.cpp(true)
        .std("c++17")
        .flag_if_supported("/Zc:__cplusplus")
        .flag_if_supported("-U_FORTIFY_SOURCE");

    for file in sources {
        println!("cargo:rerun-if-changed={file}");
    }
    for file in cpp_sources {
        cxx.file(file);
        println!("cargo:rerun-if-changed={file}");
    }

    cxx.compile("tracingcc");
}
