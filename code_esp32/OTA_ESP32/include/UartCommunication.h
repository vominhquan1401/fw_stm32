#ifndef UART_COMMUNICATION_H
#define UART_COMMUNICATION_H

#include <ArduinoLib.h>

void setupUart();
void waitMillis(unsigned long interval);
void outBootloaderMode();
bool writeMemoryBlock(uint32_t addr, const uint8_t *data, size_t len);
bool sendGo(uint32_t addr);
#endif
