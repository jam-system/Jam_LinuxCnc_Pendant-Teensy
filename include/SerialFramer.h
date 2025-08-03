#pragma once
#include <Arduino.h>
#include <vector>
#include <string>
#include <cstdarg>
#include <cstdio>

class SerialFramer {
public:
    static constexpr uint8_t STX = 0x02;
    static constexpr uint8_t ETX = 0x03;
    static constexpr size_t PAYLOAD_BUFFER_SIZE = 128;

    struct Message {
        char command;
        char parameter;
        std::string data;
    };

    SerialFramer(HardwareSerial& serialPort, size_t maxBufferSize = 256);

    void begin(unsigned long baudrate);
    void sendMessage(char command, char parameter, const char* format = "", ...);
    bool poll(Message& outMessage);

private:
    HardwareSerial& serial;
    std::vector<uint8_t> buffer;
    size_t maxBuffer;

    bool isAscii(char c) const;
};
