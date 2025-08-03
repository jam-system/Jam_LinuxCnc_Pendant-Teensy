#include <Arduino.h>
#include "SerialFramer.h"
//#include "KeyboardHandler.h"
#include "Status.h"
#include "Display.h"
#include "Controller.h"
#include "ButtonHandler.h"
#include "EncoderHandler.h"

SerialFramer framer(Serial5);
//KeyboardHandler keyboard;
Status status;
Display display(status);
ButtonHandler buttons;
EncoderHandler encoder(5, 4); 

Controller controller(framer, status, display, buttons, encoder);

void setup() {
    Serial.begin(115200);
    Serial5.begin(115200);
    controller.begin();
}

void loop() {
    controller.update();
}
