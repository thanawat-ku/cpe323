#include <esp_now.h>
#include <WiFi.h>

#define POT_PIN A0

uint8_t broadcastAddress[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

typedef struct {
  uint16_t potValue;
} DataPacket;

DataPacket data;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);
}

void loop() {
  data.potValue = analogRead(POT_PIN);

  esp_now_send(broadcastAddress,
               (uint8_t*)&data,
               sizeof(data));

  Serial.println(data.potValue);
  delay(50);
}
