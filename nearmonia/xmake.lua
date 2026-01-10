



target("nearmonia-amd64pc")
    set_targetdir("../output/EFI/BOOT")
    set_filename("BOOTX64.EFI")
    set_toolchains("clang-amd64pc")
    add_files("src/*.c", "src/extern/**/*.c")
    add_files("src/arch/amd64pc/*.c", "src/arch/amd64pc/*.S")
    add_cflags("-g -target x86_64-pc-win32-coff -ffreestanding -nostdlib", {force = true})
    add_asflags("-f win64", {force = true})
    add_ldflags("/machine:amd64 /subsystem:efi_application /entry:LdrEfiEntry", {force = true})

target("nearmonia-ia64")
    set_targetdir("../output/EFI/BOOT")
    set_filename("BOOTIA64.EFI")
    set_toolchains("clang-ia64")
    add_files("src/*.c", "src/extern/**/*.c")
    add_files("src/arch/ia64/*.c", "src/arch/ia64/*.S")
    add_cflags("-target ia64-unknown-windows -ffreestanding -nostdlib", {force = true})
    add_asflags("-target ia64-unknown-windows -ffreestanding -nostdlib", {force = true})
    add_ldflags("-target ia64-unknown-windows -ffreestanding -nostdlib", {force = true})


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