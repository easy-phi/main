#!/usr/bin/python
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
import os
import serial 
import struct
import threading
import Queue
from module_thread import *
from message_parser import *

        
class main_server():
    def __init__(self):
        self.number_of_ports = 5
        ###self.usb_ports = ['1.1', '1.4.1', '1.4.2', '1.4.3', '1.4.4']  ### first prototype rack
        ###self.usb_ports = ['1.4.4','1.4.3', '1.4.2', '1.4.1', '1'] #### temp tests
        self.usb_ports = ['1.4.4','1.4.3', '1.4.2', '1.4.1', '1.2']
        self.regrule = re.compile('\usb-0:(.*?)\:')

        self.zmq_port = 5555
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REP)
        self.socket.bind("tcp://*:%s" % self.zmq_port)

        self.zmq_port_local = 5554
        self.context_local = zmq.Context()
        self.socket_local = self.context_local.socket(zmq.PAIR)
        self.socket_local.bind("tcp://127.0.0.1:%s" % self.zmq_port_local)
        
        self.ser = serial.Serial()
        self.ser.baudrate = 3000000
        self.ser.timeout = 0.1
        
        self.error_queue = Queue.Queue()
        
        self.threads = []
        for i in range(self.number_of_ports): 
            self.threads.append(ModuleThread(i, self.usb_ports[i], self.error_queue))
            self.threads[i].start()

        self.parser = parser(self.threads, self.error_queue)
        
        self.tread_scan_ports = threading.Thread(target=self.scan_ports)
        self.tread_scan_ports.start()
        
        self.tread_main_server = threading.Thread(target=self.main_server)
        self.tread_main_server.start()

        logging.info("Starting Easy-phi server")
        
        
    def main_server(self):
        while 1: 
            msg = self.socket.recv()
            if (msg == ""):
                self.error_queue.put("Received empty message")
            else:    
                answer = self.parser.parse_message(msg)
                if (answer != ""):
                    self.socket.send(answer)
   
  
    def scan_ports(self):         
        while 1: 
            list_path_storages = commands.getoutput('ls /dev/disk/by-path/')
            array_path_storages = list_path_storages.split()
            
            list_path_serial = commands.getoutput('ls /dev/serial/by-path/')
            array_path_serial = list_path_serial.split()
            mnt_point = "/var/www/sd_cards"
            
            for i in range(self.number_of_ports):
                serial_present = 0
                for path_storage in array_path_storages:
                    regresult = self.regrule.findall(path_storage)
                    for port in regresult:
                        if port == self.threads[i].usb_port :
                            link = commands.getoutput('readlink -e /dev/disk/by-path/' + path_storage)            
                            mount = commands.getoutput('df ' + link)
                            array_mount = mount.split()
                            mount_point = array_mount[len(array_mount)-1]
                            if not(os.path.ismount(mnt_point + '/slot_' + str(i + 1))):  ### not yet mounted
                                mount_result = commands.getoutput('mount ' + link + ' ' + mnt_point + '/slot_' + str(i + 1) + ' -o rw,uid=0,gid=0,umask=000')
                                logging.info("mount result: " + mount_result)
                                mount_point = (mnt_point + '/slot_' + str(i + 1))
                            if (os.path.ismount(mnt_point + '/slot_' + str(i + 1))): 
                                for path_serial in array_path_serial:
                                    regresult = self.regrule.findall(path_serial)
                                    for port in regresult:
                                        if port == self.threads[i].usb_port : 
                                            serial_dev = commands.getoutput('readlink -e /dev/serial/by-path/' + path_serial)
                                            serial_present = 1
                                            if self.threads[i].board_present == 0:
                                                logging.info("found on port %d (%s): %s %s (%s)" % ((i+1), self.threads[i].usb_port, mount_point, serial_dev, link))
                                                self.connect_port(i, serial_dev)
                                                    
                                            
                if  (serial_present == 0) and (os.path.ismount(mnt_point + '/slot_' + str(i + 1))): 
                    mount_result = commands.getoutput('umount ' + mnt_point + '/slot_' + str(i + 1))
                    self.threads[i].disconnect()
                    logging.info("umount result: " + mount_result)
                            
            time.sleep(1)
                                            
    def request_serial(self, command):         
        self.ser.write(command) 
        self.ser.write(chr(13))
        loop_cnt = 0
        text = ''
        char = ''
        while (char != chr(13)):
            char = self.ser.read(1)
            if (char != chr(10)):  text = text + char
            loop_cnt = loop_cnt + 1
            if (loop_cnt == 1000):
                self.error_queue.put("Timout on read while getting module parameters: %s" % command)
                break
        return text

    def connect_port(self, number, serial_dev):         
        try:
            self.ser.port = serial_dev
            self.ser.open()

            
            self.threads[number].version       = self.request_serial("SYSTem:VERSion?")
            self.threads[number].type          = self.request_serial("SYSTem:TYPE?")
            self.threads[number].name          = self.request_serial("SYSTem:NAME?")
            self.threads[number].description   = self.request_serial("SYSTem:DESC?")
            self.threads[number].serial_number = self.request_serial("SYSTem:SN?")
            
            logging.info("Module version         is: %s" % self.threads[number].version      )    
            logging.info("Module type            is: %s" % self.threads[number].type         )    
            logging.info("Module name            is: %s" % self.threads[number].name         )    
            logging.info("Module description     is: %s" % self.threads[number].description  )    
            logging.info("Module serial_number   is: %s" % self.threads[number].serial_number)    

            self.ser.close()
        
            self.threads[number].ser.port = serial_dev
            self.threads[number].board_present = 1
            self.threads[number].connect()
        except (serial.SerialException):
            self.error_queue.put("Connection Error on slot: d on device %s" % (number, serial_dev))
        

Server = main_server()
for t in Server.threads: t.join()
Server.tread_scan_ports.join()
Server.tread_main_server.join()
    
logging.info("Exiting Main Thread")