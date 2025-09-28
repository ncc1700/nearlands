target("kernel")
    set_toolchains("clangpe")
    set_filename("kernel.elf")
    set_targetdir("../bin/SYSTEM/")
    add_files("src/*.c", "src/**/*.c");
    add_files("src/*.asm", "src/**/*.asm");
    add_cflags("-target x86_64-pc-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fno-rtti", {force = true})
    add_cxxflags("-target x86_64-pc-none-elf", "-ffreestanding", "-nostdlib", "-fno-exceptions", "-fno-rtti", {force = true})
    add_asflags("-f elf64") 
    add_ldflags("-nostdlib -T linker/linker.ld", {force = true})


toolchain("clangpe")
    set_kind("standalone")
    set_toolset("cc", "clang")
    set_toolset("as", "nasm", {force = true})
    add_asflags("-f elf64", {force = true})
    set_toolset("ld", "ld.lld")
toolchain_end();