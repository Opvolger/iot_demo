# Setup

Arch / Manjaro (rights to com ports) [link](https://wiki.archlinux.org/title/working_with_the_serial_console#Connect_using_a_terminal_emulator_program)

```bash
sudo usermod -a -G uucp $USER
```

## Install Software

- paho-mqtt (python-paho-mqtt)
- Arduino
- ch34x-dkms-git (driver for Upload to esp8266)

## Arduino

Install [link](http://blog.hackerstore.nl/esp8266-programmeren/)

add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to Board manager

Download ESP8266 Board

Download Library `PubSubClient`. [link](https://github.com/knolleary/pubsubclient)

## Docker-Compose

[mosquitto](https://github.com/vvatelot/mosquitto-docker-compose)
