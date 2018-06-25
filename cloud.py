import artikcloud
from artikcloud.rest import ApiException
from pprint import pprint

#cloud
device_token = "256d57fbb481455abda36ed8563b59d4"
device_id = "34e6d7c9bc754fd294f96bae4a786ba9"

artikcloud.configuration = artikcloud.Configuration()
artikcloud.configuration.access_token = device_token


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
