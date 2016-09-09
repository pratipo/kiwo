
void ewrap() {
  Serial.println("ewrap");
  //  //use a nested for loop to initialize all the pins
  //  int pixel = patternStep * 3;
  //  for (int s = pixel; s <= pixel + 2; s++) {
  //    strip.setPixelColor(s, strip.Color(0, 0, 128));
  //    strip.show();
  //    delay(lapse);
  //    clearStrip();
  //  }
  //  delay(lapse);
  //  strip.setPixelColor(pixel + 1, strip.Color(200, 0, 0));
  strip.show();
}

void uwrap() {
  Serial.println("uwrap");
  //  //use a nested for loop to initialize all the pins
  //  int pixel = patternStep * 3;
  //  for (int s = pixel; s <= pixel + 2; s++) {
  //    strip.setPixelColor(s, strip.Color(0, 128, 0));
  //    strip.show();
  //    delay(lapse);
  //    clearStrip();
  //  }
  //  delay(lapse);
  //  strip.setPixelColor(pixel + 1, strip.Color(200, 0, 0));
  strip.show();

}

void purl() {
  Serial.println("purl");
//  //use a nested for loop to initialize all the pins
//  int pixel = patternStep * 3;
//  for (int s = pixel; s <= pixel + 2; s++) {
//    strip.setPixelColor(s, strip.Color(128, 128, 0));
//    strip.show();
//    delay(lapse);
//    clearStrip();
//  }
//  delay(lapse);
//  strip.setPixelColor(pixel + 1, strip.Color(200, 0, 0));
  strip.show();
}

//---------
//WS28012
// Fill the dots one after the other with a color

void colorStrip(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow(uint8_t wait, int np) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < np; i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//---------
// BLE
void setupBleConnection()
{
  BLE.begin(9600); //Set BLE BaudRate to default baud rate 9600
  BLE.print("AT+CLEAR"); //clear all previous setting
  BLE.print("AT+ROLE0"); //set the bluetooth name as a slaver
  BLE.print("AT+SAVE1");  //don't save the connect information
}

