#include <Adafruit_NeoPixel.h>

#define PIN 1
#define STRIPSIZE 48
#define TOUCH_PIN_A A9
#define TOUCH_PIN_B A5

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);

int TOUCH_BASE_A;
int TOUCH_BASE_B;
 
 void setup() {
  strip.begin();
  strip.setBrightness(250);  // Lower brightness and save eyeballs!
  strip.show(); // Initialize all pixels to 'off' 
  TOUCH_BASE_A = touchRead(TOUCH_PIN_A);
  TOUCH_BASE_B = touchRead(TOUCH_PIN_B);

  Serial.begin(9600);
Serial.println("Beginning Calibration");
 }
 
 void loop()                     
 {
    for(uint8_t i = 0; i < STRIPSIZE; i++){
      if(i > 0){
        strip.setPixelColor(i-1, strip.Color(0,0,0));
      }
      strip.setPixelColor(i, strip.Color(255,255,255));
      strip.show();
      while(not isTouchedA() and not isTouchedB()){
        delay(3);
        continue;
      }
      if(isTouchedA()){
          Serial.println("Pixel " + String(i) + ": top");
          strip.setPixelColor(i, strip.Color(0,255,0));
          strip.show();
          delay(1000);
        } else if(isTouchedB()) {
          Serial.println("Pixel " + String(i) + ": bottom");
          strip.setPixelColor(i, strip.Color(255,0,0));
          strip.show();
          delay(1000);
        }
    }
    Serial.println("All done!");
    colorWipe(strip.Color(64, 64, 64), 100);
 }

 void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      delay(wait);
  }
}

bool isTouchedA(){
    return touchRead(TOUCH_PIN_A) >= (1.5 * TOUCH_BASE_A);
}
bool isTouchedB(){
    return touchRead(TOUCH_PIN_B) >= (1.5 * TOUCH_BASE_B);
}
