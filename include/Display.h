#pragma once
#include <string>
#include "Status.h"
#include <Adafruit_GFX.h>
#include <Adafruit_HX8357.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <DisplayUtils.h>

class Display {
public:
    Display(Status& status);

    void begin();
    void update();
    void printLine(uint8_t line, const std::string& text);

private:
    Status& status;
    uint32_t lastUpdate = 0;
    float num = 0;
  
   
};
