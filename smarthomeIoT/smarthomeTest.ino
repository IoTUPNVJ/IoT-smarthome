#include <WiFi.h>
#include <MQTT.h>

WiFiClient net;
MQTTClient client;

void publishDataTest();

void setup(){
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  client.begin("mqtt-dashboard.com", net);

  connect();
}

void loop(){
  client.loop();
  if (!client.connected()) {//dia akan berusaha nyambung
    connect();
  }

  publishDataTest();

  delay(5000);
}

void connect(){
/*cek apakah wifi connect/tidak
udh nyambung ke broker apa blom
*/
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  while (!client.connect("ec93d603")) {
    delay(500);
  }
}

void publishDataTest(){
  client.publish("suhu", "50");
  client.publish("jendela 1", "buka");
}

