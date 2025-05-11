#include "Connection.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>

// const char *ssid = "OPPO Reno7 Z 5G";
// const char *password = "vominhquan1401";
const char *ssid = "WIFI-FREE";
const char *password = "14012004";
const char *json_url = "https://raw.githubusercontent.com/vominhquan1401/fw_stm32/main/version.json";
// const char *firmware_url = "https://app.coreiot.io/features/otaUpdates/fw_stm32.bin";
const char *bearer_token = "Bearer eyJhbGciOiJIUzUxMiJ9.eyJzdWIiOiJxdWFuLnZvMTQwMUBoY211dC5lZHUudm4iLCJ1c2VySWQiOiJiOTBmZGJkMC0yYmIzLTExZjAtYWFlMC0wZjg1OTAzYjM2NDQiLCJzY29wZXMiOlsiVEVOQU5UX0FETUlOIl0sInNlc3Npb25JZCI6Ijc1YWFlYWZhLTM1MmItNDQ3Zi1hYmQ5LWU3YTY2N2Y2NTQ4NSIsImV4cCI6MTc0NjY4MDEzNSwiaXNzIjoiY29yZWlvdC5pbyIsImlhdCI6MTc0NjY3MTEzNSwiZmlyc3ROYW1lIjoiUVXDgk4iLCJsYXN0TmFtZSI6IlbDlSBNSU5IIiwiZW5hYmxlZCI6dHJ1ZSwiaXNQdWJsaWMiOmZhbHNlLCJ0ZW5hbnRJZCI6ImI4ZmNjOTAwLTJiYjMtMTFmMC1hYWUwLTBmODU5MDNiMzY0NCIsImN1c3RvbWVySWQiOiIxMzgxNDAwMC0xZGQyLTExYjItODA4MC04MDgwODA4MDgwODAifQ.5naFMo-8AHR_bI6gHq6CzUk6mERFrRuScBAi4RMvQd77UJYbZTi9yH36edEnocbSq90INQItvHuEwR2o5V2i3w";
const char *local_file_path = "/firmware.bin";
const char *version_file_path = "/version.txt";

String readCurrentVersion()
{
    if (SPIFFS.exists(version_file_path))
    {
        File f = SPIFFS.open(version_file_path);
        String ver = f.readStringUntil('\n');
        f.close();
        return ver;
    }
    return "0.0.0";
}

void saveVersion(String ver)
{
    File f = SPIFFS.open(version_file_path, FILE_WRITE);
    if (f)
    {
        f.print(ver);
        f.close();
    }
}

void listSPIFFSFiles()
{
    Serial.println("\n📂 Danh sách file hiện tại trong SPIFFS:");
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
        Serial.printf(" - %s (%u bytes)\n", file.name(), file.size());
        file = root.openNextFile();
    }
}

bool downloadFirmwareToSPIFFS(const char *url, const char *filepath)
{
    HTTPClient http;
    http.begin(url); // URL mới từ GitHub

    int httpCode = http.GET(); // Gửi yêu cầu GET

    // Kiểm tra mã HTTP trả về
    if (httpCode != 200)
    {
        Serial.printf("❌ HTTP lỗi: %d\n", httpCode);
        http.end();
        return false;
    }

    WiFiClient *stream = http.getStreamPtr();   // Lấy con trỏ stream từ HTTP response
    File f = SPIFFS.open(filepath, FILE_WRITE); // Mở file để ghi vào SPIFFS

    if (!f)
    {
        Serial.println("❌ Không thể mở file để ghi");
        http.end();
        return false;
    }

    const size_t bufSize = 256; // Kích thước bộ đệm
    uint8_t buf[bufSize];       // Mảng dữ liệu để lưu dữ liệu tạm
    size_t total = 0;

    // Đọc dữ liệu từ stream và ghi vào SPIFFS
    while (http.connected() && stream->available())
    {
        size_t len = stream->readBytes(buf, bufSize); // Đọc dữ liệu vào bộ đệm
        f.write(buf, len);                            // Ghi dữ liệu vào file
        total += len;                                 // Cộng tổng số byte đã tải xuống
        delay(1);                                     // Tạm dừng một chút để tránh quá tải
    }

    f.close();  // Đóng file
    http.end(); // Kết thúc kết nối HTTP

    Serial.printf("✅ Tải xong %u byte vào %s\n", total, filepath); // In ra thông báo thành công
    return true;
}

void deleteAllFilesInDir()
{
    File root = SPIFFS.open("/");

    File file = root.openNextFile();
    while (file)
    {
        String path = file.name();
        Serial.printf("🗑️ Xoá: %s\n", path.c_str());
        String fullpath = "/" + path;
        SPIFFS.remove(fullpath);
        file = root.openNextFile();
    }
    Serial.printf("✅ Đã xoá toàn bộ file \n");
}

void setup_wifi()
{
    delay(10);
    Serial.println();
    Serial.print("Đang kết nối tới ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("Đã kết nối WiFi");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    if (!SPIFFS.begin(true))
    {
        Serial.println("❌ Lỗi khi mount SPIFFS");
        return;
    }
    // checkForUpdate();
    //  deleteAllFilesInDir();
    listSPIFFSFiles();
}

bool checkForUpdate()
{
    HTTPClient http;
    http.begin(json_url);
    int httpCode = http.GET();

    if (httpCode != 200)
    {
        Serial.printf("❌ Lỗi tải JSON (%d)\n", httpCode);
        http.end();
        return false;
    }

    String payload = http.getString();
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
        Serial.println("❌ Không đọc được JSON");
        http.end();
        return false;
    }

    String latest_version = doc["version"];
    String fw_url = doc["url"];

    String current_version = readCurrentVersion();
    Serial.printf("🔎 Phiên bản hiện tại: %s\n", current_version);
    Serial.printf("📦 Phiên bản mới    : %s\n", latest_version);

    if (current_version != latest_version)
    {
        deleteAllFilesInDir();
        Serial.println("🔄 Có bản mới, bắt đầu tải...");
        if (downloadFirmwareToSPIFFS(fw_url.c_str(), local_file_path))
        {
            saveVersion(latest_version);
            Serial.println("✅ Đã lưu firmware và cập nhật version");
            return true;
        }
        else
        {
            Serial.println("❌ Tải hoặc ghi file thất bại");
        }
    }
    else
    {
        Serial.println("✅ Firmware đã là bản mới nhất");
    }

    http.end();
    return false;
}
