#pragma once
#include <EncoderButton.h>

class EncoderHandler {
public:
    EncoderHandler(uint8_t pinA, uint8_t pinB);
    void begin();
    void update();

    int8_t poll();              // encoder: return -1, 0, or +1
    uint8_t clickCount();       // button: return number of clicks
    bool isClicked();           // button: true if clicked
    void resetClickCount();

private:
    EncoderButton encoder;
    int32_t lastPosition = 0;
    int8_t lastIncrement = 0;

};