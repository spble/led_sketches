void pulseBackBoth(Adafruit_NeoPixel *s1, Adafruit_NeoPixel *s2, uint32_t colour, uint8_t wait){

    uint8_t PULSE_LENGTH = 3;
    uint8_t mid_length_1 = s1->numPixels() - 2*SIDELENGTH;
    uint8_t mid_length_2 = s2->numPixels() - 2*SIDELENGTH;

    // Build front
    for(uint8_t i=0; i <= 254; i++){
        for(uint8_t j=SIDELENGTH; j < SIDELENGTH + mid_length_1; j++){
            s1->setPixelColor(j, colour);
            s1->setBrightness(i);
        }
        for(uint8_t k=SIDELENGTH; k < SIDELENGTH + mid_length_2; k++){
            s2->setPixelColor(k, colour);
            s2->setBrightness(i);
        }
        s1->show();
        s2->show();

        delay(wait);
    }
    for(uint8_t j=SIDELENGTH; j < SIDELENGTH + mid_length_1; j++){
        s1->setPixelColor(j, s1->Color(0,0,0));
    }
    for(uint8_t k=SIDELENGTH; k < SIDELENGTH + mid_length_2; k++){
        s2->setPixelColor(k, s1->Color(0,0,0));
    }


    colourWipeBoth(s1, s2, colour, wait);


    s1->setBrightness(BASE_BRIGHTNESS);
    s2->setBrightness(BASE_BRIGHTNESS);

}

void colourWipeBoth(Adafruit_NeoPixel *s1, Adafruit_NeoPixel *s2, uint32_t colour, uint32_t wait){
    uint16_t pixel_1 = ceil(s1->numPixels()/2);
    uint16_t pixel_2 = ceil(s2->numPixels()/2);
    uint16_t limit = max(pixel_1, pixel_2);

    for(uint16_t i=0; i <= limit; i++) {
        s1->setPixelColor(pixel_1 + i, colour);
        s1->setPixelColor(pixel_1 - i, colour);
        s2->setPixelColor(pixel_2 + i, colour);
        s2->setPixelColor(pixel_2 - i, colour);
        s1->show();
        s2->show();
        delay(wait);
    }
}

void colourWipeRedYellow(Adafruit_NeoPixel *tstrip, Adafruit_NeoPixel *bstrip){
    if(sequence_no % 200 > 100) {
        colourWipeBoth(tstrip, bstrip, RED, 3);
    } else {
        colourWipeBoth(tstrip, bstrip, YELLOW, 3);
    }

}

void rainbow(Adafruit_NeoPixel *tstrip, Adafruit_NeoPixel *bstrip) {
    uint16_t i;
    for(i=0; i< tstrip->numPixels(); i++) {
        tstrip->setPixelColor(i, Wheel((i+sequence_no) & 255));
    }

    for(i=0; i< bstrip->numPixels(); i++) {
        bstrip->setPixelColor(i, Wheel((i+sequence_no) & 255));
    }
    tstrip->show();
    bstrip->show();
}

void setAllPixels(Adafruit_NeoPixel *strip, uint32_t colour){
	for(uint16_t i = 0; i < strip->numPixels(); i++){
		strip->setPixelColor(i, colour);
	}
	strip->show();
}

void rotatePaletteSunset(Adafruit_NeoPixel *tstrip, Adafruit_NeoPixel *bstrip){
	//if(sequence_no % skip_cycles > 0) return;
    uint16_t pixel_1 = ceil(tstrip->numPixels()/2);
    uint16_t pixel_2 = ceil(bstrip->numPixels()/2);
    uint16_t limit = max(pixel_1, pixel_2);
    for(unsigned int l = 0; l < limit; l++){
        tstrip->setPixelColor(pixel_1 + l, Sunset(sequence_no + l));
        tstrip->setPixelColor(pixel_1 - l, Sunset(sequence_no + l));
        bstrip->setPixelColor(pixel_2 + l, Sunset(sequence_no + l));
        bstrip->setPixelColor(pixel_2 - l, Sunset(sequence_no + l));
    }
    tstrip->show();
    bstrip->show();
}

uint32_t Sunset(unsigned int i) {
    if (i > 1019) return Sunset(i % 1020);
    if (i > 764) return tstrip.Color((i % 255), 0, 255 - (i % 255));          //blue -> red
    if (i > 509) return tstrip.Color(255 - (i % 255), 0, 255);                //purple -> blue
    if (i > 255) return tstrip.Color(255, 128 - (i % 255) / 2, (i % 255));    //orange -> purple
    return tstrip.Color(255, i / 2, 0);                                       //red -> orange
}
