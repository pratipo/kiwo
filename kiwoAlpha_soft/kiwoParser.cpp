#include "kiwoParser.h"
#include "JsonListener.h"


void kiwoParser::whitespace(char c) {
  Serial.println("whitespace");
}

void kiwoParser::startDocument() {
  Serial.println("start document");
}

void kiwoParser::key(String key) {
  Serial.println("key: " + key);
}

void kiwoParser::value(String value) {
  Serial.println("value: " + value);
}

void kiwoParser::endArray() {
  Serial.println("end array. ");
}

void kiwoParser::endObject() {
  Serial.println("end object. ");
}

void kiwoParser::endDocument() {
  Serial.println("end document. ");
}

void kiwoParser::startArray() {
   Serial.println("start array. ");
}

void kiwoParser::startObject() {
   Serial.println("start object. ");
}

