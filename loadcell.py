import serial
import string
import time
import subprocess
import artikcloud
from artikcloud.rest import ApiException
from pprint import pprint


#cloud
device_token = "256d57fbb481455abda36ed8563b59d4"
device_id = "34e6d7c9bc754fd294f96bae4a786ba9"

artikcloud.configuration = artikcloud.Configuration();
artikcloud.configuration.access_token = device_token

gauges2 = serial.Serial()
gauges2.port = '/dev/rfcomm1'
gauges2.baudrate = 9600
gauges2.parity = 'N'
gauges2.writeTimeourt = 0
gauges2.timeout = 3
gauges2.open()

print("Connected to " + gauges2.portstr)

while True:
	weight = 0
	try:
		get_str = gauges2.readline()
		get_str.replace("\n","")
		get_str.replace("\r","")
		if (0 < len(get_str)):
			weight = float(get_str)
			print(str(weight))
	except KeyboardInterrupt:
		break
	except:
		pass

	if 100 < weight:
		# Create an instance of the API Class
		api_instance = artikcloud.MessagesApi()
		device_message = {}
		device_message['weight'] = weight
		device_sdid = device_id
		ts = None                         

		# Construct a Message Object for request
		data = artikcloud.Message(device_message, device_sdid, ts)

		try:
			api_response = api_instance.send_message(data) # Send Message
		except ApiException as e:
			pprint("Exception when calling MessagesApi->send_message: %s\n" % e)

	time.sleep(1)

gauges2.close()