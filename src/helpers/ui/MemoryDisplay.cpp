#include "MemoryDisplay.h"

#ifndef DISPLAY_ROTATION
  #define DISPLAY_ROTATION 0
#endif

#ifndef DISPLAY_SCALE_X
  #define DISPLAY_SCALE_X 3.125f  // 400 / 128
#endif

#ifndef DISPLAY_SCALE_Y
  #define DISPLAY_SCALE_Y 3.75f  // 240 / 64
#endif

// #ifndef DISPLAY_PIXEL_SCALE
//   #define DISPLAY_PIXEL_SCALE 3.75f  // 240 / 64
// #endif

// #define PIXEL_SCALE round(DISPLAY_SCALE_X)

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
    display.setTextSize(2 * DISPLAY_SCALE_X); 
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

    display.clearDisplay();

    if (_peripher_power) _peripher_power->release();
  }
}

void MemoryDisplay::clear() {
  display.fillScreen(MEMORY_BLACK);
}

void MemoryDisplay::startFrame(Color bkg) {
  display.fillScreen(MEMORY_BLACK);
  display.setTextColor(MEMORY_WHITE);
  display.setTextSize(1 * DISPLAY_SCALE_X);      // This one affects size of Please wait... message
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
}

void MemoryDisplay::setTextSize(int sz) {
  display.setTextSize(sz * DISPLAY_SCALE_X);
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
  uint8_t byteWidth = (w + 7) / 8;

  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      uint8_t byte = bits[j * byteWidth + i / 8];
      bool pixelOn = byte & (0x80 >> (i & 7));

      if (pixelOn) {
        for (int dy = 0; dy < DISPLAY_SCALE_X; dy++) {
          for (int dx = 0; dx < DISPLAY_SCALE_X; dx++) {
            display.drawPixel(x * DISPLAY_SCALE_X + i * DISPLAY_SCALE_X + dx, y * DISPLAY_SCALE_Y + j * DISPLAY_SCALE_X + dy, _color);
          }
        }
      }
    }
  }
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
