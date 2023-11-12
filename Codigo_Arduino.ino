#include "ArduinoJson.h"
#include "EspMQTTClient.h"
// MQTT Configuracoes
EspMQTTClient client{
  "WIFI_SSID", //SSID do WiFi
  "WIFI_PASSWORD",     // Senha do wifi
  "mqtt.tago.io",  // Endereço do servidor
  "Default",       // Usuario
  "YOUR_TOKEN",         // Token do device
  "YOUR_DEVICE",           // Nome do device
  1883             // Porta de comunicação
};

#define TRIG_PIN 19 
#define ECHO_PIN 18
float duration_us, distance_cm;

void setup() {
  Serial.begin(9600);
  Serial.println("Conectando WiFi");
  while (!client.isWifiConnected()) {
    Serial.print('.'); client.loop(); delay(1000);
  }
  Serial.println("WiFi Conectado");
  Serial.println("Conectando com Servidor MQTT");
  while (!client.isMqttConnected()) {
    Serial.print('.'); client.loop(); delay(1000);
  }
  Serial.println("MQTT Conectado");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

}
// Callback da EspMQTTClient
void onConnectionEstablished()
{}
char bufferJson[100];
String bueiro = "Limpo";
void loop() {

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
 
  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(ECHO_PIN, HIGH);
  
  // calculate the distance
  distance_cm = 0.017 * duration_us;
 
  // ligt up led if distance under limit
  if (distance_cm < 50) {bueiro = "Obstruido";
  }else{bueiro = "Limpo";
  }

  StaticJsonDocument<300> documentoJson;
  documentoJson["variable"] = "Bueiro1";
  documentoJson["value"] = bueiro;
  serializeJson(documentoJson, bufferJson);
  Serial.println(bufferJson);
  client.publish("topicoEngSoft", bufferJson);
  delay(2000);
  client.loop();
}