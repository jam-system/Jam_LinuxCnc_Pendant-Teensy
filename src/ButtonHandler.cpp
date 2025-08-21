#include "ButtonHandler.h"
#include <Arduino.h>

static const int PIN_MAP[ButtonHandler::NUM_BUTTONS] = {
    14, 15, 16, 17, 0, 1, 2, 3, 4, 5, 6, 7, 8
};

ButtonHandler::ButtonHandler() {
    for (int i = 0; i < NUM_BUTTONS; ++i) {
        buttons[i].id = static_cast<ButtonID>(i);
        buttons[i].pin = PIN_MAP[i];
    }
}

void ButtonHandler::begin() {
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
    if (onPress) onPress(id);
}

void ButtonHandler::handleHold(ButtonID id) {
    if (onHold) onHold(id);
}

void ButtonHandler::handleRelease(ButtonID id) {
    if (onRelease) onRelease(id);
}

const char* ButtonHandler::getName(ButtonID id) {
    switch (id) {
        case ButtonID::xAxis:  return "xAxis";
        case ButtonID::yAxis:  return "yAxis";
        case ButtonID::zAxis:  return "zAxis";
        case ButtonID::Home:   return "Home";
        case ButtonID::Jog_01: return "Jog_01";
        case ButtonID::Jog_10: return "Jog_10";
        case ButtonID::Jog_1:  return "Jog_1";
        case ButtonID::Spare_3: return "Spare_3";
        case ButtonID::Spare_4: return "Spare_4";
        case ButtonID::Spare_5:   return "Spare_5";
        case ButtonID::Spare_6:   return "Spare_6";
        case ButtonID::Spare_7:   return "Spare_7";
   
        default:               return "Unknown";
    }
}
