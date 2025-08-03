#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

#include <Arduino.h>
#include <Keypad.h>

enum KeyEventType {
    KEY_EVENT_NONE,
    KEY_EVENT_PRESSED,
    KEY_EVENT_HOLD,
    KEY_EVENT_RELEASED
};

struct KeyEvent {
    char key = 0;
    KeyEventType type = KEY_EVENT_NONE;
};

class KeyboardHandler {
public:
    void begin();
    bool update(KeyEvent &event);  // Returns true if a key event occurred
};

#endif
