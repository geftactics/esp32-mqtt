#include "config.h"
#include "heltec.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

WiFiClientSecure wifi_client = WiFiClientSecure();
MQTTClient mqtt_client = MQTTClient(256);

void incomingMessageHandler(String &topic, String &payload) {
  Serial.println("Message received!");
  Serial.println("Topic: " + topic);
  Serial.println("Payload: " + payload);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, payload); 
  Heltec.display->display();
}

void publishMessage()
{
  JsonDocument doc;
  doc["value"] = random(1000);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  mqtt_client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
  Serial.println("Sent a message");
}

void connect()
{
  WiFi.mode(WIFI_STA); 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.println("Connecting to Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  wifi_client.setCACert(AWS_CERT_CA);
  wifi_client.setCertificate(AWS_CERT_CRT);
  wifi_client.setPrivateKey(AWS_CERT_PRIVATE);

  mqtt_client.begin(AWS_IOT_ENDPOINT, 8883, wifi_client);
  mqtt_client.onMessage(incomingMessageHandler);

  Serial.print("Connecting to AWS IOT...");
  while (!mqtt_client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if(!mqtt_client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  mqtt_client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("AWS IoT Connected!");
}

void setup() {
  Serial.begin(115200);
  Heltec.begin(true, false, false);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Waiting for message..."); 
  Heltec.display->display();
}

void loop() {
  if (!mqtt_client.connected()) {
    connect();
  }
  mqtt_client.loop();
  delay(1000);
}