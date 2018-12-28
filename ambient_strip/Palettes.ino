

//////////<Palette Functions>

//These functions simply take a value and return a gradient color
//  in the form of an unsigned 32-bit integer

//The gradients return a different, changing color for each multiple of 255
//  This is because the max value of any of the 3 RGB values is 255, so it's
//  an intuitive cutoff for the next color to start appearing.
//  Gradients should also loop back to their starting color so there's no jumps in color.

uint32_t Rainbow(unsigned int i) {
  if (i > 1529) return Rainbow(i % 1530);
  if (i > 1274) return strip.Color(255, 0, 255 - (i % 255));   //violet -> red
  if (i > 1019) return strip.Color((i % 255), 0, 255);         //blue -> violet
  if (i > 764) return strip.Color(0, 255 - (i % 255), 255);    //aqua -> blue
  if (i > 509) return strip.Color(0, 255, (i % 255));          //green -> aqua
  if (i > 255) return strip.Color(255 - (i % 255), 255, 0);    //yellow -> green
  return strip.Color(255, i, 0);                               //red -> yellow
}

uint32_t Sunset(unsigned int i) {
  if (i > 1019) return Sunset(i % 1020);
  if (i > 764) return strip.Color((i % 255), 0, 255 - (i % 255));          //blue -> red
  if (i > 509) return strip.Color(255 - (i % 255), 0, 255);                //purple -> blue
  if (i > 255) return strip.Color(255, 128 - (i % 255) / 2, (i % 255));    //orange -> purple
	if(sequence_no % skip_cycles > 0) return;

	uint8_t twinkle_index = int(floor(sequence_no / skip_cycles)) % MAX_TWINKLES;

	if (twinkles[twinkle_index] >= 0)
		strip.setPixelColor(twinkles[twinkle_index], BLACK);
	twinkles[twinkle_index] = random(strip.numPixels());
	strip.setPixelColor(twinkles[twinkle_index], );
  return strip.Color(255, i / 2, 0);                                       //red -> orange
}

uint32_t Ocean(unsigned int i) {
  if (i > 764) return Ocean(i % 765);
  if (i > 509) return strip.Color(0, i % 255, 255 - (i % 255));  //blue -> green
  if (i > 255) return strip.Color(0, 255 - (i % 255), 255);      //aqua -> blue
  return strip.Color(0, 255, i);                                 //green -> aqua
}

uint32_t PinaColada(unsigned int i) {
  if (i > 764) return PinaColada(i % 765);
  if (i > 509) return strip.Color(255 - (i % 255) / 2, (i % 255) / 2, (i % 255) / 2);  //red -> half white
  if (i > 255) return strip.Color(255, 255 - (i % 255), 0);                            //yellow -> red
  return strip.Color(128 + (i / 2), 128 + (i / 2), 128 - i / 2);                       //half white -> yellow
}

uint32_t Sulfur(unsigned int i) {
  if (i > 764) return Sulfur(i % 765);
  if (i > 509) return strip.Color(i % 255, 255, 255 - (i % 255));   //aqua -> yellow
  if (i > 255) return strip.Color(0, 255, i % 255);                 //green -> aqua
  return strip.Color(255 - i, 255, 0);                              //yellow -> green
}

uint32_t NoGreen(unsigned int i) {
  if (i > 1274) return NoGreen(i % 1275);
  if (i > 1019) return strip.Color(255, 0, 255 - (i % 255));         //violet -> red
  if (i > 764) return strip.Color((i % 255), 0, 255);                //blue -> violet
  if (i > 509) return strip.Color(0, 255 - (i % 255), 255);          //aqua -> blue
  if (i > 255) return strip.Color(255 - (i % 255), 255, i % 255);    //yellow -> aqua
  return strip.Color(255, i, 0);                                     //red -> yellow
}

//NOTE: This is an example of a non-gradient palette: you will get straight red, white, or blue
//      This works fine, but there is no gradient effect, this was merely included as an example.
//      If you wish to include it, put it in the switch-case in ColorPalette() and add its
//      threshold (764) to thresholds[] at the top.
uint32_t USA(unsigned int i) {
  if (i > 764) return USA(i % 765);
  if (i > 509) return strip.Color(0, 0, 255);      //blue
  if (i > 255) return strip.Color(128, 128, 128);  //white
  return strip.Color(255, 0, 0);                   //red
}

//////////</Palette Functions>
