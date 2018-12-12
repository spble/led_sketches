#include <Adafruit_NeoPixel.h>

#define TOPPIN 2
#define TOPSTRIPSIZE 95 

#define BOTTOMPIN 1
#define BOTTOMSTRIPSIZE 94

#define SIDELENGTH 42

#define BASE_BRIGHTNESS 150

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel tstrip = Adafruit_NeoPixel(TOPSTRIPSIZE, TOPPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel bstrip = Adafruit_NeoPixel(BOTTOMSTRIPSIZE, BOTTOMPIN, NEO_GRB + NEO_KHZ800);


void setup() {
  tstrip.begin();
  tstrip.setBrightness(BASE_BRIGHTNESS);  // Lower brightness and save eyeballs!
  tstrip.show(); // Initialize all pixels to 'off'
  bstrip.begin();
  bstrip.setBrightness(BASE_BRIGHTNESS
  );  // Lower brightness and save eyeballs!
  bstrip.show(); // Initialize all pixels to 'off'
  
}

void loop() {
  colourWipeBoth(&tstrip, &bstrip, tstrip.Color(0,0,0), 5);
  colourWipeBoth(&tstrip, &bstrip, tstrip.Color(0,0,128), 10);
  delay(255);
  colourWipeBoth(&tstrip, &bstrip, tstrip.Color(0,128,0), 10);
  delay(255);
  colourWipeBoth(&tstrip, &bstrip, tstrip.Color(128,0,0), 10);
  delay(255);
  pulseBackBoth(&tstrip, &bstrip, tstrip.Color(122, 40, 140), 1);
  delay(255);
  rainbow(15);
}


void colourWipeBoth(Adafruit_NeoPixel *s1, Adafruit_NeoPixel *s2, uint32_t colour, uint8_t wait){
  uint16_t pixel_1 = ceil(s1->numPixels()/2);
  uint16_t pixel_2 = ceil(s2->numPixels()/2);
  uint16_t limit = max(pixel_1, pixel_2);
  
  for(uint16_t i=0; i <= limit; i++) {
      s1->setPixelColor(pixel_1 + i, colour);
      s1->setPixelColor(pixel_1 - i, colour);
      s2->setPixelColor(pixel_2 + i, colour);
      s2->setPixelColor(pixel_2 - i, colour);
      s1->show();
      s2->show();
      delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<tstrip.numPixels(); i++) {
      tstrip.setPixelColor(i, Wheel((i+j) & 255));
    }
    
    for(i=0; i<bstrip.numPixels(); i++) {
      bstrip.setPixelColor(i, Wheel((i+j) & 255));
    }
    tstrip.show();
    bstrip.show();
    delay(wait);
  }
}

void pulseBackBoth(Adafruit_NeoPixel *s1, Adafruit_NeoPixel *s2, uint32_t colour, uint8_t wait){
  
  uint8_t PULSE_LENGTH = 3;
  uint8_t mid_length_1 = s1->numPixels() - 2*SIDELENGTH;
  uint8_t mid_length_2 = s2->numPixels() - 2*SIDELENGTH;

  // Build front
  for(uint8_t i=0; i <= 254; i++){
    for(uint8_t j=SIDELENGTH; j < SIDELENGTH + mid_length_1; j++){
      s1->setPixelColor(j, colour);
      s1->setBrightness(i);
    }
    for(uint8_t k=SIDELENGTH; k < SIDELENGTH + mid_length_2; k++){
      s2->setPixelColor(k, colour);
      s2->setBrightness(i);
    }
    s1->show();
    s2->show();
    
    delay(wait);
  }
  for(uint8_t j=SIDELENGTH; j < SIDELENGTH + mid_length_1; j++){
    s1->setPixelColor(j, s1->Color(0,0,0));
  }
  for(uint8_t k=SIDELENGTH; k < SIDELENGTH + mid_length_2; k++){
    s2->setPixelColor(k, s1->Color(0,0,0));
  }  
 
 
  colourWipeBoth(s1, s2, colour, wait);

  
  s1->setBrightness(BASE_BRIGHTNESS);
  s2->setBrightness(BASE_BRIGHTNESS);

}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return tstrip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return tstrip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return tstrip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
