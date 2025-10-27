use uefi::proto::console::gop::{BltOp, BltPixel, BltRegion, GraphicsOutput};
extern crate alloc;
use alloc::vec;
use alloc::vec::Vec;
use uefi::Error;

mod fonts;

pub struct GraphicsBuffer {
    pub width: usize,
    pub height: usize,
    pbuffer: Vec<BltPixel>,
}

impl GraphicsBuffer {
    pub fn new(width: usize, height: usize) -> Self {
        return GraphicsBuffer {
            width: width,
            height: height,
            pbuffer: vec![BltPixel::new(0, 0, 0); width * height],
        };
    }

    pub fn get_pixel(&mut self, x: usize, y: usize) -> Option<&mut BltPixel> {
        return self.pbuffer.get_mut(y * self.width + x);
    }
    pub fn blit(&self, gop: &mut GraphicsOutput) -> Result<(), Error> {
        gop.blt(BltOp::BufferToVideo {
            buffer: &self.pbuffer,
            src: BltRegion::Full,
            dest: (0, 0),
            dims: (self.width, self.height),
        })
    }
    pub fn draw_rectangle(&mut self, x: usize, y: usize, sizex: usize, sizey: usize, color: (u8, u8, u8)) {
        let beginx = x;
        let beginy = y;
        let endx = x + sizex;

        let endy = y + sizey;
        
        for numx in beginx..endx {
            for numy in beginy..endy {
                let pixel = self.get_pixel(numx, numy).expect("Invalid Pixel");
                pixel.red = color.0;
                pixel.green = color.1;
                pixel.blue = color.2;
            }
        }
    }
    pub fn _draw_rectangle_and_blit(&mut self, x: usize, y: usize, sizex: usize, sizey: usize, gop: &mut GraphicsOutput, color: (u8, u8, u8)) -> Result<(), Error> {
        self.draw_rectangle(x, y, sizex, sizey, color);
        self.blit(gop)
    }
    pub fn draw_character(&mut self, c: char, x: usize, y: usize, size: usize, color: (u8, u8, u8)) -> Result<(), &'static str> {
        if ((c as usize) < 32) || ((c as usize) > 126) {
            return Err("Invalid Size");
        }

        let cu8 = (c as usize - 32) as usize;
        let bitmap = &fonts::FONT8X8[cu8];

        for i in 0..8 {
            for j in 0..8 {
                if (bitmap[i] & (1 << j)) != 0 {
                    self.draw_rectangle(x + j * size, y + i * size, size, size, color);
                }
            }
        }
        Ok(())
    }
    pub fn _draw_character_and_blit(&mut self, c: char, x: usize, y: usize, size: usize, gop: &mut GraphicsOutput, color: (u8, u8, u8)) -> Result<(), &'static str> {
        self.draw_character(c, x, y, size, color).expect("Couldn't Draw Character");
        self.blit(gop).expect("Couldn't draw Into buffer");
        Ok(())
    }

    pub fn draw_text(&mut self, text: &str, x: usize, y: usize, size: usize, color: (u8, u8, u8)) {
        let mut xadd: usize = 0;
        for c in text.chars() {
            self.draw_character(c, x + ((xadd * size) * 8), y, size, color).expect("Couldn't Draw Character");
            xadd+=1;
        }
    }

    pub fn _draw_text_and_blit(&mut self, text: &str, x: usize, y: usize, size: usize, gop: &mut GraphicsOutput, color: (u8, u8, u8)) {
        self.draw_text(text, x, y, size, color);
        self.blit(gop).expect("Couldn't draw Into buffer");
    }

    pub fn measure_text(&self, text: &str, size: usize) -> usize {
        return text.chars().count() * 8 * size ;
    }
}
