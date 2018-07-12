# MQTT client - publisher
# https://www.hivemq.com/blog/mqtt-client-library-paho-python
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
#topic1 = 'ind/che/vel/maa/407/com'
topic1 = 'ind/che/vel/maa/407/soi'
topic2 = "ind/che/vel/maa/407/res"
terminate = False

#------------------------------ callbacks ---------------------------- 

def on_connect(client, userdata, flags, rc):
    print('CONN-ACK received with code %d.' % (rc))
    print('With user data: %s.' % userdata)
 
def on_publish(client, userdata, mid):
    print("published msg id: "+str(mid))
    print('With user data: %s.' % userdata)

#---------------------------- main -----------------------------------
     
# client = mqtt.Client("raman_rajas_client_1963", clean_session=False)
# client.username_pw_set("User", "password")     
client = mqtt.Client(userdata = 'Raja\'s callback events-> data')
client.on_connect = on_connect
client.on_publish = on_publish

# optional: LWT - Last Will and Testement
client.will_set(topic1, payload='I am dead and gone!', qos=0, retain=False)

#client.connect(server, port, keepalive=60)    # blocking call
# Aliter:
client.connect_async(server, port, keepalive=60)  # non blocking

# client.loop_forever()   # blocking call - reconnects automatically (useful esp. for mqtt listeners)
client.loop_start()       # start a background thread (useful if you are also an mqtt sender)

time.sleep(2)
print ('Press ^C to quit...')
try:
    while not terminate:
        #msg = "hello from Python "+str(randint(1,100))
        msg = str(randint(1,100))
        print (msg)
        (rc, mid) = client.publish(topic1, payload=msg, qos=0, retain=True)
        print (rc, mid)
        time.sleep(7)
except KeyboardInterrupt:
    print ('Keyboard interrupt !')
    
client.loop_stop()   # kill the background thread   
time.sleep(1) 
print ('Main thread quits.')
