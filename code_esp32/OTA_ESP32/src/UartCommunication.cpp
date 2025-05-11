#include "UartCommunication.h"

#define STM32_BOOT0 26 // Chân GPIO để kéo BOOT0
#define STM32_NRST 27  // Chân GPIO để reset STM32
#define STM32_UART_RX 16
#define STM32_UART_TX 17
#define STM32_BAUD 9600
#define USART 1

HardwareSerial stm32Serial(USART);

void waitMillis(unsigned long interval)
{
    unsigned long start = millis();
    while (millis() - start < interval)
    {
        // không làm gì cả – chờ nhưng không dùng delay()
    }
}
void outBootloaderMode()
{
    digitalWrite(STM32_BOOT0, LOW); // BOOT0 = 1
    delay(10);
    digitalWrite(STM32_NRST, LOW); // Reset STM32
    delay(50);
    digitalWrite(STM32_NRST, HIGH);
    delay(200);
}

void enterBootloaderMode()
{
    pinMode(STM32_BOOT0, OUTPUT);
    pinMode(STM32_NRST, OUTPUT);

    digitalWrite(STM32_BOOT0, HIGH); // BOOT0 = 1
    delay(10);
    digitalWrite(STM32_NRST, LOW); // Reset STM32
    delay(50);
    digitalWrite(STM32_NRST, HIGH);
    delay(200);
}

bool waitForAck(uint32_t timeout = 1000)
{
    uint32_t start = millis();
    while (millis() - start < timeout)
    {
        if (stm32Serial.available())
        {
            uint8_t b = stm32Serial.read();
            Serial.printf("Giá trị phản hồi ACK = 0x%02X\n", b);
            return b == 0x79;
        }
    }
    return false;
}

bool sendCommand(uint8_t cmd)
{
    stm32Serial.write(cmd);
    waitMillis(20);
    stm32Serial.write(cmd ^ 0xFF);

    return waitForAck();
}

bool sendAddress(uint32_t addr)
{
    uint8_t buf[4];
    buf[0] = (addr >> 24) & 0xFF;
    stm32Serial.write(buf[0]);

    waitMillis(20);
    buf[1] = (addr >> 16) & 0xFF;
    stm32Serial.write(buf[1]);

    waitMillis(20);
    buf[2] = (addr >> 8) & 0xFF;
    stm32Serial.write(buf[2]);

    waitMillis(20);
    buf[3] = addr & 0xFF;
    stm32Serial.write(buf[3]);

    waitMillis(20);
    uint8_t checksum = buf[0] ^ buf[1] ^ buf[2] ^ buf[3];
    // stm32Serial.write(buf, 4);
    // waitMillis(20);
    stm32Serial.write(checksum);
    return waitForAck();
}

bool writeMemoryBlock(uint32_t addr, const uint8_t *data, size_t len)
{
    if (!sendCommand(0x31))
    {
        Serial.println("ERASE: Wrong due to a failed send command.");
        return false;
    }
    if (!sendAddress(addr))
    {
        Serial.println("ERASE: Wrong due to a failed send address.");
        return false;
    }
    stm32Serial.write((uint8_t)(len - 1));
    waitMillis(20);
    uint8_t checksum = len - 1;
    for (size_t i = 0; i < len; i++)
    {
        checksum ^= data[i];
        stm32Serial.write(data[i]);
        waitMillis(20);
    }
    stm32Serial.write(checksum);
    return waitForAck();
}

bool eraseFullFlash()
{

    if (!sendCommand(0x43))
    {
        Serial.println("ERASE: Wrong due to a failed send command.");
        return false;
    }
    stm32Serial.write(0xFF);
    waitMillis(20);
    stm32Serial.write(0x00);
    return waitForAck();
}

bool testBootloaderConnection()
{
    Serial.println("Gửi SYNC (0x7F) đến STM32...");
    stm32Serial.write(0x7F);
    if (waitForAck())
    {
        Serial.println("✅ STM32 đã phản hồi ACK (0x79) – bootloader hoạt động!");
        return true;
    }
    else
    {
        Serial.println("❌ Không nhận được ACK – kiểm tra BOOT0 và RESET!");
        return false;
    }
}

void testWriteMemoryBlock()
{
    uint8_t sampleData[256];
    for (int i = 0; i < 256; i++)
        sampleData[i] = i;
    if (writeMemoryBlock(0x08000000, sampleData, sizeof(sampleData)))
    {
        Serial.println("Ghi 256 byte thành công.");
    }
    else
    {
        Serial.println("Ghi thất bại.");
    }
}

void setupUart()
{
    stm32Serial.begin(STM32_BAUD, SERIAL_8N1, STM32_UART_RX, STM32_UART_TX);
    delay(1000);

    Serial.println("--- Bắt đầu kiểm tra STM32 Bootloader ---");
    enterBootloaderMode();

    if (!testBootloaderConnection())
        return;

    // Bước 2: Erase toàn bộ
    if (eraseFullFlash())
    {
        Serial.println("Xoá flash thành công.");
    }
    else
    {
        Serial.println("Lỗi khi xoá flash.");
        return;
    }

    // Bước 3: Ghi dữ liệu mẫu
    // testWriteMemoryBlock();

    Serial.println("--- Kết thúc ---");
}

bool sendGo(uint32_t addr)
{
    if (!sendCommand(0x21))
        return false;
    return sendAddress(addr);
}