#include "Handle.h"
#include "Connection.h"
#include "UartCommunication.h"

#define ADDR_START 0x08000000

void Handle()
{
    File binFile = SPIFFS.open("/firmware.bin", FILE_READ);
    if (!binFile)
    {
        Serial.println("Can't open file firmware.bin");
        return;
    }

    uint8_t sampleData[256]; // Mảng 256 byte
    size_t bytesRead = 0;
    int block_index = 0;
    // Đọc file theo từng khối 256 byte
    while (binFile.available())
    {
        // Đọc tối đa 256 byte từ file
        bytesRead = binFile.read(sampleData, sizeof(sampleData));

        // Gửi dữ liệu đọc được qua UART (hoặc xử lý dữ liệu ở đây)
        Serial.printf("Đọc %d byte từ file\n", bytesRead);
        uint32_t addr = ADDR_START + block_index * 0x0100;
        // Serial.printf("Address (hex): 0x%08X\n", addr);
        if (!writeMemoryBlock(addr, sampleData, bytesRead))
        {
            Serial.printf("Đọc %d byte từ file\n", block_index);
        }
        block_index++;
        waitMillis(100);
    }
    binFile.close();
    if (!sendGo(ADDR_START))
    {
        Serial.println("CAN'T GO to start address");
    }
}
