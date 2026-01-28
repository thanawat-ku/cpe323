/*
 *  ESP32
 *  Light Sleep and wake up
 *  by Mischianti Renzo <https://mischianti.org>
 *
 *  https://mischianti.org/category/tutorial/esp32-tutorial/esp32-practical-power-saving/
 *
 */
 
#include <WiFi.h>
#include <BluetoothSerial.h>
#include "driver/adc.h"
#include <esp_bt.h>
#include <esp_wifi.h>
#include <esp_sleep.h>
#define STA_SSID "<YOUR-SSID>"
#define STA_PASS "<YOUR-PASSWD>"
 
BluetoothSerial SerialBT;
 
int variable = 0;
 
void setup() {
    Serial2.begin(115200);
 
    while(!Serial2){delay(500);}
 
    SerialBT.begin("ESP32test"); //Bluetooth device name
    SerialBT.println("START BT");
 
    Serial2.println("START WIFI");
    WiFi.begin(STA_SSID, STA_PASS);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial2.print(".");
    }
 
    Serial2.println("");
    Serial2.println("WiFi connected");
    Serial2.println("IP address: ");
    Serial2.println(WiFi.localIP());
 
    delay(1000);
 
    variable += 10;
 
    Serial2.println();
    Serial2.println("LIGHT SLEEP ENABLED FOR 5secs");
    delay(100);
 
    esp_sleep_enable_timer_wakeup(5 * 1000 * 1000);
    esp_light_sleep_start();
 
    Serial2.println();
    Serial2.println("LIGHT SLEEP WAKE UP");
 
    Serial2.print("Variable = ");
    Serial2.println(variable);
}
 
void loop() {
 
}
 
void disableWiFi(){
    adc_power_off();
    WiFi.disconnect(true);  // Disconnect from the network
    WiFi.mode(WIFI_OFF);    // Switch WiFi off
 
    Serial2.println("");
    Serial2.println("WiFi disconnected!");
}
void enableWiFi(){
    adc_power_on();
    WiFi.disconnect(false);  // Reconnect the network
    WiFi.mode(WIFI_STA);    // Switch WiFi off
 
    Serial2.println("START WIFI");
    WiFi.begin(STA_SSID, STA_PASS);
 
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial2.print(".");
    }
 
    Serial2.println("");
    Serial2.println("WiFi connected");
    Serial2.println("IP address: ");
    Serial2.println(WiFi.localIP());
}
void disableBluetooth(){
    btStop();
    esp_bt_controller_disable();
    delay(1000);
    Serial2.println("BT STOP");
}
