#include "EncoderHandler.h"

EncoderHandler::EncoderHandler(uint8_t pinA, uint8_t pinB)
    : encoder(pinA, pinB) {}

void EncoderHandler::begin() {
    //encoder.begin();
}

void EncoderHandler::update() {
    encoder.setRateLimit(20);
    encoder.update();  // required
    int32_t currentPosition = encoder.position();
    lastIncrement = currentPosition - lastPosition;
    lastPosition = currentPosition;
}


int8_t EncoderHandler::poll() {
    int8_t result = lastIncrement;
    lastIncrement = 0;
    return result;
}

uint8_t EncoderHandler::clickCount() {
    return encoder.clickCount();
}

bool EncoderHandler::isClicked() {
   // return encoder.clicked();
}

void EncoderHandler::resetClickCount() {
   // encoder.resetClickCount();
}
