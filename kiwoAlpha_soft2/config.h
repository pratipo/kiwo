#pragma once

#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>   //Software Serial Port
#include <EEPROM.h>

//-------------------
#define RxD 4
#define TxD 5
//-------------------
#define NEOPIN 6
#define PEGS 26
//-------------------

SoftwareSerial BLE(RxD, TxD);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PEGS , NEOPIN, NEO_GRB + NEO_KHZ800);

const byte interruptPinF = 2;
const byte interruptPinB = 3;

volatile byte buttonPressed = LOW;
volatile int step = 0;

/*
position: 0 hB
          1 lB

id        2 hB
          3 lB

type      4 B

colors    5  B: R  first  YARN 1
          6  B: G
          7  B: B
          
          8  B: R  second
          9  B: G
          10 B: B
          
          11 B: R  third
          12 B: G
          13 B: B
          
          14 B: R  fourth
          15 B: G
          16 B: B

          17 B: R  first   YARN 2
          18 B: G
          19 B: B
          
          20 B: R  second
          21 B: G
          22 B: B
          
          23 B: R  third
          24 B: G
          25 B: B
          
          26 B: R  fourth
          27 B: G
          28 B: B

          ...              YARN 3
          ...              YARN 4

sizeW     29
sizeL     30

stitches  31 hB: stitch 0
          32 lB
          33 hB: stitch 1
          34 lB
          .....

          /hB/  a b c d e f g h  /lB/  i j k l m n o p
          [a b c d e f g h]   peg number: byte (0-255)
          [i j]             yarn selector
          [k l m]           stitch type: 4 bits (cast-on, cast off, e-wrap , u-wrap, purl, cable, increase, decrease, ... )
          [n o p]           extra: 3 bits (cable: target offset (from origin): [-4, +4])

*/

// eeprom offsets
int idEOffset = 2;
int typeEOffset = 4;
int colorsEOffset = 5;
int wEOffset = 29;
int lEOffset = 30;
int stitchEOffset = 31;

// stich bitwise offsets
byte pegOffset =  8;
byte yarnOffset =  6;
byte typeOffset =  3;
byte xOffset =  0;


//byte xtra(word stitch){
//  byte x = (stitch>>xOffset);
//  x = x&B00000111;
//  return x;
//}

byte type(word stitch){
  byte t = (stitch>>typeOffset);
  t = t&B00000111;
  return t;
}

byte yarn(word stitch){
  byte y = (stitch>>yarnOffset);
  y = y&B00000011;
  return y;
}

uint32_t colour(word stitch){
  byte y = yarn(stitch);
  byte t = type(stitch);

  byte r = EEPROM.read(colorsEOffset + y*4*3 + t*3); // get the indexed color
  byte g = EEPROM.read(colorsEOffset + y*4*3 + t*3+1); // get the indexed color
  byte b = EEPROM.read(colorsEOffset + y*4*3 + t*3+2); // get the indexed color

  uint32_t rgb = (uint32_t)r << 16 | (uint32_t)g << 8 | (uint32_t)b;;
  return rgb;
}

byte peg(word stitch){
  byte p = (stitch>>pegOffset);
  p = p&B11111111;
  return p;
}

int patternLength(){
  if (EEPROM.read(idEOffset) || EEPROM.read(idEOffset+1) ) { //pattern loaded ?
    int s = (int) EEPROM.read(wEOffset)*EEPROM.read(lEOffset);
    return s;
  }
  else {
    Serial.println("ERROR! no pattern loaded in eeprom!");
    return -1;
  }
}





    
