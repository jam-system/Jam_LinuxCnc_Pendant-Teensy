#include "SerialFramer.h"

SerialFramer::SerialFramer(HardwareSerial& serialPort, size_t maxBufferSize)
    : serial(serialPort), maxBuffer(maxBufferSize) {}

void SerialFramer::begin(unsigned long baudrate) {
    serial.begin(baudrate);
}

void SerialFramer::sendMessage(char command, char parameter, const char* format, ...) {
    if (!isAscii(command) || !isAscii(parameter)) return;
Serial.printf("Sending: %c %c %s\n", command, parameter, format);
    char payload[PAYLOAD_BUFFER_SIZE];
    va_list args;
    va_start(args, format);
    int len = vsnprintf(payload, sizeof(payload), format, args);
    va_end(args);

    if (len < 0 || static_cast<size_t>(len) >= sizeof(payload)) return;

    serial.write(STX);
    serial.write(command);
    serial.write(parameter);
    serial.write(reinterpret_cast<const uint8_t*>(payload), len);
    serial.write(ETX);
}

bool SerialFramer::poll(Message& outMessage) {
    while (serial.available()) {
        uint8_t byte = serial.read();
        if (buffer.size() < maxBuffer) buffer.push_back(byte);
        else buffer.clear();

        auto it_stx = std::find(buffer.begin(), buffer.end(), STX);
        auto it_etx = std::find(it_stx + 1, buffer.end(), ETX);

        if (it_stx != buffer.end() && it_etx != buffer.end()) {
            if (std::distance(it_stx, it_etx) >= 3) {
                char cmd = *(it_stx + 1);
                if (!isAscii(cmd)) {
                    buffer.erase(buffer.begin(), it_etx + 1);
                    Serial.printf("Cmd not ascii: %02X\n", cmd);
                    return false;
                }   

                char param = *(it_stx + 2);
                if (!isAscii(param)) {
                    buffer.erase(buffer.begin(), it_etx + 1);
                    Serial.printf("Param not ascii: %02X\n", param);
                    return false;
                }   
                std::string payload(it_stx + 3, it_etx);
                if (!std::all_of(payload.begin(), payload.end(), [this](char c){ return isAscii(c); })) {
                    buffer.erase(buffer.begin(), it_etx + 1);
                    Serial.printf("Payload not ascii: %s\n", payload.c_str());
                    return false;
                }
                outMessage = { cmd, param, payload };
            }
            buffer.erase(buffer.begin(), it_etx + 1);
            return true;
        }
    }
    return false;
}

bool SerialFramer::isAscii(char c) const {
    return (c >= 0x20 && c <= 0x7E);
}
