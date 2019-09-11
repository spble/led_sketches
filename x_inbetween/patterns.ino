
void colourWipeBothUp(Adafruit_NeoPixel *s1, Adafruit_NeoPixel *s2, uint32_t colour, uint32_t wait){
    uint16_t i = sequence_no % max(s1->numPixels(), s2->numPixels());
    if(i <= s1->numPixels()) s1->setPixelColor(i, colour);
    if(i <= s2->numPixels()) s2->setPixelColor(i, colour);
    s1->show();
    s2->show();
}

void colourWipeBothMiddle(Adafruit_NeoPixel *s1, Adafruit_NeoPixel *s2, uint32_t colour, uint32_t wait){
    uint16_t pixel_1 = ceil(s1->numPixels()/2);
    uint16_t pixel_2 = ceil(s2->numPixels()/2);
    uint16_t limit = max(pixel_1, pixel_2);

    uint16_t i = sequence_no % limit;
    s1->setPixelColor(pixel_1 + i, colour);
    s1->setPixelColor(pixel_1 - i, colour);
    s2->setPixelColor(pixel_2 + i, colour);
    s2->setPixelColor(pixel_2 - i, colour);
    s1->show();
    s2->show();
}

void colourWipeWhiteBlackRed(Adafruit_NeoPixel *tstrip, Adafruit_NeoPixel *bstrip){
    unsigned int current_colour = (unsigned int)floor(sequence_no/max(bstrip->numPixels(), tstrip->numPixels())) % 3;
    switch(current_colour) {
        case 0:
            colourWipeBothUp(tstrip, bstrip, WHITE, 0);
            break;
        case 1:
            colourWipeBothUp(tstrip, bstrip, BLACK, 0);
            break;
        case 2:
            colourWipeBothUp(tstrip, bstrip, RED, 0);
            break;
    }

}

void colourWipeRedYellow(Adafruit_NeoPixel *tstrip, Adafruit_NeoPixel *bstrip){
    if((unsigned int)floor(sequence_no/(uint32_t)ceil(tstrip->numPixels()/2)) % 2 == 0) {
        colourWipeBothMiddle(tstrip, bstrip, RED, 0);
    } else {
        colourWipeBothMiddle(tstrip, bstrip, YELLOW, 0);
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

void nebula(Adafruit_NeoPixel *tstrip, Adafruit_NeoPixel *bstrip){
    for(unsigned int i; i < NUM_TWINKLES; i++) {
        if(sequence_no >= twinkles[i].start_no){
            if(sequence_no < (twinkles[i].start_no + twinkles[i].period)){
                // We're in the middle of a twinkle, let's render.
                render_twinkle(tstrip, &twinkles[i]);
                render_twinkle(bstrip, &twinkles[i]);
            } else {
                // Twinkle is in the past, reinitialise it
                twinkles[i] = initialise_twinkle();
            }
        }
    }
    tstrip->show();
    bstrip->show();
}

void render_twinkle(Adafruit_NeoPixel *strip, Twinkle *t){
    for(unsigned int j = 0; j < t->length; j++){
        uint8_t period_brightness = ((sequence_no - t->start_no)/t->period)*255;
        uint8_t brightness = 255*pow(EULER, -1*(((j - t->length)^2)/(2*(t->length / 3)^2)));
        uint8_t r = ((uint8_t)(t->colour >> 16) * brightness) >> 8;
        uint8_t g = ((uint8_t)(t->colour >> 8) * brightness) >> 8;
        uint8_t b = ((uint8_t)t->colour * brightness) >> 8;
        strip->setPixelColor(t->position + j, r, g, b);
    }
}

Twinkle initialise_twinkle(){
    return (Twinkle){sequence_no + 60, 50, 1, random(3, 297), WHITE};
}
