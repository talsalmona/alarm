# Alarm Hack
Hacking a [Crow Runner](http://www.thecrowgroup.com/Products_Systems/runner_8_16/runner_8_16/) alarm system with esp8266

## Setup
Currently I connected into just the door sensor. 
Reading the volage on the analog pin (A0) and sending MQTT 'open' in the value is above 500 or 'closed' otherwise.

## Home Assistant Settings
```YAML
sensor:
  - platform: mqtt
    name: "Door"
    state_topic: "sensor/door" 
```

## Future work
Hack more sensors through the digital pins
