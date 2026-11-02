-- Off by default
option("entry",  {default = false, description = "Build the executables in entry/"})
option("werror", {default = false, description = "Treat warnings as errors"})
option("llvm",   {default = false, description = "Use LLVM as the only toolchain (clang-cl on Windows)"})

option("linker", {default = "detect", values = {"detect", "system", "mold", "wild", "lld"},
	description = "Linker: 'detect' picks lld with llvm, mold on Linux when installed"})

option("sanitizer", {default = "none", description = "Sanitizers, comma separated: none, address, undefined, leak, thread"})
