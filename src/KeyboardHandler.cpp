#include "KeyboardHandler.h"

// === Matrix configuration ===
const byte ROWS = 2;
const byte COLS = 4;

// === Teensy pin configuration ===
const byte rowPins[ROWS] = {36, 37};
const byte colPins[COLS] = {22, 17, 16, 15};

// Key map
char keymap[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'}
  
};

Keypad keypad(makeKeymap(keymap), rowPins, colPins, ROWS, COLS);

void KeyboardHandler::begin() {
    Serial.println("KeyboardHandler initialized.");
}

bool KeyboardHandler::update(KeyEvent &event) {
    if (keypad.getKeys()) {
        for (int i = 0; i < LIST_MAX; i++) {
            Key &k = keypad.key[i];
            if (k.kstate == IDLE) continue;
            event.key = k.kchar;
            switch (k.kstate) {
                case PRESSED:   event.type = KEY_EVENT_PRESSED; break;
                case HOLD:      event.type = KEY_EVENT_HOLD; break;
                case RELEASED:  event.type = KEY_EVENT_RELEASED; break;
                default:        event.type = KEY_EVENT_NONE; break;
            }
            return true;
        }
    }
    event.type = KEY_EVENT_NONE;
    return false;
}
