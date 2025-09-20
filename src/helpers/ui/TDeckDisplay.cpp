#include "TDeckDisplay.h"

#ifndef DISPLAY_ROTATION
  #define DISPLAY_ROTATION 3
#endif

#define SCALE_X 1.33f // 320 / 128
#define SCALE_Y 1.77f // 240 / 64

bool TDeckDisplay::i2c_probe(TwoWire& wire, uint8_t addr) {
  return true;
}

bool TDeckDisplay::begin() {
  if (!_isOn) {
    if (_peripher_power) _peripher_power->claim();

    pinMode(PIN_TFT_LEDA_CTL, OUTPUT);
    digitalWrite(PIN_TFT_LEDA_CTL, HIGH);
    digitalWrite(PIN_TFT_RST, HIGH);

    spiTFT.begin(PIN_TFT_SCL, -1, PIN_TFT_SDA, PIN_TFT_CS);

    display.init(240, 320);
    display.setRotation(DISPLAY_ROTATION);

    display.setSPISpeed(40e6);

    display.fillScreen(ST77XX_BLACK);
    display.setTextColor(ST77XX_WHITE);
    display.setTextSize(2); 
    display.cp437(true); // Use full 256 char 'Code Page 437' font
  
    _isOn = true;
  }
  return true;
}

void TDeckDisplay::turnOn() {
  TDeckDisplay::begin();
}

void TDeckDisplay::turnOff() {
  if (_isOn) {
    digitalWrite(PIN_TFT_LEDA_CTL, HIGH);
    digitalWrite(PIN_TFT_RST, LOW);
    digitalWrite(PIN_TFT_LEDA_CTL, LOW);
    _isOn = false;

    if (_peripher_power) _peripher_power->release();
  }
}

void TDeckDisplay::clear() {
  //Serial.println("DBG: display.Clear");
  display.fillScreen(ST77XX_BLACK);
}

void TDeckDisplay::startFrame(Color bkg) {
  display.fillScreen(0x00);
  display.setTextColor(ST77XX_WHITE);
  display.setTextSize(1); // This one affects size of Please wait... message
  display.cp437(true); // Use full 256 char 'Code Page 437' font
}

void TDeckDisplay::setTextSize(int sz) {
  display.setTextSize(sz);
}

void TDeckDisplay::setColor(Color c) {
  switch (c) {
    case DisplayDriver::DARK :
      _color = ST77XX_BLACK;
      break;
    case DisplayDriver::LIGHT : 
      _color = ST77XX_WHITE;
      break;
    case DisplayDriver::RED : 
      _color = ST77XX_RED;
      break;
    case DisplayDriver::GREEN : 
      _color = ST77XX_GREEN;
      break;
    case DisplayDriver::BLUE : 
      _color = ST77XX_BLUE;
      break;
    case DisplayDriver::YELLOW : 
      _color = ST77XX_YELLOW;
      break;
    case DisplayDriver::ORANGE : 
      _color = ST77XX_ORANGE;
      break;
    default:
      _color = ST77XX_WHITE;
      break;
  }
  display.setTextColor(_color);
}

void TDeckDisplay::setCursor(int x, int y) {
  display.setCursor(x*SCALE_X, y*SCALE_Y);
}

void TDeckDisplay::print(const char* str) {
  display.print(str);
}

void TDeckDisplay::fillRect(int x, int y, int w, int h) {
  display.fillRect(x*SCALE_X, y*SCALE_Y, w*SCALE_X, h*SCALE_Y, _color);
}

void TDeckDisplay::drawRect(int x, int y, int w, int h) {
  display.drawRect(x*SCALE_X, y*SCALE_Y, w*SCALE_X, h*SCALE_Y, _color);
}

void TDeckDisplay::drawXbm(int x, int y, const uint8_t* bits, int w, int h) {
  display.drawBitmap(x*SCALE_X, y*SCALE_Y, bits, w, h, _color);
}

uint16_t TDeckDisplay::getTextWidth(const char* str) {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(str, 0, 0, &x1, &y1, &w, &h);
  return w / SCALE_X;
}

void TDeckDisplay::endFrame() {
  // display.display();
}
