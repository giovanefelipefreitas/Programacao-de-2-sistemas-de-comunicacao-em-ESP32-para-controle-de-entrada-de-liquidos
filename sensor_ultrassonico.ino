// Código utilizado para o Sensor Ultrassônico

#include <PubSubClient.h>
#include <Ultrasonic.h>
#include <WiFi.h>

const char* mqttUser = "";
const char* mqttPassword = "";
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "test.mosquitto.org";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int trigPin = 18;
const int echoPin = 12;
Ultrasonic ultrasonic(trigPin, echoPin);

void callback(char* topic, byte* payload, unsigned int length) {

}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Tentando se conectar ao MQTT...");
        if (client.connect("ESP32_Sensor", mqttUser, mqttPassword)) {
            Serial.println("Conectado ao MQTT!");
        } else {
            Serial.print("Falha na conexão. Erro: ");
            Serial.print(client.state());
            delay(2000);
        }
    }
}
void setup() {
    Serial.begin(115200);

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println("Conectado ao WiFi");
}

void loop() {

    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    long distance = ultrasonic.read(CM);

    char message[50];
    snprintf(message, sizeof(message), "Distancia: %ld cm", distance);
    client.publish("altura", message);

    delay(1000);
}
