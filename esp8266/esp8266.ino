// https://randomnerdtutorials.com/esp8266-relay-module-ac-web-server/
// https://community.home-assistant.io/t/arduino-relay-switches-over-wifi-controlled-via-hass-mqtt-comes-with-ota-support/13439/11
// https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/
// http://blog.hackerstore.nl/esp8266-programmeren/
// https://github.com/knolleary/pubsubclient

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

void callback(char* topic, byte* payload, unsigned int length);

const int pin_led = 2;
const int pin_relay = 0;

#define MQTT_SERVER "192.168.8.99"
#define MQTT_PORT 1883
const char* mqtt_username = "etty";
const char* mqtt_password = "wachtwoord";

const char* ssid = "etty";
const char* wifi_password = "demodemo";
const char* topic_switch = "etty/lamp";

unsigned long previousMillis = 0;
unsigned long interval = 30000;

WiFiClient wifiClient;
PubSubClient mqttClient(MQTT_SERVER, MQTT_PORT, callback, wifiClient);

void setup() {
  pinMode(pin_led, OUTPUT);
  pinMode(pin_relay, OUTPUT);
  
  // Serial text (for debugging)
  Serial.begin(115200);

  // Connect to WiFi
  while (WiFi.begin(ssid, wifi_password) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {

  // convert topic to string to make it easier to work with
  String topicStr = topic;

  // Print out some debugging info
  Serial.println("Callback update.");
  Serial.print("Topic: ");
  Serial.println(topicStr);

  if (topicStr == topic_switch)
  {
    char stringFirstChar = (char)payload[0];
    Serial.print("payload: ");
    Serial.println(stringFirstChar);
    if (stringFirstChar == '1') {
      //turn the switch on if the payload is '1' and publish to the MQTT server a confirmation message
      digitalWrite(pin_led, LOW); //the LED is on; this is because it is acive low on the ESP8266.
      digitalWrite(pin_relay, HIGH);
      Serial.println("Switch On");
    } else if (stringFirstChar == '0') {
      //turn the switch off if the payload is '0' and publish to the MQTT server a confirmation message
      digitalWrite(pin_led, HIGH); //the LED is off; this is because it is acive low on the ESP8266.
      digitalWrite(pin_relay, LOW);
      Serial.println("Switch Off");
    }
  }
}

void reconnectMQTT() {
  //make sure we are connected to WIFI before attemping to reconnect to MQTT
  if (WiFi.status() == WL_CONNECTED) {
    // Loop until we're reconnected to the MQTT server
    while (!mqttClient.connected()) {
      Serial.print("Attempting MQTT connection...");

      // Generate client name based on MAC address and last 8 bits of microsecond counter
      String clientName;
      clientName += "esp8266-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);

      // if connected, subscribe to the topic(s) we want to be notified about
      if (mqttClient.connect((char*) clientName.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("\tMQTT Connected");
        mqttClient.subscribe(topic_switch);
      } else {
        Serial.println("\tFailed.");
      }
    }
  }
}

void loop() {
  // reconnect if connection is lost WiFi
  unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= interval)) {
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    previousMillis = currentMillis;
  }

  // reconnect if connection is lost MQTT
  if (!mqttClient.connected() && WiFi.status() == 3) {
    Serial.println("(Re)connect to network MQTT");
    reconnectMQTT();
  }

  // maintain MQTT connection
  mqttClient.loop();

  //MUST delay to allow ESP8266 WIFI functions to run
  delay(10);
}

//generate unique name from MAC addr
String macToStr(const uint8_t* mac) {

  String result;

  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5) {
      result += ':';
    }
  }

  return result;
}
