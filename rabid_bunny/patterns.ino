void rainbowRotate(int skip_cycles){
	int wheel_interval = floor(255 / strip.numPixels());
	uint32_t colour = Wheel(0);
	for(uint16_t i=0; i < strip.numPixels(); i++) {
		if(!(i % 2)){
			colour = Wheel(((i-1)*wheel_interval)+int(floor(sequence_no/skip_cycles)));
		}
		strip.setPixelColor(i, colour);
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
		strip.setPixelColor(l, Sunset(sequence_no + l));
	}
}

void dragonBomb(){

	unsigned int num_pulses = 8;
	float pulse_gap = 20;
	unsigned int gaussian_width = 10;
	
	unsigned int pulse_length = (SEC_BACK_END / num_pulses) + pulse_gap;
	unsigned int starting_offset = sequence_no % (int)(SEC_BACK_END*2.8);


	unsigned int pulse_no = 0;
	unsigned int red, green, blue;
	long x_position;
	for(unsigned int l = 0; l < strip.numPixels(); l++){
		if(l < SEC_BACK_END){
			// Do spiral pulse
			red = 0;
			green = 0;
			blue = 0;
			pulse_no = l/pulse_length;
			for(unsigned int p = 0; p < num_pulses; p++) {
				x_position = (l - starting_offset + gaussian_width*4) + (p * pulse_length);
				green += leading_pulse(x_position, gaussian_width);
				blue += leading_pulse(x_position, gaussian_width/2);
			}
			strip.setPixelColor(l, strip.Color(red, green, blue));
		} else {
			red = green - random(0, green/2);
			red += blue - random(0, blue/2);
			strip.setPixelColor(l, strip.Color(red, 0, red*0.05));
		}
	}

	
}
