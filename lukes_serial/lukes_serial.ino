// Glediator example with OctoWS2811, by mortonkopf
//
// https://forum.pjrc.com/threads/33012-Gladiator-with-OctoWS2811-working-example

// You can also use Jinx to record Glediator format data to a SD card.
// To play the data from your SD card, use this modified program:
// https://forum.pjrc.com/threads/46229&viewfull=1#post153927

#include <OctoWS2811.h>


const int ledsPerStrip = 300;
const int NUM_LEDS = 2400;
const int NUM_STRIPS = 8;

const int IDLE_TIMEOUT = 50000000;
// This mods the sequence, so that a lower number is faster.
const int IDLE_PATTERN_SPEED = 1;
// Divides the colour, lower numbers is brighter
const int IDLE_BRIGHTNESS = 14;

DMAMEM int displayMemory[ledsPerStrip*6];
int drawingMemory[ledsPerStrip*6];

const int config = WS2811_GRB | WS2811_800kHz;
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

  //while (serialGlediator() != 1) {idlePattern(5, 30);}

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
  for(unsigned int l = 0; l < ledsPerStrip; l++){
    for(unsigned int s = 0; s < NUM_STRIPS; s++){
        leds.setPixel(l + (s * ledsPerStrip), Sunset(sequence_no + l));
    }
  }
  leds.show();
}

uint32_t Sunset(unsigned int i) {
  if (i > 1019) return Sunset(i % 1020);
  if (i > 764) return Color((i % 255)/IDLE_BRIGHTNESS, 0, (255 - (i % 255))/IDLE_BRIGHTNESS);          //blue -> red
  if (i > 509) return Color((255 - (i % 255))/IDLE_BRIGHTNESS, 0, 255/IDLE_BRIGHTNESS);                //purple -> blue
  if (i > 255) return Color(255/IDLE_BRIGHTNESS, (128 - (i % 255) / 2)/IDLE_BRIGHTNESS, (i % 255)/IDLE_BRIGHTNESS);    //orange -> purple
  return Color(255/IDLE_BRIGHTNESS, (i / 2)/IDLE_BRIGHTNESS, 0);                                       //red -> orange
}
