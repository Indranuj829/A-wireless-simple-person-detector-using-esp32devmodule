#include <WiFi.h>
#include <PubSubClient.h>

// -------------------
// Wi-Fi credentials
// -------------------
const char* ssid = "Keralavision_Abel";
const char* password = "abel9288";
const char* mqtt_server = "192.168.220.17";


WiFiClient espClient;
PubSubClient client(espClient);

// -------------------
// Pin definitions
// -------------------
const int ledPin = 2;      // Updated LED pin (D2)
const int buzzerPin = 18;  // Buzzer pin (D18)

// -------------------
// Connect to Wi-Fi
// -------------------
void setup_wifi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
}

// -------------------
// MQTT callback
// -------------------
void callback(char* topic, byte* message, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)message[i];

  if (msg == "Person") {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }
}

// -------------------
// Reconnect MQTT
// -------------------
void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP32Client")) client.subscribe("person_detector");
    else delay(2000);
  }
}

// -------------------
void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

// -------------------
void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}

