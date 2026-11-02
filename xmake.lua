set_project("raytracer")

includes("xmake/options.lua", "xmake/rules.lua", "xmake/tasks.lua")

set_languages("c++23")
add_rules("mode.debug", "mode.release")
add_rules("template")

if has_config("llvm") then
	set_toolchains(is_plat("windows") and "clang-cl" or "clang")
end

target("raytracer")
	set_kind("static")
	add_files("src/**.cpp")
	add_includedirs("include", {public = true})
	add_includedirs("src")
	add_headerfiles("include/(raytracer/*.hpp)")

-- separating domain logic (src/) from the presentation
if has_config("entry") then
	target("raytracer-cli")
		set_kind("binary")
		add_files("entry/*.cpp")
		add_deps("raytracer")
end
