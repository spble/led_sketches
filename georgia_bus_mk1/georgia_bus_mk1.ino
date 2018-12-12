#include <Adafruit_NeoPixel.h>

#define LEDPIN 1
#define STRIPSIZE 600


#define LOW_BRIGHTNESS 15
#define MID_BRIGHTNESS 60
#define HIGH_BRIGHTNESS 150 




// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, LEDPIN, NEO_GRB + NEO_KHZ800);
  const uint32_t BLACK = strip.Color(0,0,0);
  const uint32_t RED = strip.Color(128,0,0);
  const uint32_t GREEN = strip.Color(0,128,0);
  const uint32_t BLUE = strip.Color(0,0,128);
  const uint32_t PINK = strip.Color(122, 40, 140);
  uint32_t colour;

void setup() {
  strip.begin();
  strip.setBrightness(MID_BRIGHTNESS);  // Lower brightness and save eyeballs!
  strip.show(); // Initialize all pixels to 'off'

}


void loop() {
  for(unsigned int i = 0; i < 510; i++){
    for(unsigned int l = 0; l < STRIPSIZE; l++){
      colour = PaletteGeorgia(i+l);
      strip.setPixelColor(l, colour);
    }
    strip.show();
    delay(10);
  }
}


uint32_t PaletteSunset(unsigned int i) {
  if (i > 1019) return PaletteSunset(i % 1020);
  if (i > 764) return strip.Color((i % 255), 0, 255 - (i % 255));          //blue -> red
  if (i > 509) return strip.Color(255 - (i % 255), 0, 255);                //purple -> blue
  if (i > 255) return strip.Color(255, 128 - (i % 255) / 2, (i % 255));    //orange -> purple
  return strip.Color(255, i / 2, 0);                                       //red -> orange
}

uint32_t PaletteGeorgia(unsigned int i) {
  if (i > 509) return PaletteGeorgia(i % 510);
  if (i > 255) return strip.Color(175, 198 - (i % 198), 255 - (i % 255));
  return strip.Color(255, 175 - (i % 175), 175);
}


uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

uint32_t USA(unsigned int i) {
  if (i > 764) return USA(i % 765);
  if (i > 509) return strip.Color(0, 0, 255);      //blue
  if (i > 255) return strip.Color(128, 128, 128);  //white
  return strip.Color(255, 0, 0);                   //red
}

uint32_t NoGreen(unsigned int i) {
  if (i > 1274) return NoGreen(i % 1275);
  if (i > 1019) return strip.Color(255, 0, 255 - (i % 255));         //violet -> red
  if (i > 764) return strip.Color((i % 255), 0, 255);                //blue -> violet
  if (i > 509) return strip.Color(0, 255 - (i % 255), 255);          //aqua -> blue
  if (i > 255) return strip.Color(255 - (i % 255), 255, i % 255);    //yellow -> aqua
  return strip.Color(255, i, 0);                                     //red -> yellow
}

uint32_t Ocean(unsigned int i) {
  if (i > 764) return Ocean(i % 765);
  if (i > 509) return strip.Color(0, i % 255, 255 - (i % 255));  //blue -> green
  if (i > 255) return strip.Color(0, 255 - (i % 255), 255);      //aqua -> blue
  return strip.Color(0, 255, i);                                 //green -> aqua
}


