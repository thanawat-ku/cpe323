#include <esp_now.h>
#include <WiFi.h>

#define LED_PIN 15

typedef struct {
  uint16_t potValue;
} DataPacket;

DataPacket data;

// NEW callback signature for ESP32 core 3.x / IDF v5
void onDataReceive(const esp_now_recv_info_t *recv_info,
                   const uint8_t *incomingData,
                   int len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.println(data.potValue);
  int pwmValue = map(data.potValue, 0, 4095, 0, 255);
  Serial.println(pwmValue);
  analogWrite(LED_PIN,pwmValue);
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  // REGISTER RECEIVE CALLBACK (new signature)
  //esp_now_register_recv_cb(onDataReceive);
  esp_now_register_recv_cb(esp_now_recv_cb_t(onDataReceive));
}

void loop() {

}
