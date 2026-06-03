#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define MQ135_PIN A0
#define LED_R 5
#define LED_G 6
#define LED_B 7

DHT dht(DHTPIN, DHTTYPE);
unsigned long lastMsg = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  dht.begin();
  Serial.println("Sistema iniciando...");
  
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, HIGH);
  digitalWrite(LED_B, HIGH);
  
  delay(2000);
}

void loop() {
  unsigned long now = millis();
  
  if (now - lastMsg > 2000) {
    lastMsg = now;
    
    float temp = dht.readTemperature();
    float umidade = dht.readHumidity();
    int gas = analogRead(MQ135_PIN);
    
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print("C | Umidade: ");
    Serial.print(umidade);
    Serial.print("% | Gas: ");
    Serial.println(gas);
    
    if (gas > 600) {
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, HIGH);
      digitalWrite(LED_B, HIGH);
      Serial.println("CRITICO");
    } 
    else if (gas > 350) {
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, HIGH);
      Serial.println("ALERTA");
    } 
    else {
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, HIGH);
      Serial.println("NORMAL");
    }
  }
}
