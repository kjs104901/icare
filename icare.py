import serial
import string
import time
import subprocess
import artikcloud
from artikcloud.rest import ApiException
from pprint import pprint

try:
	from subprocess import DEVNULL # py3k
except ImportError:
	import os
	DEVNULL = open(os.devnull, 'wb')

#cloud
device_token = "256d57fbb481455abda36ed8563b59d4"
device_id = "34e6d7c9bc754fd294f96bae4a786ba9"

artikcloud.configuration = artikcloud.Configuration();
artikcloud.configuration.access_token = device_token

# setting
subprocess.call ('rfcomm release 0', shell=True)
subprocess.call ('rfcomm bind 0 98:D3:31:F6:0D:EA', shell=True)

subprocess.call ('rfcomm release 1', shell=True)
subprocess.call ('rfcomm bind 1 98:D3:31:F6:0C:D8', shell=True)

# bluetooth set
gauges = serial.Serial()
gauges.port = '/dev/rfcomm0'
gauges.baudrate = 9600
gauges.parity = 'N'
gauges.writeTimeout = 0
gauges.timeout = 3
gauges.open()

print("Connected to " + gauges.portstr)

#streaming
youtube_str = 'ffmpeg -f alsa -ac 2 -i hw:0,0 -f v4l2 -s 640x480 -r 10 -i /dev/video1 -vcodec libx264 -pix_fmt yuv420p -preset ultrafast -r 5 -g 20 -b:v 2500k -codec:a libmp3lame -ar 44100 -threads 6 -b:a 11025 -bufsize 512k -f flv rtmp://a.rtmp.youtube.com/live2/bqzm-4bz8-0ubj-9mew'
subprocess.Popen (youtube_str, shell=True, stdin=subprocess.PIPE, stdout=DEVNULL, stderr=subprocess.STDOUT)

# sound
lullaby_str = 'aplay lullaby.wav'
lullaby_proc = subprocess.Popen (lullaby_str, shell=True, stdout=subprocess.PIPE)
lullaby_proc.kill()

# eyedetect
cmd = ['./eyedetect']
proc = subprocess.Popen(['stdbuf', '-o0'] + cmd, stdout=subprocess.PIPE);

#loadcell
loadcell_str = 'python loadcell.py'
subprocess.Popen (loadcell_str, shell=True, stdin=subprocess.PIPE, stdout=DEVNULL, stderr=subprocess.STDOUT)

waken = False
weight_count = 0
while True:
	line = proc.stdout.readline()
	line_str = str(line.rstrip())
	print(line_str);
	#proc.stdout.flush()

	if line != '':
		if (line_str == "1"):
			gauges.write('1')
			waken = True
			print("mortor")

			if (lullaby_proc.poll() is not None):
				lullaby_proc = subprocess.Popen (lullaby_str, shell=True, stdout=subprocess.PIPE)
		else :
			gauges.write('0')
			print("sleeping")

	if waken:
		# Create an instance of the API Class
		api_instance = artikcloud.MessagesApi()

		action_arr = {"actions":[{"name": "wake"}]}
		device_ddid = device_id
		device_sdid = device_id
		ts = None                         

		# Construct a Message Object for request
		data = artikcloud.Actions(action_arr, device_ddid, device_sdid, ts)

		try:
			api_response = api_instance.send_message(data) # Send Message
		except ApiException as e:
			pprint("Exception when calling MessagesApi->send_message: %s\n" % e)

		waken = False

	continue
	weight = 0
	try:
		get_str = gauges2.readline()
		get_str.replace("\n","")
		get_str.replace("\r","")
		if (0 < len(get_str)):
			weight = float(get_str)
			print(str(weight)+" g(new)")
			weight_count += 1
	except KeyboardInterrupt:
		break
	except:
		print("no data from blue")
		pass

	print(str(weight)+" g")
	if 3 < weight_count:
		weight_count = 0
		if 300 < weight:
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

gauges.close()