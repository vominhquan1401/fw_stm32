#include "ArduinoLib.h"
#include "Connection.h"
#include "UartCommunication.h"
#include "Handle.h"

void setup()
{
  Serial.begin(9600);
  setup_wifi();
  setupUart();
  Handle();
  outBootloaderMode();
}

void loop()
{
  static uint32_t time_update;
  if (millis() - time_update >= 10000)
  {
    if (checkForUpdate())
    {
      Handle();
      outBootloaderMode();
    }
    time_update = millis();
  }
}
