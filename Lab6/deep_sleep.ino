#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "...................";
const char* password = "......................";

#define POT_PIN 34
#define LED_PIN 32

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

String potValue = "0";

// HTML + JavaScript 
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Real-Time Dashboard</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    button { padding: 10px 20px; font-size: 16px; }
  </style>
</head>
<body>

<h2>ESP32 Real-Time Dashboard</h2>

<p>Potentiometer Value:</p>
<h1 id="pot">0</h1>

<button onclick="toggleLED()">Toggle LED</button>

<script>
let ws = new WebSocket(`ws://${location.host}/ws`);

ws.onmessage = (event) => {
  document.getElementById("pot").innerHTML = event.data;
};

function toggleLED() {
  ws.send("toggle");
}
</script>

</body>
</html>
)rawliteral";

// Handle WebSocket events
void onWebSocketEvent(AsyncWebSocket *server,
                      AsyncWebSocketClient *client,
                      AwsEventType type,
                      void *arg,
                      uint8_t *data,
                      size_t len) {

  if (type == WS_EVT_DATA) {
    String msg = "";
    for (int i = 0; i < len; i++) {
      msg += (char)data[i];
    }

    if (msg == "toggle") {
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  ws.onEvent(onWebSocketEvent);
  server.addHandler(&ws);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  server.begin();
}

void loop() {
  int pot = analogRead(POT_PIN);
  potValue = String(pot);

  ws.textAll(potValue);   // Push value instantly to browser
  delay(100);
}
