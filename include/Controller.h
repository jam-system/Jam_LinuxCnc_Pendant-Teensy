#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Status.h"
#include "ButtonHandler.h"
#include "SerialFramer.h"
#include "Display.h"
#include "EncoderHandler.h"
#include <Arduino.h>

class Status;
class Display;

class Controller {
public:
  Controller(SerialFramer& framer, Status& status, Display& display, ButtonHandler& buttons, EncoderHandler& encoder);

  void begin();
  void update();

  void handleButtonPress(ButtonID id);
  void handleButtonHold(ButtonID id);
  void handleButtonRelease(ButtonID id);

private:
  SerialFramer& framer;
  Status& status;
  Display& display;
  ButtonHandler& buttons;
  EncoderHandler& encoder;
  SerialFramer::Message msg;
  
};

#endif // CONTROLLER_H
