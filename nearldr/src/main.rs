#![no_main]
#![no_std]


use core::{panic, time::Duration};
use log::{error, info};
use uefi::{allocator::Allocator, prelude::*, proto::media::file::FileHandle, runtime::ResetType};

mod fileio;


extern crate alloc;
use alloc::vec::Vec;



#[global_allocator]
static GLOBAL: Allocator = Allocator;

#[entry]
fn main() -> Status {
    uefi::helpers::init().unwrap();
    info!("Entering NearLDR");
    let firmware = uefi::system::firmware_vendor();
    info!("Running on firmware {} revision {}", firmware, uefi::system::firmware_revision());

    if fileio::check_if_file_exists(uefi::cstr16!("\\nearldr.conf")) == false {
        error!("config file doesn't exist, system shutting down in 10 seconds");
        uefi::boot::stall(Duration::from_secs(10));
        uefi::runtime::reset(ResetType::SHUTDOWN, Status::SUCCESS, None);
    }
    let file: FileHandle;
    let content: Vec<u8>;
    let fileres = fileio::open_file(uefi::cstr16!("\\nearldr.conf"));
    
    match fileres {
        Ok(f) => {
            file = f;
        },
        Err(e) => {
            error!("{}", e);
            loop{}
        },
    }

    let contentres = fileio::read_file(file);

    match contentres {
        Ok(c) => {
            content = c;
        },
        Err(e) => {
            error!("{}", e);
            loop{}
        },
    }
    // this unsafe can probably be removed
    unsafe {
        let h = uefi::CStr8::from_bytes_with_nul_unchecked(&content);
        info!("{}", h);
    }
    

    uefi::boot::stall(Duration::from_secs(10));
    loop {}
}
