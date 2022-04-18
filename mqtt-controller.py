import time
import paho.mqtt.client as mqtt

def on_message(client, userdata, message):
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)
broker_address="www.kisonhe.xyz"

client = mqtt.Client("mqttInstance") #create new instance
client.on_message=on_message #attach function to callback
client.connect(broker_address,port=1883) #connect to broker
client.loop_start() #start the loop
client.subscribe("lol")
client.subscribe("lol")
client.publish("lol","payload")
time.sleep(1)

client.loop_stop() #stop the loop