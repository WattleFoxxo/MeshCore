#pragma once

#include "DisplayDriver.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>
#include <helpers/RefCountedDigitalPin.h>

#define MEMORY_BLACK 0
#define MEMORY_WHITE 1

class MemoryDisplay : public DisplayDriver {
  Adafruit_SharpMem display;
  bool _isOn;
  uint8_t _color;
  RefCountedDigitalPin* _peripher_power;

  bool i2c_probe(TwoWire& wire, uint8_t addr);
public:
#ifdef USE_PIN_MEMORY
MemoryDisplay(RefCountedDigitalPin* peripher_power=NULL) : DisplayDriver(128, 64), 
      display(PIN_MEMORY_SCL, PIN_MEMORY_SDA, PIN_MEMORY_CS, 400, 240), // 400x240 Memory in pixel display
      _peripher_power(peripher_power)
  {
    _isOn = false;
  }
#else
MemoryDisplay(RefCountedDigitalPin* peripher_power=NULL) : DisplayDriver(128, 64),
      display(&SPI, PIN_MEMORY_CS, 400, 240, 100000), // 400x240 Memory in pixel display
      _peripher_power(peripher_power)
  {
    _isOn = false;
  }
#endif
  bool begin();

  bool isOn() override { return _isOn; }
  void turnOn() override;
  void turnOff() override;
  void clear() override;
  void startFrame(Color bkg = DARK) override;
  void setTextSize(int sz) override;
  void setColor(Color c) override;
  void setCursor(int x, int y) override;
  void print(const char* str) override;
  void fillRect(int x, int y, int w, int h) override;
  void drawRect(int x, int y, int w, int h) override;
  void drawXbm(int x, int y, const uint8_t* bits, int w, int h) override;
  uint16_t getTextWidth(const char* str) override;
  void endFrame() override;
};
