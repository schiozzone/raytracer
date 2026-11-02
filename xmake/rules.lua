rule("template")
	on_load(function (target)
		local enabled = {}
		for _, name in ipairs((get_config("sanitizer") or "none"):split(",")) do
			if name ~= "none" then
				enabled[name] = true
				target:set("policy", "build.sanitizer." .. name, true)
			end
		end
		target:data_set("sanitizers", enabled)

		-- /MD links the runtime dynamically, /MT links it statically
		-- clang-cl's sanitizer runtimes are only built against /MD
		if is_plat("windows") then
			target:set("runtimes", not table.empty(enabled) and "MD" or is_mode("debug") and "MDd" or "MT")
		end
	end)
	on_config(function (target)
		import("lib.detect.find_program")
		local is_msvc = target:has_tool("cxx", "cl")
		local is_cl = is_msvc or target:has_tool("cxx", "clang_cl")
		local is_gcc = target:has_tool("cxx", "gcc", "gxx")
		local is_clang = target:has_tool("cxx", "clang", "clangxx")

		-- ASan/USan/LSan/TSan
		local enabled = target:data("sanitizers")
		local sanitized = not table.empty(enabled)
		for name in pairs(enabled) do
			if not ({address = true, undefined = true, leak = true, thread = true})[name] then
				raise("sanitizer: unknown name '%s' (address, undefined, leak, thread)", name)
			end
		end
		if enabled.thread and (enabled.address or enabled.leak) then
			-- Both mess with memory allocation so only one can be enabled at a time
			raise("sanitizer: thread can't be combined with address or leak")
		end
		if is_plat("windows") and (enabled.thread or enabled.leak) then
			raise("sanitizer: Windows supports address (and undefined with clang-cl) only")
		end
		if is_msvc and enabled.undefined then
			raise("sanitizer: MSVC has no undefined sanitizer, use --llvm=y")
		end
		if is_plat("windows") and enabled.undefined and not enabled.address then
			raise("sanitizer: on Windows undefined only links together with address")
		end
		if is_plat("macosx") and enabled.leak then
			raise("sanitizer: macOS has no standalone leak sanitizer")
		end

		-- Compiler flags
		if is_cl then
			target:add("cxxflags", "/W4", "/permissive-", "/Zc:__cplusplus")
			if has_config("werror") then target:add("cxxflags", "/WX") end
		else
			target:add("cxxflags", "-Wall", "-Wextra", "-Wpedantic", "-Wshadow", "-Wconversion", "-Wsign-conversion",
				"-Wold-style-cast", "-Wnon-virtual-dtor", "-Woverloaded-virtual", "-Wcast-align",
				"-Wnull-dereference", "-Wdouble-promotion", "-Wformat=2", "-Wimplicit-fallthrough")
			if is_gcc then
				target:add("cxxflags", "-Wduplicated-cond", "-Wduplicated-branches", "-Wlogical-op", "-Wuseless-cast")
			end
			if has_config("werror") then target:add("cxxflags", "-Werror") end
		end

		local linking = target:is_binary() or target:is_shared()
		if not linking then return end
		local ldkind = target:is_shared() and "shflags" or "ldflags"

		local linker = get_config("linker") or "detect"
		if linker == "detect" then
			if has_config("llvm") then linker = "lld"
			elseif is_plat("linux") and find_program("mold") then linker = "mold"
			else linker = "system" end
		end
		if linker == "mold" or linker == "lld" then
			if not is_cl then target:add(ldkind, "-fuse-ld=" .. linker) end
		elseif linker == "wild" then
			local wild = find_program("wild")
			if not wild then raise("linker: wild is not installed") end
			if is_clang then
				target:add(ldkind, "--ld-path=" .. wild)
			else
				-- Workaround for GCC
				local dir = path.join(os.projectdir(), "build", ".wild")
				os.mkdir(dir)
				os.ln(wild, path.join(dir, "ld"), {force = true})
				target:add(ldkind, "-B" .. dir)
			end
		end

		if not target:is_binary() or is_mode("debug") or sanitized then return end

		if is_plat("linux") then
			target:add("ldflags", "-static-libstdc++", "-static-libgcc")
		elseif is_plat("mingw") then
			target:add("ldflags", "-static")
		end
	end)
rule_end()
