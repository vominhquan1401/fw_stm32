#ifndef CONNECTION_H
#define CONNECTION_H

#include <ArduinoLib.h>

void setup_wifi();
bool downloadFirmwareToSPIFFS(const char *url, const char *filepath);
bool checkForUpdate();
#endif
