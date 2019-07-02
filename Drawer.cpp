#include "Arduino.h"
#include "Drawer.h"
#include <U8g2lib.h>

Drawer::Drawer(int clk, int data): _display(U8G2_R0, clk, data, U8X8_PIN_NONE)
{
  _display.begin();
}

void Drawer::Clear()
{
	_display.clearBuffer();
	_display.sendBuffer();
}

void Drawer::DrawMain()
{
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB10_tr);
  this->DrawCenter("UX-ES-2",12);
  _display.setFont(u8g2_font_ncenB10_tr);
  this->DrawCenter("! Karte auflegen !",32);
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

void Drawer::DrawLastUser(String lastUser)
{
	_display.clearBuffer();
	_display.setFont(u8g2_font_ncenB08_tr);
	this->DrawCenter("Letzter Nutzer...", 8);
	_display.setFont(u8g2_font_ncenB12_tr);
	this->DrawCenter(lastUser, 30);
	_display.sendBuffer();
}

void Drawer::DrawDes(String user)
{
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB12_tr);
  this->DrawCenter(user, 12);
  _display.setFont(u8g2_font_ncenB08_tr);
  _display.drawStr(0, 24, "<-");
  _display.drawStr(128 - _display.getStrWidth("->"), 24, "->");
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Bitte waehlen", 32);
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
    _display.drawBox(i*13+1, 21, 10, 10);
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

void Drawer::DrawKaffeeKing(String King)
{
	_display.clearBuffer();
	_display.setFont(u8g2_font_ncenB08_tr);
	this->DrawCenter("Kaffee Koenig...", 8);
	_display.setFont(u8g2_font_ncenB12_tr);
	this->DrawCenter(King, 30);
	_display.sendBuffer();

}

void Drawer::DrawCurrentAmount(int amount)
{
	String a = String(amount, DEC);
	_display.clearBuffer();
	_display.setFont(u8g2_font_ncenB08_tr);
	this->DrawCenter("Anzahl Bezuege", 8);
	_display.setFont(u8g2_font_ncenB12_tr);
	this->DrawCenter(a, 30);
	_display.sendBuffer();
}

void Drawer::DrawWaitForUser()
{
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Bitte waehlen", 8);
  _display.setFont(u8g2_font_ncenB12_tr);
  this->DrawCenter("<- 1x  |  2x ->", 30);
  _display.sendBuffer(); 
}

void Drawer::DrawPayOne()
{
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Preis 1 Credit", 8);
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Bitte Karte auflegen", 30);
  _display.sendBuffer(); 
  
}

void Drawer::DrawSplitQ2()
{
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Split?", 8);
  _display.setFont(u8g2_font_ncenB12_tr);
  this->DrawCenter("<- Nein | Ja ->", 30);
  _display.sendBuffer(); 
}

 void Drawer::DrawPay2 ()
 {
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Preis 2 Credits", 8);
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Bitte Karte auflegen", 30);
  _display.sendBuffer(); 
  
 }
 
 void Drawer::DrawPay2_1()
 {
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Preis 1. von 2 Credits", 8);
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Bitte Karte auflegen", 30);
  _display.sendBuffer(); 
    
 }
 
 void Drawer::DrawPay2_2()
 {
 _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Preis 2. von 2 Credits", 8);
  _display.setFont(u8g2_font_ncenB08_tr);
  this->DrawCenter("Bitte Karte auflegen", 30);
  _display.sendBuffer(); 
 }

 void Drawer::DrawLowCredit()
 {
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB12_tr);
  this->DrawCenter("Kein Guthaben ", 12);
  _display.setFont(u8g2_font_ncenB12_tr);
  this->DrawCenter("mehr !", 30);
  _display.sendBuffer(); 
 }

 void Drawer::DrawCredit(int ID, int Credit)
 {
  _display.clearBuffer();
  _display.setFont(u8g2_font_ncenB10_tr);

  String IDs = "ID: " + String(ID);
  this->DrawCenter(IDs, 12);
  _display.setFont(u8g2_font_ncenB12_tr);
  String Cred = "Guthaben: " + String(Credit);
  this->DrawCenter(Cred, 30);
  _display.sendBuffer(); 
  
 }


 void Drawer::Drawer::Err()
  {
    _display.clearBuffer();
    _display.setFont(u8g2_font_ncenB10_tr);

    this->DrawCenter("Err", 15);

     _display.sendBuffer(); 
    
  }
