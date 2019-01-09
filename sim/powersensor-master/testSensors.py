import argparse
import datetime
import time
import traceback
import sys
import pprint
from time import sleep
from concurrent.futures import ThreadPoolExecutor
import powersensor

def testSensor(sensorname):
    try:
        if sensorname == 'power':
            sensor = powersensor.BTPOWER()
        else:
            print("Choose one: power, all")
            sys.exit(0)

        pool = ThreadPoolExecutor(1)
        futsensor = pool.submit(sensor.readAll, "True")
        futsensorres = futsensor.result()
        sensor.close()
        print("Sensor output:")
        print(futsensorres)
    except:
        print("Problem opening sensors - try with SUDO!!!")
        traceback.print_exc()
        exit(0) 

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--sensor", default="all", help="sensor type: temp, power, host", required=False)
    
    args = vars(parser.parse_args())

    if args['sensor']=='all':
        for i in ['power']:
            print("Sensor: "+i)
            testSensor(i)
    else:
        testSensor(args['sensor'])

    
