#include "LEDGrid.h"

LEDGrid::LEDGrid(int width, int height, bool serpentine, CRGB* leds) {
  this->width = width;
  this->height = height;
  this->serpentine = serpentine;
  this->ledCount = width * height;
  this->leds = leds;
}

void LEDGrid::setPixel(int x, int y, CRGB color) {
  this->leds[this->XY(x, y)] = color;
}

void LEDGrid::setPixels(CRGB colors[WIDTH][HEIGHT]) {
  for(int x=0; x < this->width; x++) {
    for(int y=0; y < this->height; y++) {
      this->setPixel(x, y, colors[x][y]);
    }
  }
}

int LEDGrid::XY(int x, int y) {
  int i;
  
  if( this->serpentine == false) {
    i = (y * this->width) + x;
  } else {
    if( y & 0x01) {
      // Odd rows run backwards
      int reverseX = (this->width - 1) - x;
      i = (y * this->width) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * this->width) + x;
    }
  }

  return i;
}

int LEDGrid::safeXY(int x, int y) {
  if (x >= this->width || y >= this->height) return -1;
  return this->XY(x, y);
}
