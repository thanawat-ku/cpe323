/*
 *  ESP32
 *  Modem Sleep and wake up
 *  by Mischianti Renzo <https://mischianti.org>
 *
 *  https://mischianti.org/category/tutorial/esp32-tutorial/esp32-practical-power-saving/
 *
 */
 
#include <WiFi.h>
#include <BluetoothSerial.h>
#include "driver/adc.h"
#include <esp_bt.h>
 
#define STA_SSID "<YOUR-SSID>"
#define STA_PASS "<YOUR-PASSWD>"
 
BluetoothSerial SerialBT;
 
void setModemSleep();
void wakeModemSleep();
 
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
 
    setModemSleep();
    Serial2.println("MODEM SLEEP ENABLED FOR 5secs");
}
 
//void loop() {}
unsigned long startLoop = millis();
bool started = false;
void loop() {
    if (!started && startLoop+5000<millis()){
        // Not use delay It has the own policy
        wakeModemSleep();
        Serial2.println("MODEM SLEEP DISABLED");
        started = true;
    }
}
 
void disableWiFi(){
    adc_power_off();
    WiFi.disconnect(true);  // Disconnect from the network
    WiFi.mode(WIFI_OFF);    // Switch WiFi off
    Serial2.println("");
    Serial2.println("WiFi disconnected!");
}
void disableBluetooth(){
    // Quite unusefully, no relevable power consumption
    btStop();
    Serial2.println("");
    Serial2.println("Bluetooth stop!");
}
 
void setModemSleep() {
    disableWiFi();
    disableBluetooth();
    setCpuFrequencyMhz(40);
    // Use this if 40Mhz is not supported
    // setCpuFrequencyMhz(80);
}
 
void enableWiFi(){
    adc_power_on();
    delay(200);
 
    WiFi.disconnect(false);  // Reconnect the network
    WiFi.mode(WIFI_STA);    // Switch WiFi off
 
    delay(200);
 
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
 
void wakeModemSleep() {
    setCpuFrequencyMhz(240);
    enableWiFi();
}
