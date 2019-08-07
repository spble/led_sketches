// Glediator example with OctoWS2811, by mortonkopf
//
// https://forum.pjrc.com/threads/33012-Gladiator-with-OctoWS2811-working-example

// You can also use Jinx to record Glediator format data to a SD card.
// To play the data from your SD card, use this modified program:
// https://forum.pjrc.com/threads/46229&viewfull=1#post153927

#include <OctoWS2811.h>


const int ledsPerStrip = 468;
const int NUM_LEDS = 3744;
const int NUM_STRIPS = 8;

const int STRIP_LENGTHS[3] = {138, 151, 179};

const int IDLE_TIMEOUT = 50000000;
// This mods the sequence, so that a lower number is faster.
const int IDLE_PATTERN_SPEED = 1;
// Divides the colour, lower numbers is brighter
const int IDLE_BRIGHTNESS = 19;
const unsigned int BRIGHTNESS = 15;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GBR | WS2811_800kHz;
OctoWS2811 leds(ledsPerStrip, displayMemory, drawingMemory, config);

byte r,g,b;
int i;
int w = 0;
bool is_idle = false;

void setup() {
  Serial.begin(500000);
  leds.begin();
  leds.show();

}

int serialGlediator() {
  w = 0;
  while (!Serial.available()) {
    if(w > IDLE_TIMEOUT) {
      is_idle = true;
      return 255;
    }
    w++;
  }
  w = 0;
  is_idle = false;
  return Serial.read();
}



void loop() {
  
  // 255 is the 'end of frame' command
  // 254 is the 'skip pixel' command

  for (i=0; i < NUM_LEDS; i++) {
    b = serialGlediator();
    if(b == 255) break;
    r = serialGlediator();
    if(r == 255) break;
    g = serialGlediator();
    if(g == 255) break;
    if(r == 254 && g == 254 && b == 254) continue;
    leds.setPixel(i, Color(r,g,b));
  }
  leds.show();
  if(is_idle){
    idlePattern();
  }
}

/* Helper functions */
// Create a 24 bit color value from R,G,B
// IdleColor includes brightness adjustment, for the idle pattern.
unsigned int IdleColor(byte r, byte g, byte b)
{
  unsigned int r_val = ((unsigned int)r * BRIGHTNESS) >> 8;
  unsigned int g_val = ((unsigned int)g * BRIGHTNESS) >> 8;
  unsigned int b_val = ((unsigned int)b * BRIGHTNESS) >> 8;
  return ((b_val << 16) | (r_val << 8) | g_val);
}
unsigned int Color(byte r, byte g, byte b)
{
  return (((unsigned int)b << 16) | ((unsigned int)r << 8) | (unsigned int)g);
}


// Keeps doing sunset until serial becomes available again.
void idlePattern() {
    int seq_no = 0;
    while(!Serial.available()) {
      rotatePaletteSunset(seq_no);
      seq_no++;
    }
    is_idle = false;
    w = 0;
}

void rotatePaletteSunset(int sequence_no){
  if(sequence_no % IDLE_PATTERN_SPEED > 0) return;
  for(unsigned int s = 0; s < NUM_STRIPS; s++){
    for(unsigned int l = 0; l < STRIP_LENGTHS[2]; l++){
      uint32_t colour = Sunset(sequence_no + l);
  		if(s % 2 == 1) {
  			// B strip
  			// B1
  			leds.setPixel(l + (s * ledsPerStrip), colour);
  			// B2
  			if(l < STRIP_LENGTHS[1]) leds.setPixel((s * ledsPerStrip) + STRIP_LENGTHS[2] + STRIP_LENGTHS[1] - l - 1, colour);
  			// B3
  			if(l < STRIP_LENGTHS[0]) leds.setPixel(l + (s * ledsPerStrip) + STRIP_LENGTHS[2] + STRIP_LENGTHS[1], colour);
  		} else {
  			// A strip
  			// A1
  			if(l < STRIP_LENGTHS[0]) leds.setPixel(l + (s * ledsPerStrip), colour);
  			// A2
  			if(l < STRIP_LENGTHS[1]) leds.setPixel((s * ledsPerStrip) + STRIP_LENGTHS[0] + STRIP_LENGTHS[1] - l - 1, colour);
  			// A3
  			leds.setPixel(l + (s * ledsPerStrip) + STRIP_LENGTHS[0] + STRIP_LENGTHS[1], colour);
  		}
      
    }
  }
  leds.show();
}

uint32_t Sunset(unsigned int i) {
  if (i > 1019) return Sunset(i % 1020);
  if (i > 764) return IdleColor((i % 255), 0, (255 - (i % 255)));          //blue -> red
  if (i > 509) return IdleColor((255 - (i % 255)), 0, 255);                //purple -> blue
  if (i > 255) return IdleColor(255, (128 - (i % 255) / 2), (i % 255));    //orange -> purple
  return IdleColor(255, (i / 2), 0);                                       //red -> orange
}

uint32_t Pulse(unsigned int i) {
  int PulseWidth = 179;
  if(i > (PulseWidth - 1)) return Pulse(i % PulseWidth);
  if(i == 0) return IdleColor(0, 255, 0);
  return IdleColor(0,0,0);
}
