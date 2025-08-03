#include "Display.h"

#include <Arduino.h>
#include <cstdio>


Display::Display(Status& status) : status(status) {}

const uint8_t TFT_DC = 9;
const uint8_t TFT_CS = 10;
Adafruit_HX8357 gfx = Adafruit_HX8357(TFT_CS, TFT_DC);

DisplayNumber xAxis(gfx);
DisplayNumber yAxis(gfx);
DisplayNumber zAxis(gfx);

void Display::begin() {
    gfx.begin();
    gfx.setRotation(1);
    gfx.fillScreen(0);
    gfx.setFont(&FreeMonoBold24pt7b);
    gfx.fillScreen(HX8357_BLACK);
    
    xAxis.begin(&FreeMonoBold24pt7b);
    yAxis.begin(&FreeMonoBold24pt7b);
    zAxis.begin(&FreeMonoBold24pt7b);

    //Right align and stack x, y, & z
    xAxis.setPosition(gfx.width() - yAxis.w()-10, 0);
    yAxis.setPosition(gfx.width() - yAxis.w()-10, xAxis.h() + 20);
    zAxis.setPosition(gfx.width() - zAxis.w()-10, xAxis.h() + yAxis.h() + 40);

    num = 1.234;
    xAxis.draw(num, HX8357_RED, false);

    num = 5.321;
    yAxis.draw(num);

    num = 11.707;
    zAxis.draw(num);
}


void Display::printLine(uint8_t line, const std::string& text) {
    // Replace with actual LCD logic
}

void Display::update() {
   // uint32_t uptime = millis() - status.getBootTime();
    char buf[32];
    //snprintf(buf, sizeof(buf), "Uptime: %lus", uptime / 1000);
    printLine(0, buf);
}
