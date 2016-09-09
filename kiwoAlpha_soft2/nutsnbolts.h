#pragma once
#include "config.h"

//
void eepromRead() {
  int address = 0;
  while ( address < EEPROM.length() )  {
    Serial.print(address);
    Serial.print("\t\t");
    Serial.println(EEPROM.read(address++));
  }
  Serial.println("done");
}
//---------
// button interrupts
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

//---------
// BLE
void setupBleConnection()
{
  BLE.begin(9600); //Set BLE BaudRate to default baud rate 9600
  BLE.print("AT+CLEAR"); //clear all previous setting
  BLE.print("AT+ROLE0"); //set the bluetooth name as a slaver
  BLE.print("AT+SAVE1");  //don't save the connect information
}

