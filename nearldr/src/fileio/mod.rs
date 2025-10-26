use uefi::{proto::media::file::{Directory, File, FileAttribute, FileHandle, FileMode, FileType}, CStr16, Error};

extern crate alloc;

use alloc::vec::Vec;




pub fn open_file(file: &CStr16) -> Result<FileHandle,Error> {
    let mut sfs: Directory;
    let result = uefi::boot::get_image_file_system(uefi::boot::image_handle());
    match result {
        Ok(mut r) => {
            let h = r.open_volume();
            match h {
                Ok(h) => {
                    sfs = h;
                }
                Err(e) => return Err(e),
            }
        },
        Err(_) => panic!(),
    }
    let fileresult = sfs.open(file, FileMode::ReadWrite, FileAttribute::SYSTEM);
    match fileresult {
        Ok(f)=>  {
            return Ok(f);
        },
        Err(e) => {
            return Err(e);  
        },
    }
}



pub fn read_file(file: FileHandle) -> Result<Vec<u8>, &'static str> {
    let mut content = Vec::new();
    let filetype: FileType;
    let valid: bool;
    match file.into_type() {
        Ok(f) => {
            filetype = f;
        },
        Err(_) => {
            return Err("Invalid Type")
        }
    }
    match filetype {
        FileType::Regular(mut regular_file) => {
            let mut buf = [0u8;512];
            loop {
                let n: usize;
                let nres = regular_file.read(&mut buf);
                match nres {
                    Ok(nresponse) => {
                        n = nresponse;
                    },
                    Err(_) => {
                        valid = false;
                        break;
                    },
                }
                if n == 0 {
                    valid = true;
                    break;
                }
                content.extend_from_slice(&buf[..n]);
            }
            
        },
        FileType::Dir(_) => {
            valid = false;
        },
    }
    if valid == false {
        return Err("Invalid File");
    }
    return Ok(content);
}



pub fn check_if_file_exists(file: &CStr16) -> bool {
    match open_file(file) {
        Ok(f) => {
            f.close();     
            return true
        },
        Err(_) => return false
    }
}
