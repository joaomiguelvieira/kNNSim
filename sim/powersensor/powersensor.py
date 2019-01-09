import struct
import serial
import time
import traceback


class BTPOWER:
    setAddrBytes =     [0xB4, 0xC0, 0xA8, 0x01, 0x01, 0x00, 0x1E]
    readVoltageBytes = [0xB0, 0xC0, 0xA8, 0x01, 0x01, 0x00, 0x1A]
    readCurrentBytes = [0XB1, 0xC0, 0xA8, 0x01, 0x01, 0x00, 0x1B]
    readPowerBytes =   [0XB2, 0xC0, 0xA8, 0x01, 0x01, 0x00, 0x1C]
    readEnergyBytes =  [0XB3, 0xC0, 0xA8, 0x01, 0x01, 0x00, 0x1D]

    #http://digital.ni.com/public.nsf/allkb/D37754FFA24F7C3F86256706005B9BE7 baud rates
    #https://www.amazon.com/exec/obidos/ASIN/B015PRF04I/marinetrader-20
    #http://www.droking.com/digital-acdc-multimeter-4-data-display-voltage-current-power-energy/a
    def __init__(self, com="COM7", timeout=5.0):
        self.ser = serial.Serial(
            port=com,
            baudrate=9600,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS,
            timeout=timeout
        )
        if self.ser.isOpen():
            self.ser.close()
        self.ser.open()
        print "power sensor open"

    def checkChecksum(self, _tuple):
        _list = list(_tuple)
        _checksum = _list[-1]
        _list.pop()
        _sum = sum(_list)
        if _checksum == _sum % 256:
            return True
        else:
            raise Exception("Wrong checksum")

    def isReady(self):
        self.ser.write(serial.to_bytes(self.setAddrBytes))
        rcv = self.ser.read(7)
        if len(rcv) == 7:
            unpacked = struct.unpack("!7B", rcv)
            if (self.checkChecksum(unpacked)):
                return True
        else:
            raise serial.SerialTimeoutException("Timeout setting address")

    def readVoltage(self):
        self.ser.write(serial.to_bytes(self.readVoltageBytes))
        rcv = self.ser.read(7)
        if len(rcv) == 7:
            unpacked = struct.unpack("!7B", rcv)
            if (self.checkChecksum(unpacked)):
                tension = unpacked[2] + unpacked[3] / 10.0
                return tension
        else:
            raise serial.SerialTimeoutException("Timeout reading tension")

    def readCurrent(self):
        self.ser.write(serial.to_bytes(self.readCurrentBytes))
        rcv = self.ser.read(7)
        if len(rcv) == 7:
            unpacked = struct.unpack("!7B", rcv)
            if (self.checkChecksum(unpacked)):
                current = unpacked[2] + unpacked[3] / 100.0
                return current
        else:
            raise serial.SerialTimeoutException("Timeout reading current")

    def readPower(self,a=None):
        self.ser.write(serial.to_bytes(self.readPowerBytes))
        rcv = self.ser.read(7)
        if len(rcv) == 7:
            unpacked = struct.unpack("!7B", rcv)
            if (self.checkChecksum(unpacked)):
                power = unpacked[1] * 256 + unpacked[2]
                return power
        else:
            print "Timeout reading power"
            # raise serial.SerialTimeoutException("Timeout reading power")

    def readEnergy(self):
        self.ser.write(serial.to_bytes(self.readEnergyBytes))
        rcv = self.ser.read(7)
        if len(rcv) == 7:
            unpacked = struct.unpack("!7B", rcv)
            if (self.checkChecksum(unpacked)):
                energy = unpacked[1] * 256 * 256 + unpacked[2] * 256 + unpacked[3]
                return energy
        else:
            raise serial.SerialTimeoutException("Timeout reading registered power")

    def readAll(self,a):
        if (self.isReady()):
			try:
				v, c, p, e = self.readVoltage(), self.readCurrent(), self.readPower(), self.readEnergy()
				return v, c, p, e
			except:
				err = str(time.time())+" failure reading all parameters of power sensor [POWER]"
				print(err)
				traceback.print_exc()
				v, c, p, e = "", "", "", "" # return none by default
				return v, c, p, e

    def close(self):
        self.ser.close()