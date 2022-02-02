#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//WiFi
const char *ssid = "PraiseThe5un_EXT";
const char *password = "jebacjaslo";

//MQTT
const char *mqtt_broker = "192.168.31.207";
const char *topic = "test/topic";
const int mqtt_port = 1883;

WiFiClient wifi_client;
PubSubClient mqtt_client(wifi_client);

void callback(char *topic, byte *payload, unsigned int length) {
	Serial.print("Message received in topic: ");
	Serial.println(topic);
	Serial.print("Message: ");

	for (unsigned int i = 0; i < length; i++)
		Serial.print((char)payload[i]);

	Serial.println();
	Serial.println("----------------");
}

void setup() {
	Serial.begin(9600);

	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.println("Connecting to WiFi...");
	}

	Serial.println("Connected to WiFi");

	mqtt_client.setServer(mqtt_broker, mqtt_port);
	mqtt_client.setCallback(callback);

	while (!mqtt_client.connected()) {
		String client_id = "esp8266-client-";
		client_id += String(WiFi.macAddress());

		Serial.printf("The client %s connecting to mosquitto mqtt broker\n", client_id.c_str());

		if (mqtt_client.connect(client_id.c_str())) {
			Serial.println("Public emqx mqtt broker connected");
		} else {
			Serial.print("Failed with state: ");
			Serial.print(mqtt_client.state());
			delay(2000);
		}
	}

	mqtt_client.publish(topic, "Hello from ESP8266!");
	mqtt_client.subscribe(topic);
}

void loop() {
	mqtt_client.loop();
}
