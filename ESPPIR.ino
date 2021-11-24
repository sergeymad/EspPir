#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char *ssid = "MyASUS_7595"; 
const char *pass = "11111111"; 
const char *mqtt_server = "m13.cloudmqtt.com"; 
const int mqtt_port = 14483; 
const char *mqtt_user = "xxxxxx"; 
const char *mqtt_pass = "xxxxxx";
const int PIR = D7; 
#define BUFFER_SIZE 100
int tm = 300;
float temp = 0;
WiFiClient wclient; 
PubSubClient client(wclient, mqtt_server, mqtt_port);
void setup() {
  pinMode(PIR, INPUT);
  Serial.begin(9600);
  delay(60*1000);
}
void refreshData() {
  int val = digitalRead(PIR);
  if (val == HIGH) {
    client.writeString("Motion detected!",18);
    }
  else {
      client.writeString("No Motion detected!",20);
    }
delay(10);
}
void callback(const MQTT::Publish& pub)
{
  String payload = pub.payload_string();
  String topic = pub.topic();
  Serial.print(pub.topic()); 
  Serial.print(" => ");
  Serial.println(payload); 
  if(topic == "test/2")
  {
     Serial.println("test/2 OK"); 
  }
}
void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) return;
    Serial.println("WiFi connected");
  }
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      Serial.print("Connecting to MQTT server ");
      Serial.print(mqtt_server);
      Serial.println("...");
      if (client.connect(MQTT::Connect("arduinoClient2").set_auth(mqtt_user, mqtt_pass))) {
        Serial.println("Connected to MQTT server ");
        client.set_callback(callback);
        // подписываемся под топики
        client.subscribe("test/1");
        client.subscribe("test/2");
      } else {
        Serial.println("Could not connect to MQTT server"); 
      }
    }
    if (client.connected()){
      client.loop();
      refreshData();
    }
  }
}
