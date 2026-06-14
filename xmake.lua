---@diagnostic disable: param-type-mismatch

add_rules("plugin.compile_commands.autoupdate", { outputdir = "build", lsp = "clangd" })

set_policy("build.always_update_configfiles", true)
set_policy("run.autobuild", true)

-- Better verilator support.
-- See https://github.com/xmake-io/xmake/issues/7239
set_xmakever("3.0.7")

set_project("archp_cpu")
set_version("0.0.0")

target("Vtop", function()
	add_rules("verilator.binary")
	set_toolchains("@verilator")

	set_symbols("hidden")
	set_optimize("faster")
	set_strip("all")

	add_files("bundled.sv")

	-- DPI files
	add_files("src/**.sv", "src/**.cpp")
	add_headerfiles("src/**.hpp")

	add_values("verilator.flags", "-O2")

	if is_mode("trace") then
		-- See https://github.com/xmake-io/xmake/pull/7601
		add_defines("VM_TRACE") -- TODO: remove this after the PR is merged and released.
		add_values("verilator.flags", "--trace-fst")
		add_values("verilator.flags", "-DTRACE")
	end

	add_values("verilator.flags", { "--x-assign", "fast" }, { "--x-initial", "fast" }, "--noassert")
end)
