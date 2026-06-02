---@diagnostic disable: param-type-mismatch

add_rules("mode.debug", "mode.release")
add_rules("plugin.compile_commands.autoupdate", { outputdir = "build", lsp = "clangd" })

set_policy("build.always_update_configfiles", true)
set_policy("run.autobuild", true)

-- Better verilator support.
set_xmakever("3.0.7")

set_project("archp_cpu")
set_version("0.0.0")

target("archp_cpu", function()
	add_rules("verilator.binary")

	set_toolchains("@verilator")

	add_files("bundled.min.sv")
	add_files("src/top.cpp")

	add_values("verilator.flags", "-O2")

	add_values("verilator.flags", "-Wno-WIDTH", "-Wno-SELRANGE")

	-- https://github.com/xmake-io/xmake/issues/7239
	add_values("verilator.flags", { "--x-assign", "fast" }, { "--x-initial", "fast" }, "--noassert")
end)
