# MQTT client, publishes a single message and quits
# https://www.hivemq.com/blog/mqtt-client-library-paho-python
# see it at http://www.hivemq.com/demos/websocket-client/

# pip install paho-mqtt

from random import randint
import paho.mqtt as mqtt
import paho.mqtt.publish as publish

server = 'broker.mqttdashboard.com'
port = 1883
topic1 = 'Ind/Che/Vel/Maa/407/Command'

msg = "monoshot message #"+str(randint(1,100))
print (msg)

'''
publish.single(topic1, payload=msg, 
            hostname=server, port=port, 
            qos=0, retain=True,             
            client_id="rajas_monoshot_client_1994", keepalive=60, 
            will=None, auth=None,tls=None) 
'''

# Aliter: minimalistic:
publish.single(topic1, payload=msg, hostname=server, port=port)

print ('Sinlge shot fired !')

