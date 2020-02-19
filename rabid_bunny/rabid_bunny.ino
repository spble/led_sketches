#include <Adafruit_NeoPixel.h>

#define SERIAL_OUT true


// Touch Constants
#define NUM_TOUCH_PINS 6
#define TOUCH_PIN_A 1
#define TOUCH_PIN_B A5
#define TOUCH_PIN_C A4
#define TOUCH_PIN_D A3
#define TOUCH_PIN_E A2
#define TOUCH_PIN_F A1
#define TOUCH_MULTIPLIER_THRESHOLD 1.15


// Accel. Constants
#define AXIS_X A8
#define AXIS_Y A7
#define AXIS_Z A6
#define AXIS_HISTORICAL_COUNT 100
#define AXIS_ACTIVITY_POINTS 10.0
#define HIGH_ACTIVITY_THRESHOLD 6


// LED Constants
#define LED_PIN 13
#define STRIP_SIZE 257
#define BRIGHTNESS_LOW 15
#define BRIGHTNESS_MED 40
#define BRIGHTNESS_HIGH 255
#define MAX_TWINKLES 50

#define SEC_SPIRAL_END 220
#define SEC_BACK_END 230
#define SEC_LEFT_EYE 234
#define SEC_LEFT_EAR_END 237
#define SEC_RIGHT_EAR_END 240
#define SEC_RIGHT_EYE 241
#define SEC_NOSE_RIGHT 242
#define SEC_NOSE_LEFT 243
#define SEC_MOUTH_TOP_LEFT_END 247
#define SEC_MOUTH_BOTTOM_LEFT_END 250
#define SEC_MOUTH_BOTTOM_RIGHT_END 253
#define SEC_MOUTH_TOP_RIGHT_END 257



Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_SIZE, LED_PIN, NEO_GRB + NEO_KHZ800);

// Touch Variables
int touch_pins[NUM_TOUCH_PINS] = {TOUCH_PIN_A, TOUCH_PIN_B, TOUCH_PIN_C, TOUCH_PIN_D, TOUCH_PIN_E, TOUCH_PIN_F};
int touch_base[NUM_TOUCH_PINS];
bool is_touched[NUM_TOUCH_PINS];
bool was_touched[NUM_TOUCH_PINS];

// Accel. Variables
int16_t rest_xyz_vals[3] = {0};
int16_t axis_vals[AXIS_HISTORICAL_COUNT][3] = {{0}};
int16_t axis_diffs[AXIS_HISTORICAL_COUNT][3] = {{0}};
double axis_activity_max[3] = {0};
uint8_t axis_activity[3] = {0};
uint8_t total_activity_metric = 0;

// LED Variables
int16_t twinkles[MAX_TWINKLES] = { -1};
const uint32_t BLACK = strip.Color(0, 0, 0);
const uint32_t RED = strip.Color(128, 0, 0);
const uint32_t GREEN = strip.Color(0, 128, 0);
const uint32_t BLUE = strip.Color(0, 0, 128);
const uint32_t PINK = strip.Color(122, 40, 140);
const uint32_t WHITE = strip.Color(255, 255, 255);


// State variables
uint32_t sequence_no = 0;
bool suspended = false;
int mode_no = 0;
const int max_modes = 4;
uint8_t current_brightness = BRIGHTNESS_MED;



void setup() {
  setupTouch();
  setupLEDStrip(current_brightness);
  setupAxis();
  if (SERIAL_OUT) setupSerial();
}


void loop() {
  // Update state
  updateAxis();
  updateIsTouched();
  updateTouchVariables();
  debugPrint();


  // We're in a suspended state, just set to black
  if (suspended) {
    debugPrint("Going Black");
    setAllPixels(BLACK);
  } else {
    if (mode_no == 0) {
      rotatePaletteSunset(7);
    } else if (mode_no == 1) {
      rainbowRotate(7);
    } else if (mode_no == 2) {
      setAllPixels(colourFromAxis());
    } else if (mode_no == 3) {
      dragonBomb();
    }


    sequence_no++;
  }
  strip.show();
}

void updateTouchVariables() {
  if (is_touched[0]) {
    // The 'shift' button ('A') has been pressed. Do brightness and stuff.
    if (is_touched[3] & !was_touched[3]) {
      debugPrint("Toggle Suspend");
      suspended = !suspended;
    }
    if (is_touched[1] & !was_touched[1]) {
      debugPrint("Brightness down");
    }
  } else {
	  if (is_touched[3] & !was_touched[3]) {
		debugPrint("Next mode");
		mode_no = (mode_no + 1) % max_modes;
		debugPrint(String(mode_no));
	  }
	  if (is_touched[2] & !was_touched[2]) {
		debugPrint("Mode reset");
		if(mode_no == 2) {
		  setupAxis();
		} else {
		  sequence_no = 0;
		}
	  }
  }
  
}
