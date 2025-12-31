



target("nearmonia-amd64pc")
    set_targetdir("../output/EFI/BOOT")
    set_filename("BOOTX64.EFI")
    set_toolchains("clang-amd64pc")
    add_files("src/*.c", "src/extern/**/*.c")
    add_files("src/arch/amd64pc/*.c", "src/arch/amd64pc/*.S")
    add_cflags("-target x86_64-pc-win32-coff -ffreestanding -nostdlib", {force = true})
    add_asflags("-f win64", {force = true})
    add_ldflags("/machine:amd64 /subsystem:efi_application /entry:LdrEfiEntry", {force = true})


toolchain("clang-amd64pc")
    set_kind("standalone")
    set_toolset("cc", "clang")
    set_toolset("ld", "lld-link")
    
    set_toolset("as", "nasm")
toolchain_end()