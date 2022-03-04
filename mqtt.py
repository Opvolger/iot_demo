# https://pypi.org/project/paho-mqtt/

import paho.mqtt.client as mqtt
from tenacity import sleep

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("etty/lamp")
    client.subscribe("etty/temp")
    result = client.publish("etty/lamp", 1)
    # status = result[0]
    # if status == 0:
    #     print(f"Send to topic")
    # else:
    #     print(f"Failed to send message to topic")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set(username="etty", password="wachtwoord")
client.connect("192.168.8.99", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.

# client.loop()
# sleep(1)
# client.loop()
client.loop_forever()