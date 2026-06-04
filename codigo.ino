#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
int mqtt_port = 1883;

const char* topic_temp   = "iot/ar/temperatura";
const char* topic_umid   = "iot/ar/umidade";
const char* topic_gas    = "iot/ar/gas";
const char* topic_status = "iot/ar/status";

#define DHTPIN  15
#define DHTTYPE DHT22
#define GAS_PIN 34
#define LED_R   16
#define LED_G   17
#define LED_B   18

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

void setup_wifi() {
  Serial.print("Conectando ao WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado!");
}

void conectar_mqtt() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("esp32-monitor-ar")) {
      Serial.println("Conectado!");
    } else {
      delay(3000);
    }
  }
}

void setLED(int r, int g, int b) {
  digitalWrite(LED_R, r);
  digitalWrite(LED_G, g);
  digitalWrite(LED_B, b);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  dht.begin();
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  conectar_mqtt();
  Serial.println("Sistema pronto!");
}

void loop() {
  if (!client.connected()) conectar_mqtt();
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 3000) {
    lastMsg = now;

    float temp = dht.readTemperature();
    float umid = dht.readHumidity();

    // Potenciômetro simula o MQ-135
    // Mapeia 0-4095 para 0-1000 ppm
    int raw = analogRead(GAS_PIN);
    int gas = map(raw, 0, 4095, 0, 1000);

    if (isnan(temp) || isnan(umid)) return;

    String status;

    if (gas > 600) {
      setLED(HIGH, LOW, LOW);
      status = "CRITICO";
    } else if (gas > 300) {
      setLED(HIGH, HIGH, LOW);
      status = "ALERTA";
    } else {
      setLED(LOW, HIGH, LOW);
      status = "NORMAL";
    }

    client.publish(topic_temp,   String(temp, 1).c_str());
    client.publish(topic_umid,   String(umid, 1).c_str());
    client.publish(topic_gas,    String(gas).c_str());
    client.publish(topic_status, status.c_str());

    Serial.println("========================");
    Serial.print("Temp: ");    Serial.print(temp); Serial.println("°C");
    Serial.print("Umidade: "); Serial.print(umid); Serial.println("%");
    Serial.print("Gas: ");     Serial.print(gas);  Serial.println(" ppm");
    Serial.print("Status: ");  Serial.println(status);
    Serial.println("Publicado no MQTT!");
  }
}
