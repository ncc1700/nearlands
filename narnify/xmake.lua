



target("narnify-amd64pc")
    set_targetdir("../output/SYSTEM")
    set_filename("narnify.sys")
    set_toolchains("clang-amd64pc")
    add_files("src/*.c", "src/extern/**/*.c")
    add_files("src/ke/*.c", "src/ke/**/*.c")
    add_files("src/qol/*.c", "src/qol/**/*.c")
    add_files("src/ecs/*.c", "src/ecs/**/*.c")
    add_files("src/ar/amd64pc/*.c", "src/ar/amd64pc/**/*.c", 
                "src/ar/amd64pc/*.S")
    add_files("src/mm/amd64pc/*.c", "src/mm/amd64pc/**/*.c", 
                "src/mm/amd64pc/*.S")
    add_cflags("-g -target x86_64-pc-win32-coff -ffreestanding -nostdlib", {force = true})
    add_asflags("-f win64", {force = true})
    add_ldflags("/machine:amd64 /subsystem:native /entry:ArArchitectureEntry /base:0xFFFFFF8000000000", {force = true})


toolchain("clang-amd64pc")
    set_kind("standalone")
    set_toolset("cc", "clang")
    set_toolset("ld", "lld-link")
    set_toolset("as", "nasm")
toolchain_end()