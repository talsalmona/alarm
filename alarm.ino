


#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define ssid "***"
#define password "***"
#define mqtt_server "***"
#define door_topic "sensor/door"
#define mqtt_username "***"
#define mqtt_password "***"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

int DOOR = A0; //Analog channel A0 used to measure sensor voltqge (open is about 508)
boolean isOpen = false;

void setup() {
  Serial.begin(115200);
  startWifi();
  mqttClient.setServer(mqtt_server, 1883);
}

void startWifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());
}

void reconnectIfNeeded() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("AlarmClient", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

void runMqttLoop() {
  reconnectIfNeeded();
  mqttClient.loop();
}

int readDoorInput() {
  float value = 0.0;
  for(unsigned int i=0; i<10; i++){
    value += analogRead(DOOR);     //Read analog Voltage
    delay(5);                      //ADC stable
  }
  value /= 10.0;                   //Find average of 10 values
  return value;
}

void sendDoorStatusChangeIfNeeded() {
  boolean shouldSend = false;
  int value = readDoorInput();

  if (value <= 400) { //closed
    shouldSend = isOpen;
    isOpen = false;
  }
  else { // open
    shouldSend = !isOpen;
    isOpen = true;
  }

  if (shouldSend) {
    String status = isOpen ? "open" : "closed";
    mqttClient.publish(door_topic, status.c_str());
    Serial.print("sent ");
    Serial.println(status);
  }
}

void loop() {
  runMqttLoop();
  sendDoorStatusChangeIfNeeded();
  delay(1000);
}
