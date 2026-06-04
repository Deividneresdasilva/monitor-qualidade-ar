# Sistema de Monitoramento de Qualidade do Ar com IoT e MQTT

Autor: Deivid Neres Da Silva | RA: 10441233
Universidade Presbiteriana Mackenzie - Analise e Desenvolvimento de Sistemas
Disciplina: Objetos Inteligentes Conectados | Junho de 2025

## Video de Demonstracao
https://youtu.be/He9RIY2dTjc

## Simulacao Wokwi
https://wokwi.com/projects/465762557914729473

## Descricao
Sistema embarcado para monitoramento continuo da qualidade do ar em ambientes internos usando ESP32, sensores DHT22 e MQ-135, com feedback visual em LED RGB e comunicacao via protocolo MQTT com broker HiveMQ. Projeto alinhado a ODS 11 - Cidades e Comunidades Sustentaveis.

## Como Reproduzir o Projeto
1. Acesse https://wokwi.com e crie novo projeto ESP32
2. Adicione os componentes: DHT22, Potenciometro, LED RGB
3. Conecte conforme a pinagem abaixo
4. Cole o codigo do arquivo codigo.ino
5. Em libraries.txt adicione: DHT sensor library e PubSubClient
6. Clique Play para simular
7. Abra https://www.hivemq.com/demos/websocket-client/
8. Conecte e subscribe em: iot/ar/#
9. Veja os dados chegando em tempo real!

## Componentes Utilizados
| Componente | Especificacao | Funcao |
|---|---|---|
| ESP32 | WiFi integrado, 240 MHz, 4 MB flash | Comunicacao WiFi e MQTT |
| DHT22 | -40 a +80 oC, 0 a 100% UR | Temperatura e umidade |
| MQ-135 | Deteccao de gases, saida analogica | Concentracao de gases |
| LED RGB | 3 LEDs PWM, anodo comum | Indicador visual de status |

## Pinagem - ESP32
| Componente | Pino ESP32 | Observacao |
|---|---|---|
| DHT22 VCC | 3V3 | Alimentacao |
| DHT22 SDA | GPIO 15 | Entrada digital |
| DHT22 GND | GND | Terra comum |
| MQ-135 VCC | 3V3 | Alimentacao |
| MQ-135 SIG | GPIO 34 | Entrada analogica |
| MQ-135 GND | GND | Terra comum |
| LED R | GPIO 16 | PWM |
| LED G | GPIO 17 | PWM |
| LED B | GPIO 18 | PWM |
| LED GND | GND | Terra comum |

## Protocolo MQTT e Comunicacao
- Broker: HiveMQ publico (broker.hivemq.com)
- Porta: 1883
- Protocolo: TCP/IP
- QoS: nivel 1
- Biblioteca: PubSubClient
- WiFi: rede Wokwi-GUEST (simulacao) ou rede real

## Topicos MQTT
| Topico | Descricao | Unidade |
|---|---|---|
| iot/ar/temperatura | Temperatura do ar | Graus Celsius |
| iot/ar/umidade | Umidade relativa | Percentual |
| iot/ar/gas | Concentracao de gases | ppm |
| iot/ar/status | Status da qualidade | NORMAL/ALERTA/CRITICO |
| iot/ar/led | Controle remoto do LED | Comando |

## Logica de Funcionamento
- Gas ate 300 ppm = NORMAL = LED VERDE
- Gas 300 a 600 ppm = ALERTA = LED AMARELO
- Gas acima de 600 = CRITICO = LED VERMELHO

## Bibliotecas Necessarias
- DHT sensor library (Adafruit)
- PubSubClient (Nick O'Leary)

## Observacao sobre Simulacao
Na simulacao Wokwi foi utilizado um potenciometro no lugar do sensor MQ-135, pois o simulador nao suporta MQ-135 com ESP32. A logica de leitura analogica e identica ao hardware real.

## Autor
Deivid Neres Da Silva - RA 10441233
Universidade Presbiteriana Mackenzie

## Licenca
MIT
