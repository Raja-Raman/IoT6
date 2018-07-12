# MQTT client, publishes a bunch of message in one go
# https://www.hivemq.com/blog/mqtt-client-library-paho-python
# see it at http://www.hivemq.com/demos/websocket-client/

# pip install paho-mqtt

from random import randint
import paho.mqtt as mqtt
import paho.mqtt.publish as publish

server = 'broker.mqttdashboard.com'
port = 1883
topic1 = 'Ind/Che/Vel/Maa/407/Command'

msg1 = {'topic':topic1, 'payload':"multishot message #"+str(randint(1,100)) }
msg2 = {'topic':topic1, 'payload':"multishot message #"+str(randint(1,100)), 'qos':1 }
msg3 = {'topic':topic1, 'payload':"multishot message #"+str(randint(1,100)), 'retain':True }
msg4 = {'topic':topic1, 'payload':"multishot message #"+str(randint(1,100)), 'qos':0, 'retain':False }

messages = [msg1,msg2,msg3,msg4]
print (messages)

publish.multiple(messages, hostname=server, port=port)

print ('A volley of shots fired !')

