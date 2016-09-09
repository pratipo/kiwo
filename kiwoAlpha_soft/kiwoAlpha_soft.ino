#include "JsonStreamingParser.h"
//#include "JsonListener.h"
#include "kiwoParser.h"
//#include "nutsnbolts.h"

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>   //Software Serial Port

//-------------------
#define RxD 2
#define TxD 3
//-------------------
#define NEOPIN 6
#define PEGS 26
//-------------------

SoftwareSerial BLE(RxD, TxD);

JsonStreamingParser parser;
kiwoParser kparser;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PEGS , NEOPIN, NEO_GRB + NEO_KHZ800);

const byte interruptPinF = 2;
const byte interruptPinB = 2;

volatile byte buttonPressed = LOW;
volatile int step = 0;

int eepromPos = 0;

// json: {id:|0-65536|, type:|"single"|"round"|"double"|, colors:[B, B, B, ...] size:{width:W, length:L}, pattern:[I, I , I ....]} ; 
  // I  [a b c d e f g h   i j k l m n o p]: int | 16 bits
    // [a b] color: 2 bits 
    // [c d e f g h i j]peg number: byte
    // [k l m] stitch type: 3 bits (cast-on, cast off, e-wrap , u-wrap, purl, cable, increase, decrease )
    // [n o p] extra: 3 bits (cable: target offset (from origin): [-4, +4])

struct dataStruct{
  int id;
  uint8_t type;
  uint8_t sizeW;
  uint8_t sizeL;
  uint8_t peg;
  int stitch[255];
}pattern;

//int lapse = 150; // ?

void setup() {
  // init parser
  parser.setListener(&kparser);

  // init button pin & interrupt
  pinMode(interruptPinF, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPinF), fBoton, RISING); // detect button release
  pinMode(interruptPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPinB), bBoton, RISING); // detect button release

  // init neopixel
  strip.begin();

  rainbow(5, PEGS);
  colorStrip(strip.Color(0,0,0), 1);

  // init softserial: HM-10 BLE connection
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBleConnection();
  
  // init serial debugging
  Serial.begin(9600);
  Serial.println("KNIT IT WITH OTHERS alpha");
}

void loop() {

  char recvChar;
  int jsonSize = 100;

//  //  receive pattern, parse json file
//  if (BLE.available() > 0) { 
//    char json[100] = {};
//    int i = 0;
//    while (BLE.available()) {
//      recvChar = BLE.read();
//      //Serial.print(recvChar);
//      json[i++] = recvChar;
//    }

    //char json[] = "{\"a\":3, \"b\":{\"c\":\"d\"}}";
    char json[] = "{\"id\":777, \"type\":0, \"colors\":[16711680, 0],  \"size\":{\"width\":8, \"length\":4}, \"pattern\":[16, 80, 144, 208, 272, 336, 400, 464, 16, 80, 144, 208, 272]}";

    
    // results[index] = '\0';
    for (int i = 0; i < sizeof(json); i++) {
      parser.parse(json[i]);
    }
    
    for (int i = 0; i < sizeof(json); i++) {
      //EEPROM.update(address, val); //WRITE
      //address == EEPROM.length() // check position
    }
    
//  }

  if (buttonPressed) {
    if (step >= PEGS) {
      Serial.println("end of pattern");
      colorStrip(strip.Color(255, 0, 0), 3000);
      step = 0;
    }
    else if (step < 0) { // avoid underflowing
      step = 0;
    }

    //stepPattern(step, pattern);

    buttonPressed = LOW;
  }
}

void fBoton() {
  Serial.println("next");
  buttonPressed = HIGH;
  step++;
}

void bBoton() {
  Serial.println("prev");
  buttonPressed = HIGH;
  step--;
}


