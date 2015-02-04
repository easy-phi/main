#######-------------------------------------------------------------------------------------
#######Easy-phi project: http://easy-phi.ch/
#######Developer : Raphael Houlmann, University of Geneva 2014
#######Licensed under GPL, for details report to: http://easy-phi.ch/index.php/about/licensing
#######Do NOT remove this notice
#######-------------------------------------------------------------------------------------
import commands
import re
import zmq
import random
import sys
import time
import serial 
import struct
import threading
import logging
logging.basicConfig(filename='/var/log/easy-phi.log',level=logging.DEBUG)

class ModuleThread(threading.Thread):
    def __init__(self, number, usb_port, error_queue):
        threading.Thread.__init__(self)
        self.number = number
        self.usb_port = usb_port
        self.error_queue = error_queue
        
        self.zmq_port = 5556 + self.number
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REP)
        self.socket.bind("tcp://*:%s" % self.zmq_port)

        self.ser = serial.Serial()
        self.ser.baudrate = 3000000
        self.ser.timeout = 0.1
        
        self.ser.port = "noport"
        self.board_present = 0
        self.status = "Empty socket"
        self.debug = 0
        self.connected = 0
        self.lock = "locked"

        self.name = "none"
        self.description = "none"
        self.version = "none"
        self.type = "none"
        self.serial_number = "none"
       
        
    def connect(self):
        try:
            self.ser.open()
            self.connected = 1
            logging.info("Connected port %d" % (self.number + 1))
            self.status = "Power On"
            self.lock = "unlocked"
            
        except (serial.SerialException):
            self.ser.close()
            self.connected = 0
            self.board_present = 0
            self.status = "Empty socket"
            logging.warning("Cannot connect port %d " % (self.number + 1))                    
            self.error_queue.put("Cannot connect port: %d" % (self.number + 1))
        
    def disconnect(self):
        logging.warning("Disconnect: Lost connection on port %d " % (self.number + 1))
        self.error_queue.put("Lost connection on port: %d" % (self.number + 1))
        self.status = "Empty socket"
        self.lock = "locked"
        self.name = "none"
        self.description = "none"
        self.version = "none"
        self.type = "none"
        self.serial_number = "none"
        self.board_present = 0
        
       
    def run(self):
        while 1:
            msg = self.socket.recv()          #receive message
            
            if (self.connected == 1):
                try:
                    self.ser.write(msg)
                    if (self.debug == 1): logging.debug(msg)                    
                    self.ser.write(chr(13))
                except (serial.SerialException):
                    self.ser.close()
                    self.connected = 0
                    self.board_present = 0
                    self.status = "Empty socket"

                if (self.connected == 0): self.disconnect()

                    
            if re.search('\?', msg):       #if message is a read command, wait for answer and return it otherwise return ok
                if (self.connected == 1):
                    loop_cnt = 0
                    text = ''
                    char = ''
                    while (char != chr(13)):
                        try:
                            char = self.ser.read(1)
                        except:
                            self.ser.close()
                            self.connected = 0
                            self.board_present = 0
                            self.status = "Empty socket"
                            break
        
                        if (char != chr(10)):  text = text + char
                        loop_cnt = loop_cnt + 1
                        if (loop_cnt == 1000):
                            self.error_queue.put("Error: Timeout on read, port: %d with message: %s" % ((self.number + 1), msg ))
                            logging.warning("Error: Timeout on read, port: %d with message: %s" % ((self.number + 1), msg ))
                            self.socket.send("Error: Timeout on read")
                            break
                        if (self.connected == 0): self.disconnect()
                            
        
                if (self.connected == 1):
                    self.socket.send(text)
                    if (self.debug == 1): logging.debug(text)
                else:    
                    self.socket.send("Error: No access to serial port")
                    self.error_queue.put("No access to serial port on port: %d" % (self.number + 1))
            else:    
                if (self.connected == 1):
                    self.socket.send("OK")
                else:
                    self.socket.send("Error: No access to serial port")

