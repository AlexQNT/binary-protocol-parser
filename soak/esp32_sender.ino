uint32_t count = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  uint8_t payload[16];
  for (int i = 0; i < 16; ++i) {
    payload[i] = 0;
  }
  payload[0] = count & 0xFF;
  payload[1] = (count >> 8) & 0xFF;
  payload[2] = (count >> 16) & 0xFF;
  payload[3] = (count >> 24) & 0xFF;

  uint8_t check = 0x10;
  for (int i = 0; i < 16; ++i) {
    check ^= payload[i];
  }

  Serial.write(0xAA);
  Serial.write(0x55);
  Serial.write(0x10);
  Serial.write(payload, 16);
  Serial.write(check);

  ++count;
  delay(10);
}