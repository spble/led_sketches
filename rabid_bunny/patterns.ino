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
