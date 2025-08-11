// Controller.cpp
#include "Controller.h"
#include "ButtonHandler.h"
#include "EncoderHandler.h"
//#include "Display.h"
#include <Arduino.h>

Controller::Controller( Adafruit_ILI9341& d, SerialFramer& framer,  ButtonHandler& buttons, EncoderHandler& encoder)
    : tft(d), framer(framer) , buttons(buttons), encoder(encoder) {}

void Controller::begin() {
   // status.begin();
    //display.begin();
    
    framer.begin(115200);
    buttons.begin(this);
    encoder.begin();

    printf("Controller begin\n");
    /**
    Declare your DisplayNumber(s)
    */
    
   
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  /**
     Call begin() *after* tft.begin() for each DisplayNumber to set the font
     (before using other methods)
  */
 
  /**
     You can set the posi
  */

    DisplayNumber xAxis(tft);
    DisplayNumber yAxis(tft);
    DisplayNumber zAxis(tft);
    DisplayNumber aAxis(tft);


 printf("Basic DisplayNumber\n");
 

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

        switch(msg.command) {
            case 'P':
            //updateDisplay();
                //display.update(msg.parameter, msg.data);
                break;
            case 'B':
                Serial.println("Command B received");
                break;
            default:
                Serial.println("Unknown command");
        }
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

void Controller::updateDisplay() {
    // Update the display based on the current state
    // This is where you would implement your display logic
    // For example, you could update the DisplayNumber instances here
    Serial.println("Updating display...");
    
    // Example: Update xAxis with a new value
    static float xValue = 0.0f;
    xValue += 1.0f; // Increment for demonstration
    DisplayNumber xAxis(tft);
    xAxis.draw(xValue);
    
    // Add more display updates as needed
}