#include <Adafruit_NeoPixel.h>

// LED Settings
#define TOPLEFTPIN 21
#define TOPRIGHTPIN 22
#define TOPSTRIPSIZE 39 

#define BOTTOMPIN 23
#define BOTTOMSTRIPSIZE 94

#define BOTTOMSIDELENGTH 42

#define BASE_BRIGHTNESS 150
#define BRIGHTNESS_INCREMENT 15

#define TEENSY_LED_PIN 13

// Touch settings
#define NUM_TOUCH_PINS 2
#define TOUCH_PIN_A 18
#define BTN_A 0
#define TOUCH_PIN_B 19
#define BTN_B 1
#define TOUCH_MULTIPLIER_THRESHOLD 1.5

#define DEBUG true

int touch_pins[NUM_TOUCH_PINS] = {TOUCH_PIN_A, TOUCH_PIN_B};
int touch_base[NUM_TOUCH_PINS];
bool is_touched[NUM_TOUCH_PINS];
bool was_touched[NUM_TOUCH_PINS];

// State variables
uint32_t sequence_no = 0;
uint8_t brightness = BASE_BRIGHTNESS;
bool suspended = false;
int mode_no = 0;
const int NUM_OF_MODES = 3;


bool teensy_led_state = true;



// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel tlstrip = Adafruit_NeoPixel(TOPSTRIPSIZE, TOPLEFTPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel trstrip = Adafruit_NeoPixel(TOPSTRIPSIZE, TOPRIGHTPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel bstrip = Adafruit_NeoPixel(BOTTOMSTRIPSIZE, BOTTOMPIN, NEO_GRB + NEO_KHZ800);
const uint32_t BLACK = bstrip.Color(0,0,0);
const uint32_t RED = bstrip.Color(255,0,0);
const uint32_t YELLOW = bstrip.Color(235,229,52);
const uint32_t GREEN = bstrip.Color(0,128,0);
const uint32_t BLUE = bstrip.Color(0,0,128);
const uint32_t PINK = bstrip.Color(122, 40, 140);
const uint32_t WHITE = bstrip.Color(255, 255, 255);

void setup() {
    //typedef void (* FP)(*Adafruit_NeoPixel, *Adafruit_NeoPixel)
    //FP MODES[NUM_OF_MODES] = {&rainbow, &colourWipeRedYellow, &rotatePaletteSunset};
    if(DEBUG == true){
        setupSerial();
    }
    setupLEDStrip(&tlstrip, BASE_BRIGHTNESS);
    setupLEDStrip(&trstrip, BASE_BRIGHTNESS);
    setupLEDStrip(&bstrip, BASE_BRIGHTNESS);
    setupTouch();
    if(DEBUG){
      pinMode(TEENSY_LED_PIN, OUTPUT);
      digitalWrite(TEENSY_LED_PIN, teensy_led_state);
    }

    
}

void loop() {
    // Update state
    updateIsTouched();
    updateTouchVariables();
    sequence_no++;
    debugPrint();
    if(suspended) {
        setAllPixels(&tlstrip, BLACK);
        setAllPixels(&trstrip, BLACK);
        setAllPixels(&bstrip, BLACK);
    } else {
      if(mode_no == 0){
        rainbow(&tlstrip, &trstrip, &bstrip);
      } else if(mode_no == 1){
        colourWipeRedYellow(&tlstrip, &trstrip, &bstrip);
      } else if(mode_no == 2){
        rotatePaletteSunset(&tlstrip, &trstrip, &bstrip);
      }
    }

    if(DEBUG){
      digitalWrite(TEENSY_LED_PIN, teensy_led_state);
      teensy_led_state = !teensy_led_state;
    }

    
    delay(10);
}




void updateTouchVariables() {
    // If both are pressed, suspend
    if(is_touched[BTN_A] & is_touched[BTN_B] & (!was_touched[BTN_A] | !was_touched[BTN_B])){
        debugPrint("Suspend");
        suspended = true;
        return;
    }
    // If BTN_A is pressed, change modes and unsuspend
    if(is_touched[BTN_A] & !was_touched[BTN_A] & !is_touched[BTN_B]){
        mode_no = (mode_no + 1) % NUM_OF_MODES;
        suspended = false;
        debugPrint("Next mode: " + mode_no);
    }
    // If BTN_B is pressed, increase the brightness and unsuspend
    if(is_touched[BTN_B] & !was_touched[BTN_B] & !is_touched[BTN_A]){
        brightness = (brightness + BRIGHTNESS_INCREMENT) % 255;
        debugPrint("Brightness up: " + brightness);
        tlstrip.setBrightness(brightness);
        trstrip.setBrightness(brightness);
        bstrip.setBrightness(brightness);
    }
}
