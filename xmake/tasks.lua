local source_dirs = {"include", "src", "entry"}

-- Applies the clang-tidy config
task("lint")
	set_menu({usage = "xmake lint", description = "Run clang-tidy"})
	on_run(function ()
		import("core.project.config")
		import("core.project.project")
		config.load()
		os.exec("xmake project -k compile_commands build")
		local dirs = {"src"}
		if config.get("entry") then table.insert(dirs, "entry") end
		local files = {}
		for _, dir in ipairs(dirs) do
			table.join2(files, os.files(path.join(dir, "**.cpp")))
		end
		local filter = "--header-filter=(include/" .. project.name() .. "|src)/.*"
		os.execv("clang-tidy", table.join({"-p", "build", filter, "--quiet"}, files))
	end)
task_end()

-- Applies the clang-format config
task("fmt")
	set_menu({usage = "xmake fmt [--check]", description = "Run clang-format",
		options = {{"k", "check", "k", nil, "Only check, don't rewrite"}}})
	on_run(function ()
		import("core.base.option")
		local files = {}
		for _, dir in ipairs(source_dirs) do
			table.join2(files, os.files(path.join(dir, "**.cpp")), os.files(path.join(dir, "**.hpp")))
		end
		local args = option.get("check") and {"--dry-run", "--Werror"} or {"-i"}
		os.execv("clang-format", table.join(args, files))
	end)
task_end()

