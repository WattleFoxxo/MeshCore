#include "MemoryDisplay.h"

#ifndef DISPLAY_ROTATION
  #define DISPLAY_ROTATION 2
#endif

#ifndef DISPLAY_SCALE_X
  #define DISPLAY_SCALE_X 3.125f  // 400 / 128
#endif

#ifndef DISPLAY_SCALE_Y
  #define DISPLAY_SCALE_Y 3.75f  // 240 / 64
#endif

bool MemoryDisplay::i2c_probe(TwoWire& wire, uint8_t addr) {
  return true;
}

bool MemoryDisplay::begin() {
  if (!_isOn) {
    if (_peripher_power) _peripher_power->claim();

    display.begin();
    display.clearDisplay();
    display.setRotation(DISPLAY_ROTATION);
    display.fillScreen(MEMORY_BLACK);
    display.setTextColor(MEMORY_WHITE);
    display.setTextSize(2); 
    display.cp437(true);         // Use full 256 char 'Code Page 437' font
  
    _isOn = true;
  }
  return true;
}

void MemoryDisplay::turnOn() {
  MemoryDisplay::begin();
}

void MemoryDisplay::turnOff() {
  if (_isOn) {
    _isOn = false;

    // Nothing to do, it's an always on display

    if (_peripher_power) _peripher_power->release();
  }
}

void MemoryDisplay::clear() {
  display.fillScreen(MEMORY_BLACK);
}

void MemoryDisplay::startFrame(Color bkg) {
  display.fillScreen(MEMORY_BLACK);
  display.setTextColor(MEMORY_WHITE);
  display.setTextSize(1);      // This one affects size of Please wait... message
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
}

void MemoryDisplay::setTextSize(int sz) {
  display.setTextSize(sz);
}

void MemoryDisplay::setColor(Color c) {
  switch (c) {
    case DisplayDriver::DARK :
      _color = MEMORY_BLACK;
      break;
    default:
      _color = MEMORY_WHITE;
      break;
  }
  display.setTextColor(_color);
}

void MemoryDisplay::setCursor(int x, int y) {
  display.setCursor(x*DISPLAY_SCALE_X, y*DISPLAY_SCALE_Y);
}

void MemoryDisplay::print(const char* str) {
  display.print(str);
}

void MemoryDisplay::fillRect(int x, int y, int w, int h) {
  display.fillRect(x*DISPLAY_SCALE_X, y*DISPLAY_SCALE_Y, w*DISPLAY_SCALE_X, h*DISPLAY_SCALE_Y, _color);
}

void MemoryDisplay::drawRect(int x, int y, int w, int h) {
  display.drawRect(x*DISPLAY_SCALE_X, y*DISPLAY_SCALE_Y, w*DISPLAY_SCALE_X, h*DISPLAY_SCALE_Y, _color);
}

void MemoryDisplay::drawXbm(int x, int y, const uint8_t* bits, int w, int h) {
  display.drawBitmap(x*DISPLAY_SCALE_X, y*DISPLAY_SCALE_Y, bits, w, h, _color);
}

uint16_t MemoryDisplay::getTextWidth(const char* str) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(str, 0, 0, &x1, &y1, &w, &h);

  return w / DISPLAY_SCALE_X;
}

void MemoryDisplay::endFrame() {
  display.refresh();
}
