#include <Arduino.h>
#define SERIAL_START_FRAME 0xABCD
#define FLOAT_COUNT 8
#define PACKET_SIZE (2 + (FLOAT_COUNT * 4) + 2)

#pragma pack(push, 1)
typedef struct
{
  uint16_t start;
  float data[FLOAT_COUNT];
  uint16_t checksum;
} FeedbackPacket;
#pragma pack(pop)

FeedbackPacket recvPacket;
uint8_t buffer[PACKET_SIZE];
size_t bufferIndex = 0;
bool syncing = false;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop()
{
  while (Serial1.available())
  {
    uint8_t byte = Serial1.read();

    if (!syncing)
    {
      buffer[0] = buffer[1];
      buffer[1] = byte;
      if (((uint16_t)buffer[0] | ((uint16_t)buffer[1] << 8)) == SERIAL_START_FRAME)
      {
        syncing = true;
        buffer[0] = buffer[1] = 0;
        bufferIndex = 2;
        ((uint8_t *)&recvPacket)[0] = 0xCD;
        ((uint8_t *)&recvPacket)[1] = 0xAB;
      }
      continue;
    }

    ((uint8_t *)&recvPacket)[bufferIndex++] = byte;

    if (bufferIndex >= PACKET_SIZE)
    {
      uint8_t *raw = (uint8_t *)&recvPacket;
      uint16_t checksum = 0;
      for (size_t i = 0; i < PACKET_SIZE - 2; ++i)
      {
        checksum ^= raw[i];
      }

      uint16_t received = recvPacket.checksum;
      if (checksum == received)
      {
        // for (int i = 0; i < FLOAT_COUNT; i++) {
        //     Serial.println(recvPacket.data[i], 6);
        // }
        Serial.print("Flaot[0]");
        Serial.print(recvPacket.data[0], 4);
        Serial.print("\t");
        Serial.print("Flaot[1]");
        Serial.print(recvPacket.data[1], 4);
        Serial.print("\t");
        Serial.print("Flaot[2]");
        Serial.print(recvPacket.data[2], 4);
        Serial.print("\t");
        Serial.print("Flaot[3]");
        Serial.print(recvPacket.data[3], 4);
        Serial.print("\t");
        Serial.print("Flaot[4]");
        Serial.print(recvPacket.data[4], 4);
        Serial.print("\t");
        Serial.print("Flaot[5]");
        Serial.print(recvPacket.data[5], 4);
        Serial.print("\t");
        Serial.print("Flaot[6]");
        Serial.print(recvPacket.data[6], 4);
        Serial.print("\n");
        syncing = false;
        bufferIndex = 0;
      }
    }
  }
}
