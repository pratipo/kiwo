#include <Adafruit_NeoPixel.h>
const byte neopin = 6;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(18 , neopin, NEO_GRB + NEO_KHZ800);
const byte interruptPin = 2;
volatile byte buttonPressed = HIGH;
///
const int pegs = 6;
////
char patterns[3][pegs] = {
  {'e', 'u', 'e', 'e', 'p', 'e' },
  {'u', 'u', 'u', 'u', 'u', 'u'  },
  {'p', 'p', 'p', 'p', 'u', 'u'  }
};
int pattern = 0;
int patternStep = -1;
int lapse = 150;

void setup() {
  // init neopixel
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // init button pin
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), boton, FALLING);

  Serial.begin(9600);
  Serial.println("knit it with other POC1");
}

void loop() {
  while (buttonPressed) {
  }
  Serial.println("next");
  patternStep++;
  if (patternStep >= pegs)
    patternStep = 0;

  switch (patterns[pattern][patternStep]) {
    case 'e':
      ewrap();
      break;
    case 'u':
      uwrap();
      break;
    case 'p':
      purl();
      break;
  }
  buttonPressed = HIGH;
}

void ewrap() {
  Serial.println("ewrap");
  //use a nested for loop to initialize all the pins
  int pixel = patternStep * 3;
  for (int s = pixel; s <= pixel + 2; s++) {
    strip.setPixelColor(s, strip.Color(0, 0, 128));
    strip.show();
    delay(lapse);
    clearStrip();
  }
  delay(lapse);
  strip.setPixelColor(pixel + 1, strip.Color(200, 0, 0));
  strip.show();
  delay(lapse);
  clearStrip();
}

void uwrap() {
  Serial.println("uwrap");
  //use a nested for loop to initialize all the pins
  int pixel = patternStep * 3;
  for (int s = pixel; s <= pixel + 2; s++) {
    strip.setPixelColor(s, strip.Color(0, 128, 0));
    strip.show();
    delay(lapse);
    clearStrip();
  }
  delay(lapse);
  strip.setPixelColor(pixel + 1, strip.Color(200, 0, 0));
  strip.show();
  delay(lapse);
  clearStrip();
}

void purl() {
  Serial.println("purl");
  //use a nested for loop to initialize all the pins
  int pixel = patternStep * 3;
  for (int s = pixel; s <= pixel + 2; s++) {
    strip.setPixelColor(s, strip.Color(128, 128, 0));
    strip.show();
    delay(lapse);
    clearStrip();
  }
  delay(lapse);
  strip.setPixelColor(pixel + 1, strip.Color(200, 0, 0));
  strip.show();
  delay(lapse);
  clearStrip();
}


// Fill the dots one after the other with a color
void clearStrip() {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, 0);
  }
  strip.show();
}

void boton() {
  buttonPressed = LOW;
  Serial.println("pressed");
}

