#!/usr/bin/python

import serial
import time
import requests
import json
import Adafruit_GPIO.SPI as SPI
import Adafruit_MAX31855.MAX31855 as MAX31855

device_name = 'sth@gmail.com_Thermocouple'
interval = 3
dev_url = "http://192.168.1.10:10080/api/log/tcdata"
device_type = 'TC'
device_apikey = 'dsf9usadf['

apiKey = 'KTVwgiYj6awRBSEIXvCHm0ePmEuVXea0qTEX80Gp1lhCjLeQ'
feedID = '83561687'
xively_url = "https://api.xively.com/v2/feeds/"+feedID+".json"
print xively_url


CLK = 25
CS = 24
DO = 18

sensor = MAX31855.MAX31855(CLK,CS,DO)


while 1:
        tmp = sensor.readTempC()

        print tmp
        line = s.readline()
      	print line
	      upload = { "version":"1.0.0","datastreams":[{"id":"Temperature","current_value":tmp}]}
      	upload_json = json.dumps(upload)
      	headers = {'X-ApiKey': apiKey}
      	r = requests.put('https://api.xively.com/v2/feeds/'+feedID+'.json', data=upload_json, headers = headers)
	      time.sleep(interval)
