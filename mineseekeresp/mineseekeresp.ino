#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(9600);        // UART from Arduino
  SerialBT.begin("ESP32-BT"); // Device name seen in Serial Bluetooth Terminal app
  Serial.println("ESP32 Bluetooth Started. Waiting for data...");
}

void loop() {
  if (Serial.available()) {
    String data = Serial.readStringUntil('\n');  // Read message from Arduino
    SerialBT.println(data);                      // Send to phone via Bluetooth
    Serial.println("Sent to phone: " + data);
  }
}
