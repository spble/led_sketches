#include <Adafruit_NeoPixel.h>


#define AXIS_X A2
#define AXIS_Y A1
#define AXIS_Z A0

#define PIN 1
#define STRIPSIZE 48
#define TOUCH_PIN A9

int rest_x, rest_y, rest_z, TOUCH_BASE;
uint16_t red, green, blue, x, y, z;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  // put your setup code here, to run once:
  strip.begin();
  strip.setBrightness(250);  // Lower brightness and save eyeballs!
  strip.show(); // Initialize all pixels to 'off'
  Serial.begin(9600);
  Serial.println("Started!");
  resetAxis();
  TOUCH_BASE = touchRead(TOUCH_PIN);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("X: " + String(analogRead(AXIS_X) - rest_x) + ", Y: " + String(analogRead(AXIS_Y) - rest_y) + ", Z: " + String(analogRead(AXIS_Z) - rest_z));
  red = int(pow(floor(double(analogRead(AXIS_X) - rest_x)/6.0),2));
  green = int(pow(floor(double(analogRead(AXIS_Y) - rest_y)/6.0),2));
  blue = int(pow(floor(double(analogRead(AXIS_Z) - rest_z)/6.0),2));
    for(uint8_t j = 0; j < STRIPSIZE; j++){
      strip.setPixelColor(j, strip.Color(red, green, blue));
    }
    strip.show();
  Serial.println("R: " + String(red) + ", G: " + String(green)  + ", B: " + String(blue));
  while(isTouched()){
    resetAxis();
    for(uint8_t j = 0; j < STRIPSIZE; j++){
      strip.setPixelColor(j, strip.Color(255, 0, 0));
    }
    strip.show();
    delay(50);
  }
  delay(50);
}

void resetAxis() {
  rest_x = analogRead(AXIS_X);
  rest_y = analogRead(AXIS_Y);
  rest_z = analogRead(AXIS_Z);
}

bool isTouched(){
    return touchRead(TOUCH_PIN) >= (1.5 * TOUCH_BASE);
}


