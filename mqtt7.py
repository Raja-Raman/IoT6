# MQTT client - clean subscriber
# https://www.hivemq.com/blog/mqtt-client-library-paho-python
# see it at http://www.hivemq.com/demos/websocket-client/

# pip install paho-mqtt

import paho.mqtt.client as mqtt
import time
import sys

#------------------------------ globals ------------------------------
server = 'broker.mqttdashboard.com'
port = 1883
client = None
topic1 = 'Ind/Che/Vel/Maa/407/Command'
topic2 = "Ind/Che/Vel/Maa/407/Response"

#------------------------------ callbacks ---------------------------- 

def on_connect(client, userdata, flags, rc):
    print('Connected to MQTT server')
    client.subscribe (topic1, qos=0)  # on reconnection, automatically renew
 
def on_publish(client, userdata, mid):
    print("Published msg id: "+str(mid))

def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed; mid="+str(mid)+", granted QOS="+str(granted_qos))
    
def on_message(client, userdata, msg):
    print(msg.topic+"-> " +str(msg.payload)) 
    
#---------------------------- main -----------------------------------
     
client = mqtt.Client("raman_rajas_client_1963", clean_session=False)
# client.username_pw_set("User", "password")     

client.on_connect = on_connect
# client.on_publish = on_publish
client.on_subscribe = on_subscribe
client.on_message = on_message

client.connect(server, port, keepalive=60)    # temporarily blocking 
#client.subscribe (topic1, qos=0)  # done when connection materializes
print ('Press ^C to quit...')

client.loop_forever()    # blocking call - reconnects automatically (useful esp. for mqtt listeners)
  
print ('Main thread quits.')  # will not be called
