
void setupTouch(){
	for(int8_t i=0; i < NUM_TOUCH_PINS; i++){
		touch_base[i] = touchRead(touch_pins[i]);
	}
}

void setupLEDStrip(int brightness){
	strip.begin();
	strip.setBrightness(brightness);  // Lower brightness and save eyeballs!
	strip.show(); // Initialize all pixels to 'off'
}

void setupSerial(){
	Serial.begin(1234);
	Serial.println("Starting up...");
}

void setupAxis() {
	rest_xyz_vals[0] = analogRead(AXIS_X);
	rest_xyz_vals[1] = analogRead(AXIS_Y);
	rest_xyz_vals[2] = analogRead(AXIS_Z);
}

void resetAxis() {
	setupAxis();
	for(uint8_t i = 0; i < 3; i++)
		axis_activity_max[i] = 0;
}



void debugPrint(String info){
	if(SERIAL_OUT) {
		Serial.print(info);
	}
}

void debugPrint() {
	if(SERIAL_OUT) {
		String msg = "";
		msg += "Seq: " + String(sequence_no)
			+ ", Activity: " + String(total_activity_metric)
			+ " (X: " + String(axis_activity[0])
			+ ", Y: " + String(axis_activity[1])
			+ ", Z: " + String(axis_activity[2]) + ")"
			+ " Touch: ";
		for(uint8_t i = 0; i < 4; i++){
			if(is_touched[i]){
				msg += char('A' + i);
				msg += ", ";
			}
		}
    int16_t *xyz = getXYZ();
    msg += "X,Y,Z: " 
      + String(xyz[0]) + ", "
      + String(xyz[1]) + ", "
      + String(xyz[2]);
		debugPrint(msg);
	}
}


void updateIsTouched(){
	for(int8_t i=0; i < NUM_TOUCH_PINS; i++){
	    was_touched[i] = is_touched[i];
		is_touched[i] = touchRead(touch_pins[i]) >= (TOUCH_MULTIPLIER_THRESHOLD * touch_base[i]);
	}
}

void updateAxis(){
	int axis_seq = sequence_no % AXIS_HISTORICAL_COUNT;
	int axis_vals_raw[3];
	axis_vals_raw[0] = analogRead(AXIS_X);
	axis_vals_raw[1] = analogRead(AXIS_Y);
	axis_vals_raw[2] = analogRead(AXIS_Z);
	
	for(uint8_t i = 0; i < 3; i++){
		axis_vals[axis_seq][i] = int(axis_vals_raw[i] - rest_xyz_vals[i]);

		// Handle wrap-around for diffs
		if(axis_seq > 0){
			axis_diffs[axis_seq][i] = axis_vals[axis_seq][i] - axis_vals[axis_seq - 1][i];
		} else {
			axis_diffs[axis_seq][i] = axis_vals[axis_seq][i] - axis_vals[AXIS_HISTORICAL_COUNT - 1][i];
		}
	}
}

int16_t *getXYZ(){
	int axis_seq = sequence_no % AXIS_HISTORICAL_COUNT;
	return axis_vals[axis_seq];
}

/* Activity metric is a number 0-5 for how much activity has happened during the last
 * 'AXIS_HISTORICAL_COUNT' cycles. It has an X,Y,Z version (axis_activity), and an
 * average of all axis (total_activity_metric). */
void updateActivityMetrics(){
	double aggregate_diffs[3] = {0};
	for(uint8_t j = 0; j < 3; j++){
		for(uint16_t i = 0; i < AXIS_HISTORICAL_COUNT; i++){
			aggregate_diffs[j] += abs(axis_diffs[i][j]);
		}
		aggregate_diffs[j] /= AXIS_HISTORICAL_COUNT;
	}

	total_activity_metric = 0;
	for(uint8_t j = 0; j < 3; j++){
		if(aggregate_diffs[j] > axis_activity_max[j]){
			// Update max threshold
			axis_activity_max[j] = aggregate_diffs[j];
		}
		total_activity_metric +=
			axis_activity[j] = int(ceil(AXIS_ACTIVITY_POINTS * aggregate_diffs[j] / axis_activity_max[j] ));
	}
	total_activity_metric = int(floor(total_activity_metric / 3));
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

uint32_t colourFromAxis(){
	int16_t *xyz = getXYZ();
	uint8_t red, green, blue;
	red = int(pow(floor(xyz[0]/6.0),2));
	green = int(pow(floor(xyz[1]/6.0),2));
	blue = int(pow(floor(xyz[2]/6.0),2));
	return strip.Color(red, green, blue);
}

long gaussian_pulse(long x, long width) {
	if(abs(x) > width*5) return 0;
	return 255*exp(-1*((pow(x, 2))/(pow(2*width, 2))));
}

long leading_pulse(long x, long trail) {
	if(x > 0) return 0;
	return gaussian_pulse(x, trail);
}
