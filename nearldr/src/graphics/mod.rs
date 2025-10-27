use log::info;
use uefi::proto::console::gop::{BltOp, BltPixel, BltRegion, GraphicsOutput};
extern crate alloc;
use alloc::vec;
use alloc::vec::Vec;
use uefi::Error;

mod fonts;

pub struct GraphicsBuffer {
    width: usize,
    height: usize,
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
    pub fn draw_pixel(&self, x: usize, y: usize, gop: &mut GraphicsOutput) -> Result<(), Error> {
        gop.blt(BltOp::BufferToVideo {
            buffer: &self.pbuffer,
            src: BltRegion::SubRectangle {
                coords: (x, y),
                px_stride: self.width,
            },
            dest: (x, y),
            dims: (1, 1),
        });
        Ok(())
    }
    pub fn blit(&self, gop: &mut GraphicsOutput) -> Result<(), Error> {
        gop.blt(BltOp::BufferToVideo {
            buffer: &self.pbuffer,
            src: BltRegion::Full,
            dest: (0, 0),
            dims: (self.width, self.height),
        })
    }
    pub fn draw_rectangle(&mut self, x: usize, y: usize, sizex: usize, sizey: usize, gop: &mut GraphicsOutput) {
        let beginx = x;
        let beginy = y;
        let endx = x + sizex;
        let endy = x + sizey;
        for numx in beginx..endx {
            for numy in beginy..endy {
                let pixel = self.get_pixel(numx, numy).expect("Invalid Pixel");
                pixel.red = 255;
                pixel.green = 255;
                pixel.blue = 255;
            }
        }
        self.blit(gop);
    }
    pub fn draw_text(&mut self, c: char, x: usize, y: usize, size: usize, gop: &mut GraphicsOutput) -> Result<(), &'static str> {
        // if (c < 32 || c > 126) {
        //     return;
        // }
        // const uint8_t *bitmap = font8x8_basic[c - 32]; 
        // for (int i = 0; i < 8; i++) {
        //     for (int j = 0; j < 6; j++) {
        //         if (bitmap[i] & (1 << j)) { 
        //             draw_rect(display, x + j * size, y + i * size, size, size, color);
        //         }
        //     }
        // }
        if ((c as usize) < 32) || ((c as usize) > 126) {
            return Ok(());
        }

        let cu8 = (c as usize - 32) as usize;
        let bitmap = &fonts::FONT8X8[cu8];

        for i in 0..8 {
            for j in 0..6 {
                if bitmap[i] & (1 << j) != 0 {
                    self.draw_rectangle(x + j * size, y + i * size, size, size, gop);
                }
            }
        }
        Ok(())
    }
}
