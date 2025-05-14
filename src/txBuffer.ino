#include <Arduino.h>

#define START_FRAME 0xABCD
#define FLOAT_COUNT 8
#define PACKET_SIZE (2 + (FLOAT_COUNT * 4) + 2)

#pragma pack(push, 1)
typedef struct
{
  /* data */
  uint16_t start;
  float txBuffer[FLOAT_COUNT];
  uint16_t checksum;
} SerialPacket;
#pragma pack(pop)
SerialPacket txPacket;

void SendSerial(float *data)
{
  txPacket.start = START_FRAME;
  memcpy(txPacket.txBuffer, data, sizeof(float) * FLOAT_COUNT);
  uint8_t *byte = (uint8_t *)&txPacket;
  uint16_t checksum = 0;
  for (size_t i = 0; i < PACKET_SIZE - 2; i++)
  {
    checksum ^= byte[i];
  }
  txPacket.checksum = checksum;
  Serial2.write((uint8_t *)&txPacket, PACKET_SIZE);
}
void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
}
void loop()
{
  static unsigned long lastSend = 0;
  if (millis() - lastSend > 20)
  {
    float testValues[FLOAT_COUNT] = {random(-100.00, 100.00), random(-100.00, 100.00), random(-100.00, 100.00), random(-100.00, 100.00), random(-100.00, 100.00), random(-100.00, 100.00), random(-100.00, 100.00), random(-100.00, 100.00)};
    SendSerial(testValues);
    lastSend = millis();
  }
}
