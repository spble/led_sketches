#include <Adafruit_NeoPixel.h>

// LED Settings
#define TOPPIN 2
#define TOPSTRIPSIZE 300

#define BOTTOMPIN 1
#define BOTTOMSTRIPSIZE 300

#define BASE_BRIGHTNESS 50
#define BRIGHTNESS_INCREMENT 10

// How long we stay on a particular mode
#define MODE_LENGTH 10000

// Touch settings
#define NUM_TOUCH_PINS 3
#define TOUCH_PIN_A A3
#define END 0
#define TOUCH_PIN_B A4
#define DOWN 1
#define TOUCH_PIN_C A5
#define UP 2
#define TOUCH_MULTIPLIER_THRESHOLD 1.3

#define DEBUG true

#define EULER 2.71828

int touch_pins[NUM_TOUCH_PINS] = {TOUCH_PIN_A, TOUCH_PIN_B, TOUCH_PIN_C};
int touch_base[NUM_TOUCH_PINS];
bool is_touched[NUM_TOUCH_PINS];
bool was_touched[NUM_TOUCH_PINS];
bool autoscroll = true;

// State variables
uint32_t sequence_no = 0;
uint8_t brightness = BASE_BRIGHTNESS;
bool suspended = false;
int mode_no = 0;
const int NUM_OF_MODES = 4;
void (*MODES[NUM_OF_MODES])(Adafruit_NeoPixel *tstrip, Adafruit_NeoPixel *bstrip);


const int NUM_TWINKLES = 60;
struct Twinkle {
    unsigned int start_no = 0;
    unsigned int period = 0;
    unsigned int length = 0;
    unsigned int position = 0;
    uint32_t colour;
};
Twinkle twinkles[NUM_TWINKLES];



// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel tstrip = Adafruit_NeoPixel(TOPSTRIPSIZE, TOPPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel bstrip = Adafruit_NeoPixel(BOTTOMSTRIPSIZE, BOTTOMPIN, NEO_GRB + NEO_KHZ800);

const uint32_t BLACK = tstrip.Color(0,0,0);
const uint32_t RED = tstrip.Color(255,0,0);
const uint32_t YELLOW = tstrip.Color(235,229,52);
const uint32_t GREEN = tstrip.Color(0,128,0);
const uint32_t BLUE = tstrip.Color(0,0,128);
const uint32_t PINK = tstrip.Color(122, 40, 140);
const uint32_t WHITE = tstrip.Color(255, 255, 255);

void setup() {
    MODES[0] = rotatePaletteSunset;
    MODES[1] = colourWipeRedYellow;
    MODES[2] = colourWipeWhiteBlackRed;
    MODES[3] = rainbow;
    for(unsigned int i; i < NUM_TWINKLES; i++){
        twinkles[i] = initialise_twinkle();
    }
    //typedef void (* FP)(*Adafruit_NeoPixel, *Adafruit_NeoPixel)
    //FP MODES[NUM_OF_MODES] = {&rainbow, &colourWipeRedYellow, &rotatePaletteSunset};
    if(DEBUG){
        setupSerial();
    }
    setupLEDStrip(&tstrip, BASE_BRIGHTNESS);
    setupLEDStrip(&bstrip, BASE_BRIGHTNESS);
    setupTouch();
}

void loop() {
    // Update state
    updateIsTouched();
    updateTouchVariables();
    sequence_no++;
    if(sequence_no % 50 == 0) {
        debugPrint();
    }
    if(suspended) {
        setAllPixels(&tstrip, BLACK);
        setAllPixels(&bstrip, BLACK);
    } else {
        if(autoscroll){
            mode_no = (int)floor(sequence_no / MODE_LENGTH) % NUM_OF_MODES;
        }
        MODES[mode_no](&tstrip, &bstrip);
    }
    delay(1);
}




void updateTouchVariables() {
    // If all three are pressed, suspend
    if(is_touched[UP] & is_touched[DOWN] & is_touched[END] & (!was_touched[UP] | !was_touched[DOWN] | !was_touched[END])){
        debugPrint("Suspend");
        suspended = true;
        return;
    }
    // If mode-UP is pressed, increase the mode and unsuspend
    if(is_touched[UP] & !was_touched[UP] & !is_touched[DOWN] & !is_touched[END]){
        mode_no = (mode_no + 1) % NUM_OF_MODES;
        suspended = false;
        autoscroll = false;
        debugPrint("Next mode: " + mode_no);
        return;
    }
    // If mode-DOWN and mode-UP are both pressed, enable autoscroll
    if(is_touched[DOWN] & is_touched[UP] & (!was_touched[DOWN] | !was_touched[UP]) & !is_touched[END]){
        autoscroll = true;
        debugPrint("Autoscroll enabled: " + mode_no);
        return;
    }
    // If mode-DOWN is pressed, increase the mode and unsuspend
    if(is_touched[DOWN] & !was_touched[DOWN] & !is_touched[UP] & !is_touched[END]){
        if(mode_no == 0)
            mode_no = NUM_OF_MODES - 1;
        else
            mode_no--;
        suspended = false;
        autoscroll = false;
        debugPrint("Prev mode: " + mode_no);
        return;
    }
    // If touching end, and up, increase brightness.
    if(is_touched[END] & is_touched[UP] & !was_touched[UP] & !is_touched[DOWN]){
        brightness = min(255, brightness + BRIGHTNESS_INCREMENT);
        debugPrint("Brightness up: " + brightness);
        tstrip.setBrightness(brightness);
        bstrip.setBrightness(brightness);
        return;
    }
    // If touching end, and down, decrease brightness.
    if(is_touched[END] & is_touched[DOWN] & !was_touched[DOWN] & !is_touched[UP]){
        brightness = max(1, brightness - BRIGHTNESS_INCREMENT);
        debugPrint("Brightness down: " + brightness);
        tstrip.setBrightness(brightness);
        bstrip.setBrightness(brightness);
        return;
    }
}
