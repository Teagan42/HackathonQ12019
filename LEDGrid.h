#ifndef __LED_GRID_MAPPER_H__
#define __LED_GRID_H__

#include<FastLED.h>

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define WIDTH 8
#define HEIGHT 8

class LEDGrid {
public:
  LEDGrid(int width, int height, bool serpentine, CRGB* leds);
  void setPixel(int x, int y, CRGB color);
  void setPixels(CRGB colors[WIDTH][HEIGHT]);
  
private:
  CRGB* leds;
  int ledCount;
  int width, height;
  bool serpentine;
  int XY(int x, int y);
  int safeXY(int x, int y);
};

#endif
