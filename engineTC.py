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

CLK = 25
CS = 24
DO = 18

sensor = MAX31855.MAX31855(CLK,CS,DO)


while 1:
        tmp = sensor.readTempC()

        print tmp
        payload = {
                'device_type' : device_type,
                'device_name' : device_name,
                'device_apikey' : device_apikey,
                'value' : tmp
        }

        try:
                r = requests.post(dev_url, data = payload)
                print(r.text)
                r = requests.post(dev_url, data = payload)
                print(r.text)
        except requests.exceptions.ConnectionError:
                print 'Connection issue, restart..'

        time.sleep(interval)

