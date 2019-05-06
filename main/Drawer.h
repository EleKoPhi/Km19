#ifndef Drawer_h
#define Drawer_h

#include "Helper.h"
#include <U8g2lib.h>

class Drawer
{
  public:
    Drawer(int clk, int data);
    void DrawMain();
    void DrawErr(bool sdStatus, bool nfcStatus, bool rtcStatus);
    void DrawDes(string user);
    void DrawSplitQ();
    void DrawUnknown();
    void DisplayProgress(int progress);
    void Clear();
    void DrawCenter(string txt, int y);
	void DrawLastUser(string lastUser);
	void DrawKaffeeKing(string King);
	void DrawCurrentAmount(int amount);
  private:
    U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C _display;
};

#endif
