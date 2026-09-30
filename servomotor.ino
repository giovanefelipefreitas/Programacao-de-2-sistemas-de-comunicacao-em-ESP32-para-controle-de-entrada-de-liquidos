// Código utilizado para o Servo motor

#include <ESP32Servo.h>
#include <PubSubClient.h>
#include <WiFi.h>

const char* mqttUser = "";
const char* mqttPassword = "";
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int servoPin = 14;
Servo servo;

void setup() {
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println("Conectado ao WiFi");

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    servo.attach(servoPin);
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Tentando se conectar ao MQTT...");
        if (client.connect("ESP32_Controlador", mqttUser, mqttPassword)) {
            Serial.println("Conectado ao MQTT!");
            client.subscribe("altura");
        } else {
            Serial.print("Falha na conexão. Erro: ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}

void callback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    long distancia = message.substring(10, message.length() - 2).toInt();
    int angulo = map(distancia, 0, 200, 0, 180);

    servo.write(angulo);

    Serial.print("Distancia: ");
    Serial.print(distancia);
    Serial.print(" cm, Servo em: ");
    Serial.println(angulo);
}

void loop() {

    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
