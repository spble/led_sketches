#include <OctoWS2811.h>

const int ledsPerStrip = 800;
const int totalLEDs = ledsPerStrip*8;


const int PIN_FORWARD = 23;
const int PIN_BACKWARD = 22;

const int DELAY_MILLISECONDS = 40;


DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;

OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

void setup() {
  pinMode(PIN_FORWARD, INPUT);
  pinMode(PIN_BACKWARD, INPUT);
  Serial.begin(9600);
  leds.begin();
  leds.show();
}

// Less intense...
#define RED    0x160000
#define GREEN  0x001600
#define BLUE   0x000016
#define YELLOW 0x101400
#define PINK   0x120009
#define ORANGE 0x100400
#define WHITE  0x101010

int fwd,bkwd,i = 0;

void loop() {
  setAllPixels(BLUE);
  i = 0;
  while(i < totalLEDs) {
    fwd = digitalRead(PIN_FORWARD);
    bkwd = digitalRead(PIN_BACKWARD);
  	while(fwd == 0 & bkwd == 0){
  		fwd = digitalRead(PIN_FORWARD);
  		bkwd = digitalRead(PIN_BACKWARD);
  	}
    if(bkwd == 1 & i > 0){
        Serial.print("Backward ");
        leds.setPixel(i, RED);
  			i--; 
        leds.setPixel(i, GREEN);
        leds.show();
        Serial.print("LED: ");
        Serial.print(i);
        Serial.println(" (GREEN)");
        while(bkwd == 1){
          delay(DELAY_MILLISECONDS*2);
          bkwd = digitalRead(PIN_BACKWARD);
        }
  	} else if(fwd == 1) {
        Serial.print("Forward  ");
        i++;
        leds.setPixel(i, GREEN);
        leds.show();
        Serial.print("LED: ");
        Serial.print(i);
        Serial.println(" (GREEN)");
        delay(DELAY_MILLISECONDS);
  	}
  }
}

void setAllPixels(int color)
{
  for (int i=0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
  }
  leds.show();
}
