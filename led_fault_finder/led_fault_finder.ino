#include <OctoWS2811.h>

const int ledsPerStrip = 470;
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
  leds.begin();
  leds.show();
}

// Less intense...
#define REDGREEN    0x101000
#define BLUEGREEN   0x001010
#define BLUERED		0x100010

uint32_t COLOURS[3] = {BLUERED, BLUEGREEN, REDGREEN};

int fwd,bkwd,i = 0;

void loop() {
  i = 0;
  while(i < 3){
    setAllPixels(COLOURS[i]);
    delay(DELAY_MILLISECONDS);

    while(digitalRead(PIN_FORWARD)){
		//Wait for button to be released
		continue;
	}

    while(!digitalRead(PIN_FORWARD)){
	  // Wait for button to be pressed
	  continue;
    }
    i++;
  }
}

void setAllPixels(int color)
{
  for (int i=0; i < leds.numPixels(); i++) {
    leds.setPixel(i, color);
  }
  leds.show();
}
