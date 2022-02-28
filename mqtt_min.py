# https://pypi.org/project/paho-mqtt/

import paho.mqtt.client as mqtt
from tenacity import sleep

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.publish("etty/lamp", 1)


client = mqtt.Client()
client.on_connect = on_connect
client.username_pw_set(username="etty", password="wachtwoord")
client.connect("192.168.2.22", 1883, 60)

client.loop_start()
sleep(1) # wait for on_connect
client.loop_stop()
