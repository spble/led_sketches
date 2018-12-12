void rainbowRotate(int skip_cycles){
	int wheel_interval = floor(255 / strip.numPixels());
	uint32_t colour = Wheel(0);
	for(uint8_t i=0; i < strip.numPixels(); i++) {
		if(!(i % 2)){
			colour = Wheel(((i-1)*wheel_interval)+int(floor(sequence_no/skip_cycles)));
		}
		strip.setPixelColor(i, colour);
	}
}

void setExternalColour(uint32_t colour) {
	for(uint8_t i = 0; i < strip.numPixels(); i++) {
		if(is_external_led[i]){
			strip.setPixelColor(i, colour);
		}
	}
}

void twinkleExternal(int skip_cycles){
	if(sequence_no % skip_cycles > 0) return;

	uint32_t colour;
	uint8_t rand_pixel;
	uint8_t twinkle_index = int(floor(sequence_no / skip_cycles)) % MAX_TWINKLES;

	if (twinkles[twinkle_index] >= 0)
		strip.setPixelColor(twinkles[twinkle_index], BLACK);
	// Only accept random numbers that are external.
	while(!is_external_led[rand_pixel = random(strip.numPixels())]);
	twinkles[twinkle_index] = rand_pixel;
	strip.setPixelColor(twinkles[twinkle_index], Wheel(random(255)));
}

void twinkleExternal(uint32_t colour, int skip_cycles){
	if(sequence_no % skip_cycles > 0) return;

	uint8_t rand_pixel;
	uint8_t twinkle_index = int(floor(sequence_no / skip_cycles)) % MAX_TWINKLES;


	if (twinkles[twinkle_index] >= 0)
		strip.setPixelColor(twinkles[twinkle_index], BLACK);
	// Only accept random numbers that are external LEDs.
	while(!is_external_led[rand_pixel = random(strip.numPixels())]);
	twinkles[twinkle_index] = rand_pixel;
	strip.setPixelColor(twinkles[twinkle_index], colour);
}

void setAllPixels(uint32_t colour){
	for(uint8_t i = 0; i < strip.numPixels(); i++){
		strip.setPixelColor(i, colour);
	}
}
