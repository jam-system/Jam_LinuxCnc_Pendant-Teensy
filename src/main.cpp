#include <Arduino.h>
#include "Adafruit_GFX.h"
#include "Adafruit_HX8357.h"
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <DisplayUtils.h>
#include "ButtonHandler.h"
#include "EncoderHandler.h"
#include "SerialFramer.h"
#include <iostream>
#include <string>

static constexpr uint8_t cs_tft  = 10;
static constexpr uint8_t dc_tft  = 9;
// Use hardware SPI
Adafruit_HX8357 tft = Adafruit_HX8357(cs_tft, dc_tft);

char currentAxis; //  none selected
float currentJogStep;
bool setupReady = false;


SerialFramer framer(Serial5);



ButtonHandler buttons;
EncoderHandler encoder(23, 22); 

//Controller controller(tft, framer, buttons, encoder);

/**
   Declare your DisplayNumber(s)
*/
    DisplayNumber xAxis(tft);
    DisplayNumber yAxis(tft);
    DisplayNumber zAxis(tft);
    DisplayNumber jogRate(tft);

void selectAxis(char axis);
void zeroAxis(char axis);
void processJogCommand(int8_t step);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial5.begin(115200);
    delay(200);

    framer.begin(115200);
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(HX8357_BLACK);
    /**
         Call begin() *after* tft.begin() for each DisplayNumber to set the font
        (before using other methods)
    */
    xAxis.begin(&FreeMonoBold24pt7b);
    yAxis.begin(&FreeMonoBold24pt7b);
    zAxis.begin(&FreeMonoBold24pt7b);
    //Set the position, font, size and precision
    jogRate.begin(&FreeMonoBold12pt7b);
    jogRate.setFormat(4, 2); //15 digits, 2 decimal places
    
    xAxis.setPosition(0,0);
    xAxis.setTextColor(HX8357_YELLOW);
    tft.setFont(&FreeMonoBold24pt7b);
    tft.setCursor(145,xAxis.h());
    tft.print("X");
    tft.setCursor(145,xAxis.h()+xAxis.h()+20);
    tft.print("Y");
    tft.setCursor(145,xAxis.h()+xAxis.h()+xAxis.h()+40);
    tft.print("Z");

    tft.setFont(&FreeMonoBold12pt7b);
    tft.setCursor(100,260);
    tft.print("Jog Step:");

    //Right align and stack x, y, & z
    xAxis.setPosition(tft.width() - yAxis.w()-10, 0);
    yAxis.setPosition(tft.width() - yAxis.w()-10, xAxis.h() + 20);
    zAxis.setPosition(tft.width() - zAxis.w()-10, xAxis.h() + yAxis.h() + 40);

    currentJogStep = 0.01; // default jog step
    jogRate.setPosition( 250, 250);

    jogRate.setTextColor(HX8357_CYAN);
    jogRate.draw(currentJogStep);
    
    buttons.onPress = [](ButtonID id) {
        Serial.print("Pressed: ");
        Serial.println(ButtonHandler::getName(id));
    };

    buttons.begin();
    currentAxis = '-1'; //  none selected
    currentJogStep = 0.01;
    delay(200);

    buttons.onHold = [](ButtonID id) {  
        if (!setupReady) return;
        switch (id) {
            case ButtonID::xAxis:
                zeroAxis('X');
                break;
            case ButtonID::yAxis:
                zeroAxis('Y');
                break;
            case ButtonID::zAxis:
                zeroAxis('Z');
                break;
            case ButtonID::Home:
                Serial.println("Home button pressed, sending home command");
                framer.sendMessage('H', '0', "");
            default:
                Serial.println("Hold on unknown button");
                break;
        }

        Serial.print("Held: ");
        Serial.println(ButtonHandler::getName(id));
    };

    buttons.onRelease = [](ButtonID id) {
        if (!setupReady) return; 
        Serial.print("Released: ");
        Serial.println(ButtonHandler::getName(id));
        switch (id) {
            case ButtonID::Home:
                break;
            case ButtonID::xAxis:
                selectAxis('X');
                break;
            case ButtonID::yAxis:
                selectAxis('Y');
                break;
            case ButtonID::zAxis:
                selectAxis('Z');
                break;

            case ButtonID::Jog_01:
                currentJogStep = 0.01;
                jogRate.draw(currentJogStep);
                break;
            case ButtonID::Jog_10:   
                currentJogStep = 0.1;
                jogRate.draw(currentJogStep);
                break;
            case ButtonID::Jog_1:
                currentJogStep = 1.0;
                jogRate.draw(currentJogStep);
                //framer.sendMessage('d', '-',"");

                break;
            default:
                Serial.println("Hold on unknown button");
                break;
        }

    };

    delay(500);
    buttons.update();
    setupReady = true;
}


SerialFramer::Message msg;
void processMessage(const SerialFramer::Message &msg);

void loop() {
    buttons.update();
    encoder.update();  // must be called every loop

    if (framer.poll(msg))
    { 
        processMessage(msg);
    }

    int8_t step = encoder.poll();
    if (step != 0) {
        Serial.print("Encoder moved: ");
        Serial.println(step);
        processJogCommand(step);
    }
}
void processMessage(const SerialFramer::Message &msg)
{
    printf("Rec: Cmd:%c Param:%c Data:%s\n", msg.command, msg.parameter, msg.data.c_str());
   // Serial.print("Received Cmd: ");

    switch (msg.command)
    {
    case 'P': {
     //   Serial.println("Command P received");
        float value = std::stof(msg.data.c_str());
        if (msg.parameter == '0')  xAxis.draw(value);
        else if (msg.parameter == '1')  yAxis.draw(value);
        else if (msg.parameter == '2') zAxis.draw(value);
        break;
        }
    case 'B':
      //  Serial.println("Command B received");
        break;
    default:
     //   Serial.println("Unknown command");
        break;
    }
}

void drawAxisTriangle(int16_t base_top_x, int16_t base_top_y, uint16_t color) {
    /*
        Draw a triangle to indicate the selected axis
    
        base_top_x, base_top_y
            |\
            |  \  // right point 
            |  /  point_x, point_y
            |/
        base_bottom_x, base_bottom_y
    */
        int16_t base_height = 20;
        int16_t point_distance = 20;
        int16_t base_bottom_x = base_top_x;
        int16_t base_bottom_y = base_top_y + base_height;
        int16_t point_x = base_top_x + point_distance;
        int16_t point_y = base_top_y + (base_height / 2);

        tft.fillTriangle(base_top_x, base_top_y, base_bottom_x, base_bottom_y, point_x, point_y, color);
}

void selectAxis(char axis)
{
    int16_t base_top_x = 100;
    #define x_axis_triangle_Y 10
    #define y_axis_triangle_Y 62
    #define z_axis_triangle_Y 112

    // Clear all triangles
    uint16_t color = HX8357_BLACK;
    drawAxisTriangle(base_top_x, x_axis_triangle_Y, color);
    drawAxisTriangle(base_top_x, y_axis_triangle_Y, color);
    drawAxisTriangle(base_top_x, z_axis_triangle_Y, color);

   color = HX8357_RED;
    switch (axis)
    {
    case 'X':
        drawAxisTriangle(base_top_x, x_axis_triangle_Y, color);
        currentAxis = '0';
        break;
    case 'Y':
         drawAxisTriangle(base_top_x, y_axis_triangle_Y, color);
        currentAxis = '1';
        break;
    case 'Z':
        drawAxisTriangle(base_top_x, z_axis_triangle_Y, color);
        currentAxis = '2';
        break;
    default:
        currentAxis = '-';
        Serial.println("Unknown axis selected");
        break;
    }
} 

void zeroAxis(char axis)
{
    Serial.print("Zeroing axis: ");
    framer.sendMessage('Z', axis, "");
}

void processJogCommand(int8_t step)
{
    char sDinstance[10];
    
    Serial.print("Jogging axis by: ");
    Serial.println(step);
    float distance = step * currentJogStep; // each step is 0.1 unit
    sprintf(sDinstance, "%.3f", distance);

    framer.sendMessage('J', currentAxis, sDinstance);
}