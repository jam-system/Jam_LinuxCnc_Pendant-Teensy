#pragma once

//#include "Status.h"
#include "ButtonHandler.h"
#include "SerialFramer.h"
//#include "Display.h"
#include "EncoderHandler.h"
#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <DisplayUtils.h>

// For the Adafruit shield, these are the default.
#define TFT_DC 9
#define TFT_CS 10

class Status;
class Display;

class Controller {
public:
  Controller( Adafruit_ILI9341& tft, SerialFramer& framer,   ButtonHandler& buttons, EncoderHandler& encoder);

  void begin();
  void update();

  void handleButtonPress(ButtonID id);
  void handleButtonHold(ButtonID id);
  void handleButtonRelease(ButtonID id);

private:
  Adafruit_ILI9341& tft;
  SerialFramer& framer;
  //Status& status;
  //Display& display;
  ButtonHandler& buttons;
  EncoderHandler& encoder;
  SerialFramer::Message msg;
  //Adafruit_ILI9341& tft;
   void updateDisplay();
  


   
};


