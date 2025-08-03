// ButtonHandler.h
#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Bounce2.h>

class Controller;

enum class ButtonID : uint8_t {
    Play,
    Stop,
    Shift,
    Up,
    Down,
    Left,
    Right,
    Select,
    Cancel,
    Menu,
    Count
};

class ButtonHandler {
public:
    ButtonHandler();

    void begin(Controller* owner);
    void update();

    static const int NUM_BUTTONS = static_cast<int>(ButtonID::Count);
    static const char* getName(ButtonID id);

private:
    struct Button {
        ButtonID id;
        int pin;
        Bounce debouncer;
        unsigned long pressStart = 0;
        bool holdTriggered = false;
    };

    Button buttons[NUM_BUTTONS];
    Controller* owner = nullptr;
    unsigned long holdTime = 1000;

    void handlePress(ButtonID id);
    void handleHold(ButtonID id);
    void handleRelease(ButtonID id);
};

#endif // BUTTON_HANDLER_H
