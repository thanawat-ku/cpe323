// Deep Sleep Example - External Wake-up 
#include "driver/rtc_io.h"
#define BUTTON_PIN 33 // GPIO33 สำหรับปุ่มกด 
RTC_DATA_ATTR int bootCount = 0; // เก็บค่าใน RTC Memory 
void print_wakeup_reason() { 
  esp_sleep_wakeup_cause_t wakeup_reason; 
  wakeup_reason = esp_sleep_get_wakeup_cause(); 
  switch(wakeup_reason) { 
    case ESP_SLEEP_WAKEUP_EXT0: 
      Serial.println("Wakeup caused by external signal using RTC_IO"); 
      break; 
    case ESP_SLEEP_WAKEUP_TIMER: 
      Serial.println("Wakeup caused by timer"); 
      break; 
    default: 
      Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); 
      break; 
  } 
} 
void setup() { 
  Serial.begin(115200); 
  delay(1000); 
  // เพิ่ม Boot Count 
  ++bootCount; 
  Serial.println("Boot number: " + String(bootCount)); 
  // แสดงสาเหตุที่ตื่น 
  print_wakeup_reason(); 
  // ตั้งค่า Wake-up Sources 
  // 1. Timer Wake-up (60 วินาที) 
  esp_sleep_enable_timer_wakeup(60 * 1000000); 
  // 2. External Wake-up (ปุ่มกด) 
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 1); // 1 = HIGH, 0 = LOW 
  // ทำงานที่ต้องการ 
  Serial.println("Doing some work..."); 
  int sensorValue = analogRead(34); 
  Serial.println("Sensor: " + String(sensorValue)); 
  // เข้า Deep Sleep 
  Serial.println("Going to Deep Sleep..."); 
  delay(100); 
  esp_deep_sleep_start(); 
} 
void loop() { 
  // ไม่ทำงาน 
}
