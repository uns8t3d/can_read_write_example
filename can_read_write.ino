#include <SPI.h>
#include <mcp2515.h>

MCP2515 mcp2515(10);
int canType = 0;    // 0 - bcan, 1 - fcan


struct can_frame readCanFrame;
struct can_frame writeCanFrame;

void setup() {
  Serial.begin(115200);
  mcp2515.reset();
  switch (canType) {
    case 0:
      mcp2515.setBitrate(CAN_33KBPS, MCP_8MHZ);
      break;
    case 1:
      mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
      break;
  }
  mcp2515.setNormalMode();
}

void loop() {
  if (mcp2515.readMessage(&readCanFrame) == MCP2515::ERROR_OK) {
    canPrintIncoming();
  }
}

void canPrintIncoming() {
  Serial.print("0x");
  Serial.print(readCanFrame.can_id, HEX);
  Serial.print("[");
  Serial.print(readCanFrame.can_dlc, HEX);
  Serial.print("]");
  for (int i = 0; i < readCanFrame.can_dlc; i++)  {
    Serial.print(".");
    Serial.print(readCanFrame.data[i], HEX);
  }
  Serial.println();
}


void sendMessage() {
  /*
  Send message to CAN bus
  can_id - desired ID of can message
  can_dlc - length of message
  data - array of bytes
  */
  writeCanFrame.can_id = 0x12345678;
  writeCanFrame.can_dlc = 4;
  writeCanFrame.data[0] = 0x00;
  writeCanFrame.data[1] = 0x00;
  writeCanFrame.data[2] = 0x00;
  writeCanFrame.data[3] = 0x00;
  mcp2515.sendMessage(&writeCanFrame);
}