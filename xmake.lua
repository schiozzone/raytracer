set_project("raytracer")

set_languages("c++23")
add_rules("mode.debug", "mode.release")
set_toolchains("clang-cl")

target("raytracer")
	set_kind("binary")
	add_files("src/*.cpp")
	add_includedirs("include/")
	add_cxxflags("-W4", "-Wextra", "-Wpedantic", "-Wshadow", "-Wconversion", "-Wsign-conversion",
				"-Wold-style-cast", "-Wnon-virtual-dtor", "-Woverloaded-virtual", "-Wcast-align",
				"-Wnull-dereference", "-Wdouble-promotion", "-Wformat=2", "-Wimplicit-fallthrough")
