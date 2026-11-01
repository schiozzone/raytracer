set_project("raytracer")

set_languages("c++23")
add_rules("mode.debug", "mode.release")

target("raytracer")
	set_kind("binary")
	add_files("src/*.cpp")
	add_includedirs("include/")
