#include <SD.h>
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
	MillController = new Controller(sd_CS_pin, nfc_SS_pin, nfc_RS_pin, display_CLK_pin, display_DATA_pin);
  //Serial.begin(9600);
  //while (!Serial);
  MillController->Begin();
}

void loop()
{
  MillController->updateDeltaTime();
  MillController->States(MillController->updateStateTransitions());
}
