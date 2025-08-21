#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

#include <Bounce2.h>
#include <functional>

enum class ButtonID : uint8_t {
    Home,
    zAxis,
    yAxis,
    xAxis,
    Jog_01,
    Jog_10,
    Jog_1,
    Spare_3,
    Spare_4,
    Spare_5,
    Spare_6,
    Spare_7,
    Spare_8
    };

class ButtonHandler {
public:
    ButtonHandler();

    void begin();
    void update();

    static const int NUM_BUTTONS = 13; //static_cast<int>(ButtonID::Count);
    static const char* getName(ButtonID id);

    // User-defined callback functions
    std::function<void(ButtonID)> onPress;
    std::function<void(ButtonID)> onHold;
    std::function<void(ButtonID)> onRelease;

private:
    struct Button {
        ButtonID id;
        int pin;
        Bounce debouncer;
        unsigned long pressStart = 0;
        bool holdTriggered = false;
    };

    Button buttons[NUM_BUTTONS];
    unsigned long holdTime = 1000;

    void handlePress(ButtonID id);
    void handleHold(ButtonID id);
    void handleRelease(ButtonID id);
};

#endif // BUTTON_HANDLER_H
