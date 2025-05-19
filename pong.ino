#include <TVout.h>
#include <video_gen.h>
#include <fontALL.h>

TVout TV;
unsigned char x,y;
const char buff = "AAA";
const char a = "a";

void setup() {
    // TV.begin(_PAL, 640, 360);
    // TV.begin(_NTSC,128,72);
    TV.begin(_PAL,128,72);
    // TV.begin(_PAL);
    // Serial.begin(9600);
}

void loop() {
  TV.clear_screen();
  // TV.select_font(font8x8);
  // Serial.println(TV.hres());
  // Serial.printlnS(TV.vres());
  // TV.fill(WHITE);
  // TV.draw_circle(2, TV.vres()/2, 2, WHITE, WHITE);

  for(int i = 2; i < TV.hres()-4; i+=2){
    TV.draw_circle(i, TV.vres()/2, 2, WHITE,WHITE);
    delay(50);
    TV.clear_screen();
  }
  for(int j = TV.hres()-4; j > 2; j-=2){
    TV.draw_circle(j, TV.vres()/2, 2, WHITE,WHITE);
    delay(50);
    TV.clear_screen();
  }

  delay(100);
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
