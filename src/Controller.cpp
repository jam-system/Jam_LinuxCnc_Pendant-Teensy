// Controller.cpp
#include "Controller.h"
#include "ButtonHandler.h"
#include "EncoderHandler.h"
#include <Arduino.h>

Controller::Controller(SerialFramer& framer, Status& status, Display& display, ButtonHandler& buttons, EncoderHandler& encoder)
    : framer(framer), status(status), display(display), buttons(buttons), encoder(encoder) {}

void Controller::begin() {
    status.begin();
    display.begin();
    framer.begin(115200);
    buttons.begin(this);
    encoder.begin();

    Serial.println("Controller initialized.");
}

void Controller::update() {
    buttons.update();
    encoder.update();  // must be called every loop

    
    if (framer.poll(msg))
    {
        Serial.print("Received Cmd: ");
        Serial.print(msg.command);
        Serial.print(" Param: ");
        Serial.print(msg.parameter);
        Serial.print(" Data: ");
        Serial.println(msg.data.c_str());
    }
    int8_t step = encoder.poll();
    if (step != 0) {
        Serial.print("Encoder moved: ");
        Serial.println(step);
    }
}

void Controller::handleButtonPress(ButtonID id) {
    Serial.print("Pressed: ");
    Serial.println(ButtonHandler::getName(id));
}

void Controller::handleButtonHold(ButtonID id) {
    Serial.print("Held: ");
    Serial.println(ButtonHandler::getName(id));
}

void Controller::handleButtonRelease(ButtonID id) {
    Serial.print("Released: ");
    Serial.println(ButtonHandler::getName(id));
}
