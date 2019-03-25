#include "Arduino.h"
#include "Drawer.h"
#include <U8g2lib.h>

Drawer::Drawer(int clk, int data): _display(U8G2_R0, clk, data, U8X8_PIN_NONE)
{
  _display.begin();
}

void Drawer::DrawMain()
{
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB14_tr);
  this->DrawCenter("UX-ES-2",14);
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Mit Karte identifizieren",32);
  _display.sendBuffer();
}

void Drawer::Clear()
{
  _display.clearBuffer();
  _display.sendBuffer();
}

void Drawer::DrawErr(bool sdStatus, bool nfcStatus, bool rtcStatus)
{
  if (!sdStatus || !nfcStatus || !rtcStatus)
  {
    _display.clearBuffer();
    _display.setFont(u8g2_font_ncenB10_tr);
    _display.drawStr(0, 14, "System ERROR!");
    _display.sendBuffer();
    while (true);
  }
}

void Drawer::DrawDes(String user)
{
  int pos = (128 - _display.getStrWidth(user.c_str())) / 2;

  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB12_tr);

  _display.drawStr(pos, 12, user.c_str());

  _display.setFont(u8g2_font_ncenB08_tr);
  _display.drawStr(0, 24, "<-");
  _display.drawStr(128 - _display.getStrWidth("->"), 24, "->");

  pos = (128 - _display.getStrWidth("Bitte waehlen")) / 2;

  _display.setFont(u8g2_font_ncenB08_tr);
  _display.drawStr(pos, 32, "Bitte waehlen");
  _display.sendBuffer();
}

void Drawer::DrawSplitQ()
{
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Zweifach",8);
  this->DrawCenter("Weiter per Knopfdruck",20);
  this->DrawCenter("Ident Zweitnutzer",32);
  
  _display.sendBuffer();
}

void Drawer::DisplayProgress(int progress)
{
 
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB10_tr);
  this->DrawCenter("Zubereitung",14);

  for(int i=0;i<=progress/10;i++)
  {
    _display.drawBox(i*10+1+3*i, 21, 10, 10);
  }
  _display.sendBuffer();
}

void Drawer::DrawUnknown()
{
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Unbekannter Benutzer!",8);
  this->DrawCenter("Registrierung bei...",20);
  this->DrawCenter("Werner Schunn",32);
  _display.sendBuffer();
}

void Drawer::DrawCenter(String txt, int y)
{
  char pos = (128 - _display.getStrWidth(txt.c_str()))/2;
  _display.drawStr(pos, y, txt.c_str());
}
