#include "esphome.h"

class IFanRemote : public Component, public UARTDevice, public TextSensor {
 public:
  IFanRemote(UARTComponent *parent) : UARTDevice(parent) {}

  void setup() override {
    // nothing to do here
  }

  int process_char(uint8_t x, uint8_t *command)
  {
    static uint8_t pos = 0;
    static uint8_t chksum = 0; 

    if ((x == 0xAA) && (pos == 0))
        // first byte 0xAA
        pos = 1;
    else if ((x == 0x55) && (pos == 1)) {
        // second byte 0x55
        pos = 2;
        chksum = 0;
        ESP_LOGV("IFanRemote", "Detected command start (AA:55)");
    }
    else if ((pos >= 2) && (pos < 7)) {
        // save the next 5 bytes as command
        command[pos - 2] = x;
        chksum += x;
        pos++;
    }
    else if (pos == 7) {
        // the last byte is the checksum
        pos = 0;
        ESP_LOGD("IFanRemote", "Received command %s", format_hex_pretty(command, 5).c_str());
        if (x == chksum)
            return 1;
        else
            ESP_LOGE("IFanRemote", "Checksum is 0x%02x but should be 0x%02x", x, chksum);
    }
    else
        pos = 0;

    return 0;
  }

  void loop() override {
    static uint8_t command[5];
    while (available()) {
      if (process_char(read(), command) > 0) {
        publish_state(format_hex(command, 5));
      }
    }
  }
};
