import argparse
import datetime
import time
import traceback
import sys
import pprint
from time import sleep
from concurrent.futures import ThreadPoolExecutor
import powersensor
import time 

def logSensor(logname, port):
    try:
        sensor = powersensor.BTPOWER(port)
        start = time.time() 
        logfile = open(logname, "w")
        while(1):
			pwr_val = sensor.readPower()
			elapsed_time = time.time() - start 
			#print(str(elapsed_time) + ", " + str(pwr_val))
			logfile.write(str(elapsed_time) + ";" + str(pwr_val) + "\n")
			logfile.flush()
        sensor.close()
    except:
        print("Problem opening sensors - try with SUDO!!!")
        traceback.print_exc()
        exit(0) 

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--log", default="power.csv", help="sensor type: temp, power, host", required=False)
    parser.add_argument("--port", help="port to which the sensor is connected", required=True)
    
    args = vars(parser.parse_args())

    logSensor(args['log'], args['port'])

    
