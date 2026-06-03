# Sistema de Monitoramento de Qualidade do Ar com IoT e MQTT

## Descrição
Sistema embarcado para monitoramento contínuo da qualidade do ar em ambientes internos usando Arduino UNO, sensores DHT22 e MQ-135, com feedback visual em LED RGB e comunicação via protocolo MQTT.

## Componentes
- Arduino UNO
- Sensor DHT22 (temperatura e umidade)
- Sensor MQ-135 (concentração de gases)
- LED RGB (feedback visual)
- Potenciômetro (simulação de gás)

## Pinagem

| Componente | Pino Arduino |
|-----------|--------------|
| DHT22 VCC | 5V |
| DHT22 DATA | 4 |
| DHT22 GND | GND |
| Potenciômetro VCC | 5V |
| Potenciômetro SIG | A0 |
| Potenciômetro GND | GND |
| LED R | 5 |
| LED G | 6 |
| LED B | 7 |
| LED GND | GND |

## Como Usar

1. Abra https://wokwi.com/
2. Crie novo projeto Arduino UNO
3. Adicione os componentes conforme a pinagem
4. Cole o código do arquivo `codigo.ino`
5. Clique "Play" para simular

## Broker MQTT
- Host: broker.hivemq.com
- Port: 1883
- QoS: 1

## Tópicos MQTT
- `iot/ar/temperatura` - Temperatura em °C
- `iot/ar/umidade` - Umidade em %
- `iot/ar/gas` - Concentração em ppm
- `iot/ar/status` - Status (normal/alerta/crítico)
- `iot/ar/led` - Controle remoto do LED

## Autor
Deivid Neres Da Silva - RA 10441233
Universidade Presbiteriana Mackenzie

## Licença
MIT
