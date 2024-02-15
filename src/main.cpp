#include <config.h>
#include <heltec.h>
#include <ArduinoJson.h>
#include <base64.h>
#include <FS.h>
#include <MQTTClient.h>
#include <SPIFFS.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h>


#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"


WiFiClientSecure wifi_client = WiFiClientSecure();
MQTTClient mqtt_client = MQTTClient(256);


void incomingMessageHandler(String &topic, String &payload) {
  Serial.println("Message received on '" + topic + "'");
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
  Serial.println("Sent message");
}


void connect()
{

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Connecting to MQTT..."); 
  Heltec.display->display();

  wifi_client.setCACert(AWS_CERT_CA);
  wifi_client.setCertificate(AWS_CERT_CRT);
  wifi_client.setPrivateKey(AWS_CERT_KEY);

  mqtt_client.begin(AWS_IOT_ENDPOINT, 8883, wifi_client);
  mqtt_client.onMessage(incomingMessageHandler);

  Serial.print("Connecting to MQTT...");
  while (!mqtt_client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  if(!mqtt_client.connected()){
    Serial.println("MQTT Timeout!");
    return;
  }

  mqtt_client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
  Serial.println("MQTT Connected!");

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Waiting for message..."); 
  Heltec.display->display();

}


void setup() {

  Serial.begin(115200);

  Heltec.begin(true, false, false);
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Waiting for Wi-Fi setup..."); 
  Heltec.display->display();

  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP-MQTT");

  Serial.println("Connected to Wi-Fi!");
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Connected to Wi-Fi!"); 
  Heltec.display->display();

}


void loop() {

  if (!mqtt_client.connected()) {
    connect();
  }

  mqtt_client.loop();
  delay(100);

}