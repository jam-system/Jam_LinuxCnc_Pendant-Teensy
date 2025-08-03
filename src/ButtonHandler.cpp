// ButtonHandler.cpp
#include "ButtonHandler.h"
#include "Controller.h"
#include <Arduino.h>

static const int PIN_MAP[ButtonHandler::NUM_BUTTONS] = {
    19, 18, 17, 16, 15, 7, 8, 9, 10, 11
};

ButtonHandler::ButtonHandler() {
    for (int i = 0; i < NUM_BUTTONS; ++i) {
        buttons[i].id = static_cast<ButtonID>(i);
        buttons[i].pin = PIN_MAP[i];
    }
}

void ButtonHandler::begin(Controller* owner) {
    this->owner = owner;
    for (int i = 0; i < NUM_BUTTONS; ++i) {
        pinMode(buttons[i].pin, INPUT_PULLUP);
        buttons[i].debouncer.attach(buttons[i].pin);
        buttons[i].debouncer.interval(10);
    }
}

void ButtonHandler::update() {
    unsigned long now = millis();
    for (int i = 0; i < NUM_BUTTONS; ++i) {
        Button& btn = buttons[i];
        btn.debouncer.update();

        if (btn.debouncer.fell()) {
            btn.pressStart = now;
            btn.holdTriggered = false;
            handlePress(btn.id);
        }

        if (!btn.holdTriggered && btn.debouncer.read() == LOW &&
            now - btn.pressStart >= holdTime) {
            btn.holdTriggered = true;
            handleHold(btn.id);
        }

        if (btn.debouncer.rose()) {
            if (!btn.holdTriggered) {
                handleRelease(btn.id);
            }
        }
    }
}

void ButtonHandler::handlePress(ButtonID id) {
    if (owner) owner->handleButtonPress(id);
}

void ButtonHandler::handleHold(ButtonID id) {
    if (owner) owner->handleButtonHold(id);
}

void ButtonHandler::handleRelease(ButtonID id) {
    if (owner) owner->handleButtonRelease(id);
}

const char* ButtonHandler::getName(ButtonID id) {
    switch (id) {
        case ButtonID::Play:   return "Play";
        case ButtonID::Stop:   return "Stop";
        case ButtonID::Shift:  return "Shift";
        case ButtonID::Up:     return "Up";
        case ButtonID::Down:   return "Down";
        case ButtonID::Left:   return "Left";
        case ButtonID::Right:  return "Right";
        case ButtonID::Select: return "Select";
        case ButtonID::Cancel: return "Cancel";
        case ButtonID::Menu:   return "Menu";
        default:               return "Unknown";
    }
}
