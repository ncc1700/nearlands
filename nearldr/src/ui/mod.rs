use uefi::{proto::console::gop::GraphicsOutput};

use crate::graphics::GraphicsBuffer;





pub struct Ui {
    graphics: GraphicsBuffer,
}





impl Ui {
    pub fn new(graphics: GraphicsBuffer) -> Self {
        return Ui { graphics: graphics };
    }

    pub fn render_ui(&mut self, gop: &mut GraphicsOutput){
        let headertext = "Nearlands Boot Manager";
        let entertext = "ENTER=Choose";
        let optiontext: &str = "Choose the option you want to boot into";
        let advancedtext: &str = "To display advanced options, press F6";

        self.graphics.draw_rectangle(0, 0, self.graphics.width, 30, (155, 155, 155));
        self.graphics.draw_text(headertext, (self.graphics.width - self.graphics.measure_text(headertext, 1)) / 2, 15, 1,(0, 0, 0));
        self.graphics.draw_rectangle(0, self.graphics.height - 30, self.graphics.width, 30, (155, 155, 155));
        self.graphics.draw_text(entertext, 10, self.graphics.height - 19, 1, (0, 0, 0));

        self.graphics.draw_text(optiontext, 20, 90, 1, (255, 255, 255));

        self.graphics.draw_text(advancedtext, 20, self.graphics.height - 60, 1, (255, 255, 255));

        self.graphics.blit(gop).expect("Couldn't draw Into buffer");

    }

    pub fn clear_screen(&mut self, gop: &mut GraphicsOutput){
        for x in 0..self.graphics.width {
            for y in 0..self.graphics.height {
                let pixel = self.graphics.get_pixel(x, y).expect("Invalid Pixel");
                pixel.red = 0;
                pixel.green = 0;
                pixel.blue = 0;
            }
        }
        self.graphics.blit(gop).expect("Couldn't draw Into buffer");
    }

}