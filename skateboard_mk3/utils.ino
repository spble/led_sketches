
void setupTouch(){
	for(int8_t i=0; i < NUM_TOUCH_PINS; i++){
		touch_base[i] = touchRead(touch_pins[i]);
	}
}

void setupLEDStrip(Adafruit_NeoPixel *strip, int brightness){
	strip->begin();
	strip->setBrightness(brightness);  // Lower brightness and save eyeballs!
	strip->show(); // Initialize all pixels to 'off'
}

void setupSerial(){
	Serial.begin(115200);
	Serial.println("Starting up...");
}

void debugPrint(String info){
	if(DEBUG) {
		Serial.println(info);
	}
}

void debugPrint() {
	if(DEBUG) {
		String msg = "";
		msg += "Seq: " + String(sequence_no) + "Bright: " + String(brightness)
            + " Mode: " + String(mode_no)
			+ " Touch: ";
		for(uint8_t i = 0; i < 4; i++){
			if(is_touched[i]){
				msg += char('A' + i);
				msg += ", ";
			}
		}
   msg += "A: " + String(touch_base[0]) + "(" + String(touchRead(touch_pins[0])) + ")" + " B: " + String(touch_base[1]) + "(" + String(touchRead(touch_pins[1])) + ")";
		debugPrint(msg);
	}
}


void updateIsTouched(){
	for(int8_t i=0; i < NUM_TOUCH_PINS; i++){
	    was_touched[i] = is_touched[i];
		is_touched[i] = touchRead(touch_pins[i]) >= (TOUCH_MULTIPLIER_THRESHOLD * touch_base[i]);
	}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return bstrip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return bstrip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return bstrip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
