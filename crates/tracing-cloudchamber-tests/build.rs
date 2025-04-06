use std::env;

fn main() {
    cxx_build::CFG.doxygen = true;
    let sources = vec!["src/main.rs"];

    let cpp_sources = vec!["src/lib.cc"];

    println!("cargo:rerun-if-changed=src/lib.h");

    let mut cxx = cxx_build::bridges(&sources);
    cxx.cpp(true)
        .std("c++17")
        .flag_if_supported("/Zc:__cplusplus");

    if env::var_os("DEBUG").as_deref() != Some("true".as_ref()) {
        cxx.flag_if_supported("-Og");
    }

    for file in sources {
        println!("cargo:rerun-if-changed={file}");
    }
    for file in cpp_sources {
        cxx.file(file);
        println!("cargo:rerun-if-changed={file}");
    }

    cxx.compile("tracingcctests");
}
