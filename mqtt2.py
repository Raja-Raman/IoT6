# MQTT publisher
# https://wildanmsyah.wordpress.com/2017/05/11/mqtt-android-client-tutorial/#publisher
# see it at http://www.hivemq.com/demos/websocket-client/

# pip install paho-mqtt

import paho.mqtt.client as mqtt
from random import randint
import threading
import json
import time
import sys

#------------------------------ globals ------------------------------
server = 'broker.mqttdashboard.com'
# server = 'broker.mqtt-dashboard.com'  # this also works
# port = 8000
port = 1883
client = None
topic = 'Ind/Che/Vel/Maa/407/Command'
terminate = False

#------------------------------ publisher ----------------------------

def pub():
    msg = "hello from Python "+str(randint(10,100))
    print (msg)
    client.publish(topic, payload=msg, qos=0)
    if terminate:
        print ('Worker thread quits..')
    else:
        threading.Timer(5, pub).start()  # self-repeating
        
#---------------------------- main -----------------------------------

client = mqtt.Client("raman_rajas_client_1963", clean_session=False)
# client.username_pw_set("User", "password")
res = client.connect(server, port, 60)
print (res)

print ('Press ^C to quit...')
pub()  # priming

try:
    while (True):
        time.sleep(0.5)
except KeyboardInterrupt:
    print ('Keyboard interrupt !')
    
terminate = True       
time.sleep(5.5)    
print ('Main thread quits.')
