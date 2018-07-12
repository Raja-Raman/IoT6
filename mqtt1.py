# MQTT publisher
# https://wildanmsyah.wordpress.com/2017/05/11/mqtt-android-client-tutorial/#publisher

# pip install paho-mqtt

import paho.mqtt.client as mqtt
from random import randint
import threading
import json
import time
import sys

server = 'broker.mqtt-dashboard.com'
port = 1883
topic = 'rajaramans/python/program/1'

mqttc = mqtt.Client("raman_rajas_client_1963", clean_session=False)
#mqttc.username_pw_set("User", "password")
mqttc.connect(server, port, 60)
print ('Press ^C to quit...')

def pub():
    mqttc.publish(topic, payload="hello from Python "+str(randint(10,100)), qos=0)
    threading.Timer(5, pub).start()  # self-repeating

pub()  # priming
print ('Bye !')