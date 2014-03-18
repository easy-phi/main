import time, sys
from PyQt4 import QtCore, QtGui
import zmq
import serial 
import struct
import re
from pylab import *


### CONFigure:DELay:A:ENable
### CONFigure:DELay:B:ENable
### CONFigure:DELay:A:VALue 
### CONFigure:DELay:B:VALue 


class slot(QtGui.QMainWindow):
    def __init__(self, ui, address, port, tab):
        self.tab = tab
        self.tab_active = False
        self.empty = False       
        self.value_a = array([0 for i in range(100)], dtype=float)
        self.value_b = array([0 for i in range(100)], dtype=float)

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
        if not(re.search("Error", text)): 
            return text
        else:    
            return "0"
        
    def initialize(self):
        answer = self.request_serial("CONFigure:DELay:A:ENable?") 
        if (int(answer) == 1): self.delaen.setChecked(True)
        
        answer = self.request_serial("CONFigure:DELay:B:ENable?") 
        if (int(answer) == 1): self.delben.setChecked(True)
        
        answer = self.request_serial("CONFigure:DELay:A:VALue?") 
        self.delaint.setValue(int(answer))
        
        answer = self.request_serial("CONFigure:DELay:B:VALue?") 
        self.delbint.setValue(int(answer))
        
    def on_timer(self): pass
    
    def on_delaen_toggled(self): 
        if self.delaen.isChecked() : 
            answer = self.request_serial("CONFigure:DELay:A:ENable 1") 
        else : 
            answer = self.request_serial("CONFigure:DELay:A:ENable 0")  
        
    def on_delben_toggled(self): 
        if self.delben.isChecked() : 
            answer = self.request_serial("CONFigure:DELay:B:ENable 1")
        else : 
            answer = self.request_serial("CONFigure:DELay:B:ENable 0")

    def on_delaint_valueChanged(self):
        answer = self.request_serial("CONFigure:DELay:A:VALue %d" % (self.delaint.value()))
    
    def on_delbint_valueChanged(self):
        answer = self.request_serial("CONFigure:DELay:B:VALue %d" % (self.delbint.value()))

            
    def graphical_intf(self):       
        self.delaen = QtGui.QRadioButton(self.tab)
        self.delaen.setGeometry(QtCore.QRect(50, 50, 200, 27))
        self.delaen.setAutoExclusive(False)
        self.delaen.setText("Enable Delay A")
        self.delaen.setObjectName("delaen")
        self.delaen.toggled.connect(self.on_delaen_toggled) 

        self.delben = QtGui.QRadioButton(self.tab)
        self.delben.setGeometry(QtCore.QRect(50, 80, 200, 27))
        self.delben.setAutoExclusive(False)
        self.delben.setText("Enable Delay B")
        self.delben.setObjectName("delben")
        self.delben.toggled.connect(self.on_delben_toggled) 

        self.delaint = QtGui.QSpinBox(self.tab)
        self.delaint.setGeometry(QtCore.QRect(50, 110, 60, 27))
        self.delaint.setMinimum(0);
        self.delaint.setMaximum(5115);
        self.delaint.setValue(0);
        self.delaint.setObjectName("delaint")
        self.delaint.valueChanged.connect(self.on_delaint_valueChanged)
        self.delaintl = QtGui.QLabel(self.tab)
        self.delaintl.setGeometry(QtCore.QRect(150, 110, 250, 27))
        self.delaintl.setText("Delay A value (ps)")       

        self.delbint = QtGui.QSpinBox(self.tab)
        self.delbint.setGeometry(QtCore.QRect(50, 140, 60, 27))
        self.delbint.setMinimum(0);
        self.delbint.setMaximum(5115);
        self.delbint.setValue(0);
        self.delbint.setObjectName("delbint")
        self.delbint.valueChanged.connect(self.on_delbint_valueChanged)
        self.delbintl = QtGui.QLabel(self.tab)
        self.delbintl.setGeometry(QtCore.QRect(150, 140, 250, 27))
        self.delbintl.setText("Delay B value (ps)")       

        self.line2 = QtGui.QFrame(self.tab)
        self.line2.setGeometry(QtCore.QRect(50, 170, 250, 10))
        self.line2.setFrameShape(QtGui.QFrame.HLine)
        self.line2.setFrameShadow(QtGui.QFrame.Sunken)

