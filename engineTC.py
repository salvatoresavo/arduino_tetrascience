import time
import requests
import json
import math

# device information
device_apikey = 'KTVwgiYj6awRBSEIXvCHm0ePmEuVXea0qTEX80Gp1lhCjLeQ'
dev_url = "http://localhost:10080/api/log/tcdata"
device_name = 'sth@gmail.com_Thermocouple'
device_type = 'TC'


serial_port = '/dev/ttyACM0'
serial_bauds = 9600
#device_name = 'thermal couple'

print 'Opening serial port'
#s = Serial(serial_port,serial_bauds)
print 'Start reading data from ' + device_name
#line = s.readline()
while 1:
	#line = s.readline()
	#print line
	period = 50.0
	amp = 30.0
	for count in range(int(-period/2+1), int(period/2)):
		tmp = round(amp*math.sin(count/period*2*math.pi))
		print tmp
		payload = {
		    'device_type' : device_type,
		    'device_name' : device_name,
		    'device_apikey' : device_apikey,
		    'value' : tmp
		}
		r = requests.post(dev_url, data = payload)
		print(r.text)	        
		time.sleep(4)



