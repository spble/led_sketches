void rainbowRotate(int skip_cycles){
	int wheel_interval = 3;// floor(255 / strip.numPixels());
	uint32_t colour = Wheel(0);
	for(uint16_t i=0; i < strip.numPixels(); i++) {
		if(i < SEC_BACK_END){
			if(!(i % 2)){
				colour = Wheel(((i-1)*wheel_interval)+int(floor(sequence_no/skip_cycles)));
			}
			strip.setPixelColor(i, colour);
		} else {
			strip.setPixelColor(i, strip.getPixelColor(SEC_BACK_END - 1));
		}
	}
}

void mountainsRotate(int skip_cycles){
	
}

void twinkle(int skip_cycles){
	twinkle(Wheel(random(255)), skip_cycles);
}

void twinkle(uint32_t colour, int skip_cycles){
	if(sequence_no % skip_cycles > 0) return;

	uint16_t twinkle_index = int(floor(sequence_no / skip_cycles)) % MAX_TWINKLES;


	if (twinkles[twinkle_index] >= 0)
		strip.setPixelColor(twinkles[twinkle_index], BLACK);
	twinkles[twinkle_index] = random(0, strip.numPixels());
	strip.setPixelColor(twinkles[twinkle_index], colour);
}

void setAllPixels(uint32_t colour){
	for(uint16_t i = 0; i < strip.numPixels(); i++){
		strip.setPixelColor(i, colour);
	}
}

void rotatePaletteSunset(int skip_cycles){
	if(sequence_no % skip_cycles > 0) return;
	for(unsigned int l = 0; l < strip.numPixels(); l++){
		if(l < SEC_BACK_END){
			strip.setPixelColor(l, Sunset(sequence_no + l));
		} else {
			strip.setPixelColor(l, strip.getPixelColor(SEC_BACK_END - 1));
		}
	}
}

void dragonBomb(){

	unsigned int num_pulses = 8;
	float pulse_gap = 20;
	unsigned int gaussian_width = 10;
	
	unsigned int pulse_length = (SEC_BACK_END / num_pulses) + pulse_gap;
	unsigned int starting_offset = sequence_no % (int)(SEC_BACK_END*2.8);


	unsigned int red, green, blue;
	long x_position;
	for(unsigned int l = 0; l < strip.numPixels(); l++){
		if(l < SEC_BACK_END){
			// Do spiral pulse
			red = 0;
			green = 0;
			blue = 0;
			for(unsigned int p = 0; p < num_pulses; p++) {
				x_position = (l - starting_offset + gaussian_width*4) + (p * pulse_length);
				green += leading_pulse(x_position + 4, gaussian_width);
				blue += leading_pulse(x_position, gaussian_width/1.2);
				red += (leading_pulse(x_position, gaussian_width/4));
			}
			strip.setPixelColor(l, strip.Color(red, green, blue));
		} else {
			red = green - random(0, green/2);
			strip.setPixelColor(l, strip.Color(red, 0, red*0.05));
		}
	}
}



#define NUM_PARTICLES 10 
#define PARTICLE_MASS  5
#define AIR_RESISTANCE 0.1
#define GRAVITY -9.8
uint8_t current_particle = 0;
uint32_t PARTICLE_COLOUR = strip.Color(255,255,255);
struct Particle {
	double mass;
	double x;
	double dx;
	uint32_t colour;
};
Particle particles[NUM_PARTICLES];
bool particles_initialized = false;
double current_accel, force;
unsigned long time_step;
unsigned long previous_time = millis();

void looseParticles() {
	setAllPixels(BLACK);
	time_step =  millis() - previous_time;
	if(!particles_initialized){
		for(unsigned int i = 0; i < NUM_PARTICLES; i++){
			particles[i].mass = PARTICLE_MASS;
			particles[i].x = SEC_BACK_END - i;
			particles[i].dx = 0;
			particles[i].colour = strip.Color(random(0,255), random(0,255), random(0,255));
		}
		particles_initialized = true;
	}

	int16_t *xyz = getXYZ();
	current_accel = GRAVITY + (max(abs(xyz[0]),max(abs(xyz[1]),abs(xyz[2]))))/5.0;
	debugPrint(current_accel);

	for(unsigned int i = 0; i < NUM_PARTICLES; i++){
		Particle *p = &particles[i];
		if(p->x <= 0 || p->x >= SEC_BACK_END){
			p->dx = 0;
		} 
		p->dx += (current_accel + 0.1*random(-50, 50))  * (time_step*0.001);
		p->x = max(min(p->x + p->dx, SEC_BACK_END), 0);
		strip.setPixelColor(p->x, p->colour);
//		debugPrint(p->x);
//		debugPrint(p->dx);
	}
	previous_time = millis();

	uint32_t last_spiral_led_colour = strip.getPixelColor(SEC_BACK_END-1);
	for(unsigned int i = 0; i < (SEC_MOUTH_TOP_RIGHT_END - SEC_BACK_END); i++){
		strip.setPixelColor(SEC_BACK_END + i, last_spiral_led_colour);
	}

}

void particleReset(){
	particles[current_particle].mass = PARTICLE_MASS;
	particles[current_particle].x = SEC_BACK_END;
	particles[current_particle].dx = -0.5;
	current_particle = (current_particle + 1) % NUM_PARTICLES;
}


void descendingTwinklePulse() {
	for(unsigned int i = 0; i < strip.numPixels(); i++){
		double sine = sin(((double)((i + sequence_no) % strip.numPixels())/(double)strip.numPixels())*2.0*PI);
		strip.setPixelColor(i, strip.Color(sine*random(20, 170), sine*random(0, 60), sine*random(80, 255)));
	}
}

// Adapted from: https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectFire
void fire(unsigned int Cooling, unsigned int Sparking) {
  static byte heat[SEC_BACK_END];
  int cooldown;

  // Step 1.  Cool down every cell a little
  for( int i = 0; i < SEC_BACK_END; i++) {
    cooldown = random(0, ((Cooling * 10) / SEC_BACK_END) + 2);

    if(cooldown>heat[i]) {
      heat[i]=0;
    } else {
      heat[i]=heat[i]-cooldown;
    }
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for( int k= SEC_BACK_END - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2]) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' near the bottom
  if( random(255) < Sparking ) {
    int y = random(7);
    heat[y] = heat[y] + random(160,255);
    //heat[y] = random(160,255);
  }

  // Step 4.  Convert heat to LED colors
  for( int j = 0; j < STRIP_SIZE; j++) {
	if(j < SEC_BACK_END){
		setPixelHeatColor(j, heat[j] );
	} else {
		setPixelHeatColor(j, heat[SEC_BACK_END-1]);
	}
  }
}

void setPixelHeatColor (int Pixel, byte temperature) {
  // Scale 'heat' down from 0-255 to 0-191
  byte t192 = round((temperature/255.0)*191);

  // calculate ramp up from
  byte heatramp = t192 & 0x3F; // 0..63
  heatramp <<= 2; // scale up to 0..252

  // figure out which third of the spectrum we're in:
  if( t192 > 0x80) {                     // hottest
    setPixel(Pixel, 255, 255, heatramp);
  } else if( t192 > 0x40 ) {             // middle
    setPixel(Pixel, 255, heatramp, 0);
  } else {                               // coolest
    setPixel(Pixel, heatramp, 0, 0);
  }
}

void setPixel(int i, uint8_t r, uint8_t g, uint8_t b){
	strip.setPixelColor(i, strip.Color(r,g,b));
}
