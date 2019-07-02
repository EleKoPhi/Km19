#ifndef Drawer_h
#define Drawer_h

#include "Arduino.h"
#include <U8g2lib.h>

class Drawer
{
  public:
    Drawer(int clk, int data);
    void DrawMain();
    void DrawErr(bool sdStatus, bool nfcStatus, bool rtcStatus);
    void DrawDes(String user);
    void DrawSplitQ();
    void DrawUnknown();
    void DisplayProgress(int progress);
    void Clear();
    void DrawCenter(String txt, int y);
	void DrawLastUser(String lastUser);
	void DrawKaffeeKing(String King);
	void DrawCurrentAmount(int amount);
  void DrawWaitForUser();
  void DrawPayOne();
  void DrawSplitQ2();
  void DrawPay2 ();
  void DrawPay2_1();
  void DrawPay2_2();
  void DrawLowCredit();
  void DrawCredit(int ID, int Credit);
  void Err();
  
  
  private:
    U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C _display;
};

#endif