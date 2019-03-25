#include <Arduino.h>
#include "defines.h"
#include "Controller.h"

Controller MillController(sd_CS_pin, nfc_SS_pin, nfc_RS_pin,display_CLK_pin, display_DATA_pin);

float timeatstart = 0;
float deltatime = 0;
long CurrentUser = 0;
long CurrentUser_buffer = 0;

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  MillController.Begin();
}

void loop()
{
  MillController.UpDateTime();
  MillController.States(MillController.StateTransitions()); 
}
