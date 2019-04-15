#include <Arduino.h>
#include "defines.h"
#include "Controller.h"

Controller* MillController;

void setup()
{
  MillController = new Controller(sd_CS_pin, nfc_SS_pin, nfc_RS_pin,display_CLK_pin, display_DATA_pin);
  Serial.begin(9600);
  //while (!Serial);
  MillController->Begin();
}

void loop()
{
  MillController->UpDateTime();
  MillController->States(MillController->StateTransitions()); 
}
