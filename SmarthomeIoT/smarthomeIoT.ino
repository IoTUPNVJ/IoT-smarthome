#include <WiFi.h>
#include <MQTT.h>
#include "DHTesp.h"

#define DHTPIN 21

WiFiClient net;
MQTTClient client;
DHTesp dhtSensor;

const char ssid[] = "Redmi 9t";
const char pass[] = "1desember2005";

float suhu, kelembapan;
int jumlahOrang = 2; //data palsu
int kipas = 3; //data palsu
char jendela[] = "buka"; //data palsu
char buffer[10]; //itoa

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  client.begin("broker.emqx.io", net);  // Pilih satu broker
  dhtSensor.setup(DHTPIN, DHTesp::DHT22);
  connect();
}

void loop() {
  client.loop();
  if (!client.connected()) {
    connect();
  }

  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  suhu = data.temperature;
  kelembapan = data.humidity;

  if (isnan(suhu) || isnan(kelembapan)) {
    Serial.println("Gagal membaca dari sensor DHT!");
    return;
  }

  dtostrf(suhu, 4, 2, buffer);
  client.publish("smarthomeIoT/suhu", buffer);
  Serial.print("Suhu: ");
  Serial.println(buffer);

  dtostrf(kelembapan, 4, 2, buffer);
  client.publish("smarthomeIoT/kelembapan", buffer);
  Serial.print("Kelembapan: ");
  Serial.println(buffer);

  itoa(jumlahOrang, buffer, 10);
  client.publish("smarthomeIoT/jumlahOrang", buffer);

  client.publish("smarthomeIoT/jendela", jendela);

  itoa(kipas, buffer, 10);
  client.publish("smarthomeIoT/kecepatan", buffer);

  delay(2000);
}

void connect() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  while (!client.connect("ec93d603")) {
    delay(500);
    Serial.print("*");
  }
  Serial.println("\nConnected to MQTT broker!");
}
