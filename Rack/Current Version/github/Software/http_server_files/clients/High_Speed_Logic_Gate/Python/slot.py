#######-------------------------------------------------------------------------------------
#######Easy-phi project: http://easy-phi.ch/
#######Developer : Raphael Houlmann, University of Geneva 2014
#######Licensed under GPL, for details report to: http://easy-phi.ch/index.php/about/licensing
#######Do NOT remove this notice
#######-------------------------------------------------------------------------------------
import time, sys
from PyQt4 import QtCore, QtGui
import zmq
import serial 
import struct
import re
from pylab import *
from graphic import *


class slot(QtGui.QMainWindow):
    def __init__(self, ui, address, port, tab):
        self.tab = tab
        self.tab_active = False
        self.empty = False       
        self.logactive = 0
        self.resetactive = 0
        self.port = port
        self.address = address
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect("tcp://%s:%s" % (self.address, self.port))
        
    def request_serial(self, message):
        self.socket.send(message) 
        text = self.socket.recv()        
        return text

        
    def initialize(self):
        answer = self.request_serial("CONFigure:OUT1?") 
        if re.match("AND", answer): self.out1.setCurrentIndex(1)
        elif re.match("IN1", answer): self.out1.setCurrentIndex(2)
        elif re.match("IN2", answer): self.out1.setCurrentIndex(3)
        
        answer = self.request_serial("CONFigure:OUT2?") 
        if re.match("AND", answer): self.out2.setCurrentIndex(1)
        elif re.match("IN1", answer): self.out2.setCurrentIndex(2)
        elif re.match("IN2", answer): self.out2.setCurrentIndex(3)

        answer = self.request_serial("CONFigure:OUT3?") 
        if re.match("AND", answer): self.out3.setCurrentIndex(1)
        elif re.match("IN1", answer): self.out3.setCurrentIndex(2)
        elif re.match("IN2", answer): self.out3.setCurrentIndex(3)

        answer = self.request_serial("CONFigure:OUT4?") 
        if re.match("AND", answer): self.out4.setCurrentIndex(1)
        elif re.match("IN1", answer): self.out4.setCurrentIndex(2)
        elif re.match("IN2", answer): self.out4.setCurrentIndex(3)

        
    def on_timer(self): pass


    def out1change(self): 
        text = ("CONFigure:OUT1 %s" % str(self.out1.currentText())) 
        answer = self.request_serial(text) 

    def out2change(self): 
        text = ("CONFigure:OUT2 %s" % str(self.out2.currentText())) 
        answer = self.request_serial(text) 

    def out3change(self): 
        text = ("CONFigure:OUT3 %s" % str(self.out3.currentText())) 
        answer = self.request_serial(text) 

    def out4change(self): 
        text = ("CONFigure:OUT4 %s" % str(self.out4.currentText())) 
        answer = self.request_serial(text) 

    
    def graphical_intf(self):       
        self.out1l = QtGui.QLabel(self.tab)
        self.out1l.setGeometry(QtCore.QRect(50, 50, 250, 27))
        self.out1l.setText("Set output 1:")       
        self.out1 = QtGui.QComboBox(self.tab)
        self.out1.setGeometry(QtCore.QRect(160, 50, 50, 24))
        self.out1.insertItem(0,"OR")
        self.out1.insertItem(1,"AND")
        self.out1.insertItem(2,"IN1")
        self.out1.insertItem(3,"IN2")
        self.out1.activated.connect(self.out1change) 
        
        self.out2l = QtGui.QLabel(self.tab)
        self.out2l.setGeometry(QtCore.QRect(50, 80, 250, 27))
        self.out2l.setText("Set output 2:")       
        self.out2 = QtGui.QComboBox(self.tab)
        self.out2.setGeometry(QtCore.QRect(160, 80, 50, 24))
        self.out2.insertItem(0,"OR")
        self.out2.insertItem(1,"AND")
        self.out2.insertItem(2,"IN1")
        self.out2.insertItem(3,"IN2")
        self.out2.activated.connect(self.out2change) 
        
        self.out3l = QtGui.QLabel(self.tab)
        self.out3l.setGeometry(QtCore.QRect(50, 110, 250, 27))
        self.out3l.setText("Set output 3:")       
        self.out3 = QtGui.QComboBox(self.tab)
        self.out3.setGeometry(QtCore.QRect(160, 110, 50, 24))
        self.out3.insertItem(0,"OR")
        self.out3.insertItem(1,"AND")
        self.out3.insertItem(2,"IN1")
        self.out3.insertItem(3,"IN2")
        self.out3.activated.connect(self.out3change) 
        
        self.out4l = QtGui.QLabel(self.tab)
        self.out4l.setGeometry(QtCore.QRect(50, 140, 250, 27))
        self.out4l.setText("Set output 4:")       
        self.out4 = QtGui.QComboBox(self.tab)
        self.out4.setGeometry(QtCore.QRect(160, 140, 50, 24))
        self.out4.insertItem(0,"OR")
        self.out4.insertItem(1,"AND")
        self.out4.insertItem(2,"IN1")
        self.out4.insertItem(3,"IN2")
        self.out4.activated.connect(self.out4change) 


        
