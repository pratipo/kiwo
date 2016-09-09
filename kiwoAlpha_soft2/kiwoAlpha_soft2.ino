/*
 * KIWO -knit it with others- alpha 2
 * medialab prado, june 2016
 * Copyrights (C) 2016 Raul Nieves Pardo
 * GPL v3  https://www.gnu.org/licenses/gpl-3.0.html
 */

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>   //Software Serial Port
#include <EEPROM.h>

#include "config.h"
#include "leds.h"
#include "nutsnbolts.h"


//-------------------

void setup() {

  // init button pin & interrupt
  pinMode(interruptPinF, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPinF), fBoton, RISING); // detect button release
  pinMode(interruptPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPinB), bBoton, RISING); // detect button release

  // init neopixel
  strip.begin();

  // demo lightning
  rainbow(5, PEGS);
  colorStrip(strip.Color(0,0,0), 1);

  // init softserial: HM-10 BLE connection
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBleConnection();
  
  // init serial debugging
  Serial.begin(9600);
  Serial.println("KNIT IT WITH OTHERS alpha \n");

  delay(500);
  
  // recover last position!
  if (EEPROM.read(0) && EEPROM.read(1) ) { 
    step = (int) EEPROM.read(0)<<8 | (int) EEPROM.read(1);
    
    Serial.print("recovering last position = ");
    Serial.println(step);
  }

  buttonPressed = LOW;
}

//-------------------

void loop() {
  
  //  receive pattern, parse json file
  if (BLE.available() > 0) { 
    Serial.println("BLE available");
    //check for a repeated stich pattern inccoming, return

    byte val;
    int address = 2;
    while (BLE.available()) {
      val = BLE.read();
      Serial.println(val);
      
      EEPROM.update(address++, val);
      
      if (address == EEPROM.length()){
        Serial.println("error EEPROM length");
        colorStrip(strip.Color(255,0,0), 2000);
        //return;
      }
    }

    eepromRead();

    // reset "step" pointer
    EEPROM.update(0, B00000000);
    EEPROM.update(1, B00000000);
  }

  if (buttonPressed) {
    Serial.println("button pressed");
    buttonPressed = LOW;
    
    if (step >= patternLength()) {
      Serial.println("end of pattern");
      colorStrip(strip.Color(0, 255, 0), 3000);
      step = 0;
      return;
    }
    else if (step < 0) { // avoid underflowing
      Serial.println("prevented underflow");
      step = 0;
    }

    Serial.println("step it");
    stepPattern(step);

    //update persistent "step" 
    EEPROM.update(0, (byte)((step>>8)&B11111111));
    EEPROM.update(1, (byte)(step&B11111111));
  }
}

void stepPattern(int step){
  int eepromPos = stitchEOffset + step*2;
  byte hB = EEPROM.read(eepromPos);
  byte lB = EEPROM.read(eepromPos+1);

  word stitch = (word)hB<<8 | (word)lB; // built up a word from two bytes

  uint32_t c = colour(stitch);
  byte p = peg(stitch);
  
  stitchStrip(c, p);  
}


