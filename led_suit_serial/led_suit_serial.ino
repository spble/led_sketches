// Glediator example with OctoWS2811, by mortonkopf
//
// https://forum.pjrc.com/threads/33012-Gladiator-with-OctoWS2811-working-example

// You can also use Jinx to record Glediator format data to a SD card.
// To play the data from your SD card, use this modified program:
// https://forum.pjrc.com/threads/46229&viewfull=1#post153927

#include <OctoWS2811.h>

const int MAX_LEDS_PER_STRIP = 733;
const int NUM_LEDS = 2211;
const int NUM_STRIPS = 5;
const int LEDS_PER_STRIP[NUM_STRIPS] = {675, 346, 238, 733, 219};

DMAMEM int displayMemory[MAX_LEDS_PER_STRIP*6];
int drawingMemory[MAX_LEDS_PER_STRIP*6];

const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 leds(MAX_LEDS_PER_STRIP, displayMemory, drawingMemory, config);

byte r,g,b;
int i, j, start_led;

void setup() {
	Serial.begin(1000000);
	leds.begin();
	leds.show();
}

int serialGlediator() {
	while (!Serial.available()) {}
	return Serial.read();
}

void idlePattern(int g, int r) {
	return;
	leds.setPixel(j, Color(g,r,0));
	j++;
	leds.show();
}


void loop() {

  // 255 is the 'end of frame' command
  // 254 is the 'skip pixel' command

	for (j = 0; j < NUM_STRIPS; j++){
		start_led = j * MAX_LEDS_PER_STRIP;
		for (i=0; i < LEDS_PER_STRIP[j]; i++) {
			b = serialGlediator();
			if(b == 255) break;
			r = serialGlediator();
			if(r == 255) break;
			g = serialGlediator();
			if(g == 255) break;
			if(r == 254 && g == 254 && b == 254) continue;
			leds.setPixel(start_led + i, Color(r,g,b));
		}
		if(b == 255 | r == 255 | g == 255) break;
	}
	leds.show();
}

/* Helper functions */
// Create a 24 bit color value from R,G,B
unsigned int Color(byte r, byte g, byte b)
{
	return (((unsigned int)b << 16) | ((unsigned int)r << 8) | (unsigned int)g);
}
