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

	unsigned int num_pulses = 1;
	float pulse_width = 0.8;
  unsigned int gaussian_width = 8;
	
	unsigned int pulse_length = (SEC_BACK_END / num_pulses);
	unsigned int starting_offset = sequence_no % (SEC_BACK_END*3);


  unsigned int pulse_no = 0;
  unsigned int red, green, blue;
	for(unsigned int l = 0; l < strip.numPixels(); l++){
		if(l < SEC_BACK_END){
			// Do spiral pulse
      pulse_no = l/pulse_length;
      red = 0;
      green = gaussian_pulse((l - starting_offset)- (pulse_no * pulse_length), gaussian_width);
      blue = gaussian_pulse((l - starting_offset)- (pulse_no * pulse_length), gaussian_width/2);
			strip.setPixelColor(l, strip.Color(red, green, blue));
		} else {
      if(green > 0){
			  red = 255 - random(1, starting_offset);
			  strip.setPixelColor(l, strip.Color(red, 0, 0));
      } else {
        strip.setPixelColor(l, BLACK);
      }
		}
	}


	
}
