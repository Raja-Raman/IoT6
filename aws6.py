# Get occupancy data from AWS cloud

from random import randint
import requests
import json
import time
import sys

url = 'http://52.40.239.77:5000/get_latest_data'

counter = 0
print ("Getting latest room occupancy status from AWS cloud..")
print ("Press ^C to exit...\n\n");
while True:
    try:
        print (counter)
        counter = (counter+1)%100           
        response = requests.get(url)  
        code = response.status_code
        #print (code)
        #print (response.headers)
        #print (str(counter) + "\t" + response.text)  
        jdata = json.loads(response.text)
        for key in jdata.keys():
            jdata2 = json.loads(jdata[key].replace("'", "\""))
            print (key +"\t"+ str(jdata2['status']) +"\t"+ time.ctime(jdata2['time']))   
        if (code > 300):
             print('---GET failed----')
        time.sleep (10.0)              
    except KeyboardInterrupt:
        break    
    except Exception as e:
        print ("-- Could not get data from cloud --")   
        print (e) 
        time.sleep (10.0)
        
print ("Bye !")
