#include <Adafruit_NeoPixel.h>

#define SERIAL_OUT true


// Touch Constants
#define NUM_TOUCH_PINS 4
#define TOUCH_PIN_A A9
#define TOUCH_PIN_B A5
#define TOUCH_PIN_C A4
#define TOUCH_PIN_D A3
#define TOUCH_MULTIPLIER_THRESHOLD 1.5


// Accel. Constants
#define AXIS_X A2
#define AXIS_Y A1
#define AXIS_Z A0
#define AXIS_HISTORICAL_COUNT 100
#define AXIS_ACTIVITY_POINTS 10.0
#define HIGH_ACTIVITY_THRESHOLD 6


// LED Constants
#define LED_PIN 1
#define STRIP_SIZE 420 
#define BRIGHTNESS_LOW 15
#define BRIGHTNESS_MED 70
#define BRIGHTNESS_HIGH 255
#define MAX_TWINKLES 50


Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIP_SIZE, LED_PIN, NEO_GRB + NEO_KHZ800);

// Touch Variables
int touch_pins[NUM_TOUCH_PINS] = {TOUCH_PIN_A, TOUCH_PIN_B, TOUCH_PIN_C, TOUCH_PIN_D};
int touch_base[NUM_TOUCH_PINS];
bool is_touched[NUM_TOUCH_PINS];

// Accel. Variables
int16_t rest_xyz_vals[3] = {0};
int16_t axis_vals[AXIS_HISTORICAL_COUNT][3] = {{0}};
int16_t axis_diffs[AXIS_HISTORICAL_COUNT][3] = {{0}};
double axis_activity_max[3] = {0};
uint8_t axis_activity[3] = {0};
uint8_t total_activity_metric = 0;

// LED Variables
int16_t twinkles[MAX_TWINKLES] = {-1};
const uint32_t BLACK = strip.Color(0,0,0);
const uint32_t RED = strip.Color(128,0,0);
const uint32_t GREEN = strip.Color(0,128,0);
const uint32_t BLUE = strip.Color(0,0,128);
const uint32_t PINK = strip.Color(122, 40, 140);
const uint32_t WHITE = strip.Color(255, 255, 255);


// This governs the whole pattern
uint32_t sequence_no = 0;


void setup() {
	setupTouch();
	setupLEDStrip(BRIGHTNESS_LOW);
	setupAxis();
	if(SERIAL_OUT) setupSerial();
}


void loop(){
	rotatePaletteSunset(7);
    twinkle(2);
    strip.show();
	sequence_no++;
}

