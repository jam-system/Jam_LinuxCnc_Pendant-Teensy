#include <Arduino.h>
#include "SerialFramer.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <DisplayUtils.h>
#include "ButtonHandler.h"
#include "EncoderHandler.h"
#include <iostream>
#include <string>

SerialFramer framer(Serial5);
//KeyboardHandler keyboard;
//Status status;
static constexpr uint8_t cs_tft  = 10;
static constexpr uint8_t dc_tft  = 9;

// Use hardware SPI
Adafruit_ILI9341 tft = Adafruit_ILI9341(cs_tft, dc_tft);

ButtonHandler buttons;
EncoderHandler encoder(5, 4); 

//Controller controller(tft, framer, buttons, encoder);

/**
   Declare your DisplayNumber(s)
*/
    DisplayNumber xAxis(tft);
    DisplayNumber yAxis(tft);
    DisplayNumber zAxis(tft);
    DisplayNumber aAxis(tft);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial5.begin(115200);
    delay(200);

    framer.begin(115200);
    Serial.println("Basic DisplayNumber");
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
    /**
         Call begin() *after* tft.begin() for each DisplayNumber to set the font
        (before using other methods)
    */
    xAxis.begin(&FreeMonoBold24pt7b);
    yAxis.begin(&FreeMonoBold24pt7b);
    zAxis.begin(&FreeMonoBold24pt7b);
    //Set the position, font, size and precision
    aAxis.begin(50, 150, &FreeMonoBold12pt7b, 15, 0);

    /**
         You can set the posi
    */

    xAxis.setPosition(0,0);
    xAxis.setTextColor(ILI9341_YELLOW);
    tft.setFont(&FreeMonoBold24pt7b);
    tft.setCursor(45,xAxis.h());
    tft.print("X");
    tft.setCursor(45,xAxis.h()+xAxis.h()+20);
    tft.print("Y");
    tft.setCursor(45,xAxis.h()+xAxis.h()+xAxis.h()+40);
    tft.print("Z");

    //Right align and stack x, y, & z
    xAxis.setPosition(tft.width() - yAxis.w()-10, 0);
    yAxis.setPosition(tft.width() - yAxis.w()-10, xAxis.h() + 20);
    zAxis.setPosition(tft.width() - zAxis.w()-10, xAxis.h() + yAxis.h() + 40);

 


    //Set the background colour
    

    int16_t height = 20;
    int16_t x_width = 20;
    int16_t y_width = height/2;
    
    int16_t x_top = 10;
    int16_t y_top = 10;
    printf("y_top: %d\n",y_top);
    tft.fillTriangle(x_top, y_top,  x_top, y_top+height,   x_top + x_width, y_top+y_width, ILI9341_RED);

     x_top = 10;
     y_top = y_top+xAxis.h()+20;
     printf("y_top: %d\n",y_top);
    tft.fillTriangle(x_top, y_top,  x_top, y_top+height,   x_top + x_width, y_top+y_width, ILI9341_RED);

    x_top = 10;
    y_top = y_top+xAxis.h()+20;
    printf("y_top: %d\n",y_top);
    tft.fillTriangle(x_top, y_top,  x_top, y_top+height,   x_top + x_width, y_top+y_width, ILI9341_RED);


     x_top = 10;
     y_top = 61;
    tft.fillTriangle(x_top, y_top,  x_top, y_top+height,   x_top + x_width, y_top+y_width, ILI9341_BLUE);
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
    }
}
void processMessage(const SerialFramer::Message &msg)
{
    printf("Rec: %c %c %s\n", msg.command, msg.parameter, msg.data.c_str());
    Serial.print("Received Cmd: ");

    switch (msg.command)
    {
    case 'P': {
        Serial.println("Command P received");
        float value = std::stof(msg.data.c_str());
        if (msg.parameter == 'X')  xAxis.draw(value);
        else if (msg.parameter == 'Y')  yAxis.draw(value);
        else if (msg.parameter == 'Z') zAxis.draw(value);
        break;
        }
    case 'B':
        Serial.println("Command B received");
        break;
    default:
        Serial.println("Unknown command");
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
    
    int16_t base_top_x = 10;
    #define x_axis_triangle_Y 10
    #define y_axis_triangle_Y 62
    #define z_axis_triangle_Y 112

    // Clear all triangles
    uint16_t color = ILI9341_BLACK;
    drawAxisTriangle(base_top_x, x_axis_triangle_Y, color);
    drawAxisTriangle(base_top_x, y_axis_triangle_Y, color);
    drawAxisTriangle(base_top_x, z_axis_triangle_Y, color);

   color = ILI9341_RED;
    switch (axis)
    {
    case 'X':
        drawAxisTriangle(base_top_x, x_axis_triangle_Y, color);
        break;
    case 'Y':
         drawAxisTriangle(base_top_x, y_axis_triangle_Y, color);
        break;
    case 'Z':
        drawAxisTriangle(base_top_x, z_axis_triangle_Y, color);
        break;
    default:
        Serial.println("Unknown axis selected");
        break;
    }
} 
