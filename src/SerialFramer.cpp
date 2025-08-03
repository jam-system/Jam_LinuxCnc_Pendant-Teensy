#include "SerialFramer.h"

SerialFramer::SerialFramer(HardwareSerial& serialPort, size_t maxBufferSize)
    : serial(serialPort), maxBuffer(maxBufferSize) {}

void SerialFramer::begin(unsigned long baudrate) {
    serial.begin(baudrate);
}

void SerialFramer::sendMessage(char command, char parameter, const char* format, ...) {
    if (!isAscii(command) || !isAscii(parameter)) return;
Serial.printf("Sending message: %c %c %s", command, parameter, format);
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
                char param = *(it_stx + 2);
                std::string payload(it_stx + 3, it_etx);
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
