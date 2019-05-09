#include <WiFi101.h>
#include "FileIO.h"
#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include <MinimumSerial.h>
#include <FreeStack.h>
#include <BlockDriver.h>
#include <U8x8lib.h>
#include <U8g2lib.h>
#include <require_cpp11.h>
#include <MFRC522Extended.h>
#include <MFRC522.h>
#include <deprecated.h>
#include <Arduino.h>
#include "defines.h"
#include "Controller.h"

Controller* MillController;

void setup()
{
	MillController = new Controller();
	//Serial.begin(9600);
	//while (!Serial);
	MillController->Begin();
}

void loop()
{
	MillController->updateDeltaTime();
	MillController->States(MillController->updateStateTransitions());
}
