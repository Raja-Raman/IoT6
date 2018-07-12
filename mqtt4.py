# MQTT client - subscriber
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
terminate = False

#------------------------------ callbacks ---------------------------- 

def on_connect(client, userdata, flags, rc):
    print('CONN-ACK received with code %d.' % (rc))
    print('With user data: %s.' % userdata)
 
def on_publish(client, userdata, mid):
    print("published msg id: "+str(mid))
    print('With user data: %s.' % userdata)

def on_subscribe(client, userdata, mid, granted_qos):
    print("Subscribed; mid="+str(mid)+", granted QOS="+str(granted_qos))
    print('With user data: %s.' % userdata)
    
def on_message(client, userdata, msg):
    print('Received msg with user data: {}, QOS: {}'.format(userdata,msg.qos))
    print(msg.topic+"-> " +str(msg.payload)) 
    
#---------------------------- main -----------------------------------
     
# client = mqtt.Client("raman_rajas_client_1963", clean_session=False)
# client.username_pw_set("User", "password")     
client = mqtt.Client(userdata = 'Raja\'s data')

client.on_connect = on_connect
# client.on_publish = on_publish
client.on_subscribe = on_subscribe
client.on_message = on_message

# optional: LWT - Last Will and Testement
# client.will_set(topic1, payload='I am dead and gone!', qos=0, retain=False)

client.connect(server, port, keepalive=60)    # blocking call
# Aliter:
# client.connect_async(server, port, keepalive=60)  # non blocking

client.subscribe (topic1, qos=0)
print ('Press ^C to quit...')

client.loop_forever()    # blocking call - reconnects automatically (useful esp. for mqtt listeners)
  
print ('Main thread quits.')
