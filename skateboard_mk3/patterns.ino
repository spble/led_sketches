void pulseBackBoth(Adafruit_NeoPixel *tlstrip, Adafruit_NeoPixel *trstrip, Adafruit_NeoPixel *bstrip, uint32_t colour, uint8_t wait){

    uint8_t PULSE_LENGTH = 3;
    uint8_t mid_length_bot = bstrip->numPixels() - 2*BOTTOMSIDELENGTH;

    // Build front
    for(uint8_t i=0; i <= 254; i++){
        for(uint8_t j=BOTTOMSIDELENGTH; j < BOTTOMSIDELENGTH + mid_length_bot; j++){
            bstrip->setPixelColor(j, colour);
            bstrip->setBrightness(i);
        }
        bstrip->show();

        delay(wait);
    }
    for(uint8_t j=BOTTOMSIDELENGTH; j < BOTTOMSIDELENGTH + mid_length_bot; j++){
        bstrip->setPixelColor(j, bstrip->Color(0,0,0));
    }

    bstrip->setBrightness(brightness);
    
    colourWipeAll(tlstrip, trstrip, bstrip, colour, wait);

}

void colourWipeAll(Adafruit_NeoPixel *s1, Adafruit_NeoPixel *s2, Adafruit_NeoPixel *s3, uint32_t colour, uint32_t wait){
    uint16_t pixel_1 = ceil(s1->numPixels()/2);
    uint16_t pixel_2 = ceil(s2->numPixels()/2);
    uint16_t pixel_3 = ceil(s3->numPixels()/2);

    uint16_t limit = max(max(pixel_1, pixel_2), pixel_3);

    for(uint16_t i=0; i <= limit; i++) {
        s1->setPixelColor(pixel_1 + i, colour);
        s1->setPixelColor(pixel_1 - i, colour);
        s2->setPixelColor(pixel_2 + i, colour);
        s2->setPixelColor(pixel_2 - i, colour);
        s3->setPixelColor(pixel_3 + i, colour);
        s3->setPixelColor(pixel_3 - i, colour);
        s1->show();
        s2->show();
        s3->show();
        delay(wait);
    }
}

void colourWipeRedYellow(Adafruit_NeoPixel *tlstrip, Adafruit_NeoPixel *trstrip, Adafruit_NeoPixel *bstrip){
    if(sequence_no % 50 > 25) {
        colourWipeAll(tlstrip, trstrip, bstrip, RED, 1);
    } else {
        colourWipeAll(tlstrip, trstrip, bstrip, YELLOW, 1);
    }

}

void rainbow(Adafruit_NeoPixel *tlstrip, Adafruit_NeoPixel *trstrip, Adafruit_NeoPixel *bstrip) {
    uint16_t i;
    for(i=0; i< tlstrip->numPixels(); i++) {
        tlstrip->setPixelColor(i, Wheel((i+sequence_no) & 255));
    }
    for(i=0; i< trstrip->numPixels(); i++) {
        trstrip->setPixelColor(i, Wheel((i+sequence_no) & 255));
    }

    for(i=0; i< bstrip->numPixels(); i++) {
        bstrip->setPixelColor(i, Wheel((i+sequence_no) & 255));
    }
    tlstrip->show();
    trstrip->show();
    bstrip->show();
}

void setAllPixels(Adafruit_NeoPixel *strip, uint32_t colour){
	for(uint16_t i = 0; i < strip->numPixels(); i++){
		strip->setPixelColor(i, colour);
	}
	strip->show();
}

void rotatePaletteSunset(Adafruit_NeoPixel *tlstrip, Adafruit_NeoPixel *trstrip, Adafruit_NeoPixel *bstrip){
	//if(sequence_no % skip_cycles > 0) return;
    uint16_t pixel_1 = ceil(tlstrip->numPixels()/2);
    uint16_t pixel_2 = ceil(trstrip->numPixels()/2);
    uint16_t pixel_3 = ceil(bstrip->numPixels()/2);
    uint16_t limit = max(max(pixel_1, pixel_2), pixel_3);
    for(unsigned int l = 0; l < limit; l++){
        tlstrip->setPixelColor(pixel_1 + l, Sunset(sequence_no + l));
        tlstrip->setPixelColor(pixel_1 - l, Sunset(sequence_no + l));
        trstrip->setPixelColor(pixel_1 + l, Sunset(sequence_no + l));
        trstrip->setPixelColor(pixel_1 - l, Sunset(sequence_no + l));
        bstrip->setPixelColor(pixel_2 + l, Sunset(sequence_no + l));
        bstrip->setPixelColor(pixel_2 - l, Sunset(sequence_no + l));
    }
    tlstrip->show();
    trstrip->show();
    bstrip->show();
}

uint32_t Sunset(unsigned int i) {
    if (i > 1019) return Sunset(i % 1020);
    if (i > 764) return tlstrip.Color((i % 255), 0, 255 - (i % 255));          //blue -> red
    if (i > 509) return tlstrip.Color(255 - (i % 255), 0, 255);                //purple -> blue
    if (i > 255) return tlstrip.Color(255, 128 - (i % 255) / 2, (i % 255));    //orange -> purple
    return tlstrip.Color(255, i / 2, 0);                                       //red -> orange
}
