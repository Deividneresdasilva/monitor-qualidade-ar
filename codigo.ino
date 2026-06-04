/*
  SISTEMA DE MONITORAMENTO DE QUALIDADE DO AR
  Com Internet das Coisas (IoT) e Protocolo MQTT

  Autor: Deivid Neres Da Silva
  RA: 10441233
  Universidade Presbiteriana Mackenzie
  Disciplina: Objetos Inteligentes Conectados
  Data: Junho de 2025

  Hardware:
  - ESP32 (WiFi integrado)
  - DHT22 (temperatura e umidade) - GPIO 15
  - MQ-135 / Potenciometro (gases) - GPIO 34
  - LED RGB (indicador visual) - GPIO 16, 17, 18

  Broker MQTT: HiveMQ publico
  Host: broker.hivemq.com
  Porta: 1883

  Topicos MQTT:
  - iot/ar/temperatura
  - iot/ar/umidade
  - iot/ar/gas
  - iot/ar/status
  - iot/ar/led

  Bibliotecas necessarias:
  - DHT sensor library (Adafruit)
  - PubSubClient (Nick O'Leary)
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// ============================================================
// CONFIGURACOES DE REDE E MQTT
// ============================================================

const char* ssid        = "Wokwi-GUEST";   // WiFi (Wokwi)
const char* password    = "";               // Sem senha
const char* mqtt_server = "broker.hivemq.com";
int         mqtt_port   = 1883;

// ============================================================
// TOPICOS MQTT
// ============================================================

const char* topic_temp   = "iot/ar/temperatura";
const char* topic_umid   = "iot/ar/umidade";
const char* topic_gas    = "iot/ar/gas";
const char* topic_status = "iot/ar/status";
const char* topic_led    = "iot/ar/led";

// ============================================================
// PINOS
// ============================================================

#define DHTPIN   15   // DHT22 pino de dados
#define DHTTYPE  DHT22
#define GAS_PIN  34   // MQ-135 / Potenciometro
#define LED_R    16   // LED Vermelho
#define LED_G    17   // LED Verde
#define LED_B    18   // LED Azul

// ============================================================
// LIMIARES DE QUALIDADE DO AR
// ============================================================

#define LIMIAR_NORMAL  300   // abaixo = NORMAL
#define LIMIAR_ALERTA  600   // entre 300-600 = ALERTA
                             // acima de 600  = CRITICO

// ============================================================
// OBJETOS
// ============================================================

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMsg = 0;

// ============================================================
// FUNCAO: Conectar ao WiFi
// ============================================================

void setup_wifi() {
  Serial.print("Conectando ao WiFi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 30) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi Conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar WiFi!");
  }
}

// ============================================================
// FUNCAO: Conectar ao Broker MQTT
// ============================================================

void conectar_mqtt() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT (HiveMQ)...");

    String clientId = "esp32-monitor-ar-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado!");
      client.subscribe(topic_led);
    } else {
      Serial.print("Falha, codigo: ");
      Serial.print(client.state());
      Serial.println(" - tentando em 3 segundos...");
      delay(3000);
    }
  }
}

// ============================================================
// FUNCAO: Controlar LED RGB
// ============================================================

void setLED(int r, int g, int b) {
  digitalWrite(LED_R, r);
  digitalWrite(LED_G, g);
  digitalWrite(LED_B, b);
}

// ============================================================
// FUNCAO: Callback para receber comandos via MQTT
// ============================================================

void callback(char* topic, byte* payload, unsigned int length) {
  String mensagem = "";
  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }

  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(mensagem);

  // Controle remoto do LED
  if (String(topic) == topic_led) {
    if (mensagem == "verde")    setLED(LOW, HIGH, LOW);
    if (mensagem == "amarelo")  setLED(HIGH, HIGH, LOW);
    if (mensagem == "vermelho") setLED(HIGH, LOW, LOW);
    if (mensagem == "desligar") setLED(LOW, LOW, LOW);
  }
}

// ============================================================
// SETUP
// ============================================================

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("\n\n========================================");
  Serial.println("  SISTEMA MONITORAMENTO QUALIDADE DO AR ");
  Serial.println("  Deivid Neres Da Silva - RA: 10441233  ");
  Serial.println("  Mackenzie - Objetos Inteligentes      ");
  Serial.println("========================================\n");

  // Configurar pinos do LED
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  // LED inicial apagado
  setLED(LOW, LOW, LOW);

  // Inicializar sensor DHT22
  dht.begin();

  // Conectar WiFi
  setup_wifi();

  // Configurar MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Conectar ao broker
  conectar_mqtt();

  Serial.println("Sistema pronto para monitorar!\n");
}

// ============================================================
// LOOP
// ============================================================

void loop() {
  // Reconectar se desconectado
  if (!client.connected()) {
    conectar_mqtt();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 3000) {
    lastMsg = now;

    // Ler sensores
    float temp = dht.readTemperature();
    float umid = dht.readHumidity();

    // Ler gas (potenciometro mapeado para 0-1000 ppm)
    int raw = analogRead(GAS_PIN);
    int gas = map(raw, 0, 4095, 0, 1000);

    // Validar leitura DHT22
    if (isnan(temp) || isnan(umid)) {
      Serial.println("Erro na leitura do DHT22!");
      return;
    }

    // Avaliar qualidade do ar e controlar LED
    String status;
    if (gas > LIMIAR_ALERTA) {
      setLED(HIGH, LOW, LOW);   // Vermelho - CRITICO
      status = "CRITICO";
    } else if (gas > LIMIAR_NORMAL) {
      setLED(HIGH, HIGH, LOW);  // Amarelo - ALERTA
      status = "ALERTA";
    } else {
      setLED(LOW, HIGH, LOW);   // Verde - NORMAL
      status = "NORMAL";
    }

    // Publicar no MQTT
    client.publish(topic_temp,   String(temp, 1).c_str());
    client.publish(topic_umid,   String(umid, 1).c_str());
    client.publish(topic_gas,    String(gas).c_str());
    client.publish(topic_status, status.c_str());

    // Exibir no Serial Monitor
    Serial.println("========================");
    Serial.print("Temperatura: "); Serial.print(temp); Serial.println(" oC");
    Serial.print("Umidade:     "); Serial.print(umid); Serial.println(" %");
    Serial.print("Gas:         "); Serial.print(gas);  Serial.println(" ppm");
    Serial.print("Status:      "); Serial.println(status);
    Serial.println("Publicado no MQTT HiveMQ!");
    Serial.println();
  }
}
