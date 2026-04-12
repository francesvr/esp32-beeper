// draw.h
#pragma once

#include <Adafruit_SSD1306.h>
#include "config.h"

extern const char upper_alph[];
extern const char lower_alph[];
extern const char num_sym[];

float chToFreq(int channel);

void drawScrollBarCursor(int itemCount, int index, int scroll_bar_x, int scroll_bar_start, int scroll_bar_end);

void drawMenuLayout();
void drawMenuItems();

void drawBackButton();

void drawMsgScreen();
void drawSendAlertScreen();
void drawInboxScreen();
void drawRSSIScreen();
void drawQuickSendScreen();

void drawKeyboard();
void drawMsgTyped();

void drawMsgReceived();
void drawMsgSending();
void drawAlertReceived();
