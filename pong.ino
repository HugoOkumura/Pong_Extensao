#include <TVout.h>
#include <video_gen.h>
#include <fontALL.h>

TVout TV;
int x = 0;
//const char buff = "AAA";
//const char a = "a";

void setup() {
    // TV.begin(_PAL, 640, 360);
//     TV.begin(_NTSC,128,72);
   TV.begin(NTSC);
//    TV.begin(_PAL,128,72);
    // TV.begin(_PAL);
    // Serial.begin(9600);
}

void loop() {
  // TV.select_font(font8x8);
  // Serial.println(TV.hres());
  // Serial.printlnS(TV.vres());
  // TV.fill(WHITE);
  // TV.draw_circle(2, TV.vres()/2, 2, WHITE, WHITE);

//  for(int i = 2; i < TV.hres()-4; i+=2){
//    TV.draw_circle(i, TV.vres()/2, 2, WHITE,WHITE);
//    TV.select_font(font4x6);
//    TV.print(0,0,i);
//    delay(50);
//    TV.clear_screen();
//  }
//  for(int j = TV.hres()-4; j > 2; j-=2){
//    TV.draw_circle(j, TV.vres()/2, 2, WHITE,WHITE);
//    TV.print(0,0,j);
//    delay(50);
//    TV.clear_screen();
//  }
  TV.draw_rect(0, 6, 122, 88, WHITE);

  x = analogRead(A0);
  TV.select_font(font4x6);
  // TV.print(8,0,x);
  x = map(x, 0, 1023, 20, 80);

  // TV.print(8,6,x);
  TV.print(8,0,x);
  TV.draw_line(5, x-12, 5, x+12, WHITE);
  // TV.set_pixel(x, TV.vres()/2 - 10, WHITE);
  // TV.set_pixel(5, x, WHITE);

  x = analogRead(A1);
  TV.select_font(font4x6);
  // TV.println(108,0,x);
  // x = map(x, 0, 1023, 12, 110);
  x = map(x, 0, 1023, 20, 80);
  // TV.println(108,6,x);
  TV.println(108,0,x);
  // TV.set_pixel(x, TV.vres()/2 + 10, WHITE);
  // TV.set_pixel(117, x, WHITE);
  TV.draw_line(117, x-12, 117, x+12, WHITE);

  TV.print(4,72,"Resolucao: ");
  x = TV.hres();
  TV.print(x);
  TV.print("x");
  x = TV.vres();
  TV.print(x);
  TV.delay_frame(1);
  TV.clear_screen();
}

/*
/* Draw a line from one point to another
 *
 * Arguments:
 *	x0:
 *		The x coordinate of point 0.
 *	y0:
 *		The y coordinate of point 0.
 *	x1:
 *		The x coordinate of point 1.
 *	y1:
 *		The y coordinate of point 1.
 *	c:
 *		The color of the line.
 *		(see color note at the top of this file)
 
 Patched to allow support for the Arduino Leonardo 
void TVout::draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, char c) {


*/