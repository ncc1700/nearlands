



target("nearmonia-amd64pc")
    set_targetdir("../output/EFI/BOOT")
    set_filename("BOOTX64.EFI")
    set_toolchains("clang-amd64pc")
    add_files("src/*.c", "src/extern/**/*.c")
    add_files("src/fs/*.c", "src/fs/**/*.c")
    add_files("src/ldr/*.c", "src/ldr/**/*.c")
    add_files("src/qol/*.c", "src/qol/**/*.c")
    add_includedirs("src/includes")
    add_files("src/ar/amd64pc/*.c", "src/ar/amd64pc/*.S")
    add_files("src/mm/amd64pc/*.c", "src/mm/amd64pc/*.S")
    add_cflags("-g -target x86_64-pc-win32-coff -ffreestanding -nostdlib", {force = true})
    add_asflags("-f win64", {force = true})
    add_includedirs("src/includes")
    add_ldflags("/machine:amd64 /subsystem:efi_application /entry:LdrEfiEntry", {force = true})


toolchain("clang-amd64pc")
    set_kind("standalone")
    set_toolset("cc", "clang")
    set_toolset("ld", "lld-link")
    
    set_toolset("as", "nasm")
toolchain_end()

toolchain("clang-ia64")
    set_kind("standalone")
    set_toolset("cc", "clang")
    set_toolset("ld", "clang")
    set_toolset("as", "clang")
toolchain_end()