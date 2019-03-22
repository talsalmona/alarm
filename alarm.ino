
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define ssid "salmona"
#define password "***"
#define mqtt_server "192.168.86.**"
#define door_topic "sensor/door"

WiFiClient espClient;
PubSubClient mqttClient(espClient);
 
int DOOR = A0; //Analog channel A0 used to measure sensor voltqge (open is about 508)
 
void setup() {
  Serial.begin(9600);
  startWifi();
  mqttClient.setServer(mqtt_server, 1883);
}

void startWifi() {
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected: ");
  Serial.println(WiFi.localIP());  
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("AlarmClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}
 
void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  
  float value = 0.0;    
  for(unsigned int i=0; i<10; i++){
    value += analogRead(DOOR);     //Read analog Voltage
    delay(5);                      //ADC stable
  }
  value /= 10.0;                   //Find average of 10 values
  //value = (value/1024.0);          //Convert Voltage in 5v factor

  Serial.print("value: ");
  Serial.println(value);
 
  if(value <= 400){
    mqttClient.publish(door_topic, "closed");
    Serial.println("Sent door closed MQTT message");
  }
  else {
    mqttClient.publish(door_topic, "open");
    Serial.println("Sent door open MQTT message");
  }
     
  delay(1000); 
}
