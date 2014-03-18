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

##### CONFigure:COUnter:A:ENable  boolean   (Enable counter A)
##### CONFigure:COUnter:B:ENable  boolean   (Enable counter B)
##### CONFigure:COUnter:A:Read?  (Read counter A)
##### CONFigure:COUnter:B:Read?  (Read counter A)
##### CONFigure:COUnter:A:INTegrationtime   integer (Set integration time of counter A in ms)
##### CONFigure:COUnter:B:INTegrationtime   integer (Set integration time of counter B in ms)
##### CONFigure:TTLinputenable     booleen                   (Set QMA out to the TTL input)
##### CONFigure:FREQuence         integer    (Set frequency on QMA out)
##### CONFigure:DUTYcycle    integer    (Set duty cycle on QMA out)

class slot(QtGui.QMainWindow):
    def __init__(self, ui, address, port, tab):
        self.tab = tab
        self.tab_active = False
        self.empty = False
        self.value_a = array([0 for i in range(100)], dtype=float)
        self.value_b = array([0 for i in range(100)], dtype=float)

        self.freq_a = array([0 for i in range(4)], dtype=float)
        self.freq_b = array([0 for i in range(4)], dtype=float)

        self.cnta = 0
        self.cntb = 0
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
        answer = self.request_serial("CONFigure:COUnter:A:ENable?") 
        if (int(answer) == 1): self.couaen.setChecked(True)

        answer = self.request_serial("CONFigure:COUnter:B:ENable?") 
        if (int(answer) == 1): self.couben.setChecked(True)

        ####answer = self.request_serial("CONFigure:COUnter:A:INTegrationtime?") 
        ####self.couaint.setValue(int(answer))
        ####
        ####answer = self.request_serial("CONFigure:COUnter:B:INTegrationtime?") 
        ####self.coubint.setValue(int(answer))

        answer = self.request_serial("CONFigure:TTLinputenable?") 
        if (int(answer) == 1): self.ttl.setChecked(True)

        answer = self.request_serial("CONFigure:FREQuence?") 
        self.freq.setValue((int(answer)/1000))

        
        
    def on_timer(self):
        answer = ' '
        while not(re.search("No new val", answer)): 
            answer = self.request_serial("CONFigure:COUnter:A:Read?") 
            if not(re.search("No new val", answer)): 
                if (re.search("Error", answer, re.IGNORECASE)): break
                self.value_a = roll(self.value_a, 1)
                self.value_a[0] = int(answer)
                self.cnta = self.cnta + int(answer)
                self.freq_a = roll(self.freq_a, 1)
                self.freq_a[0] = int(answer)
                if self.tab_active:   ##only update gui if tab if active           
                    self.dca.update_figure(100, self.value_a)
                    self.cntal.setText("%d" % self.cnta)       
                    self.freqal.setText("Frequency Counter A = %d" % (self.freq_a[0] + self.freq_a[1] + self.freq_a[2] + self.freq_a[3]))      
        
        answer = ' '
        while not(re.search("No new val", answer)): 
            answer = self.request_serial("CONFigure:COUnter:B:Read?") 
            if not(re.search("No new val", answer)): 
                if (re.search("Error", answer, re.IGNORECASE)): break
                self.value_b = roll(self.value_b, 1)
                self.value_b[0] = int(answer)
                self.cntb = self.cntb + int(answer)
                self.freq_b = roll(self.freq_b, 1)
                self.freq_b[0] = int(answer)
                if self.tab_active:   ##only update gui if tab if active           
                    self.dcb.update_figure(100, self.value_b)
                    self.cntbl.setText("%d" % self.cntb)       
                    self.freqbl.setText("Frequency Counter B = %d" % (self.freq_b[0] + self.freq_b[1] + self.freq_b[2] + self.freq_b[3]))       

    def on_cntareset_clicked(self): 
        self.cnta = 0
        self.cntal.setText("0")       
        
    def on_cntbreset_clicked(self): 
        self.cntb = 0
        self.cntbl.setText("0")                  
            
    def on_couaen_toggled(self): 
        if self.couaen.isChecked() : 
            answer = self.request_serial("CONFigure:COUnter:A:ENable 1") 
        else : 
            answer = self.request_serial("CONFigure:COUnter:A:ENable 0")  
        
    def on_couben_toggled(self): 
        if self.couben.isChecked() : 
            answer = self.request_serial("CONFigure:COUnter:B:ENable 1")
        else : 
            answer = self.request_serial("CONFigure:COUnter:B:ENable 0")

    ##def on_couaint_valueChanged(self):
    ##    answer = self.request_serial("CONFigure:COUnter:A:INTegrationtime %d" % (self.couaint.value()))
    ##
    ##def on_coubint_valueChanged(self):
    ##    answer = self.request_serial("CONFigure:COUnter:B:INTegrationtime %d" % (self.coubint.value()))

            
    def on_ttl_toggled(self): 
        if self.ttl.isChecked() : 
            answer = self.request_serial("CONFigure:TTLinputenable 1")
        else : 
            answer = self.request_serial("CONFigure:TTLinputenable 0")
   

    def on_freq_valueChanged(self):
        answer = self.request_serial("CONFigure:FREQuence %d" % (self.freq.value() * 1000))


    def graphical_intf(self):       
        self.couaen = QtGui.QRadioButton(self.tab)
        self.couaen.setGeometry(QtCore.QRect(50, 50, 200, 27))
        self.couaen.setAutoExclusive(False)
        self.couaen.setText("Enable Counter A")
        self.couaen.setObjectName("couaen")
        self.couaen.toggled.connect(self.on_couaen_toggled)
        
        self.couben = QtGui.QRadioButton(self.tab)
        self.couben.setGeometry(QtCore.QRect(50, 80, 200, 27))
        self.couben.setAutoExclusive(False)
        self.couben.setText("Enable Counter B")
        self.couben.setObjectName("couben")
        self.couben.toggled.connect(self.on_couben_toggled)
        
        ####self.couaint = QtGui.QSpinBox(self.tab)
        ####self.couaint.setGeometry(QtCore.QRect(50, 110, 60, 27))
        ####self.couaint.setMinimum(1);
        ####self.couaint.setMaximum(240);
        ####self.couaint.setValue(1);
        ####self.couaint.setObjectName("couaint")
        ####self.couaint.valueChanged.connect(self.on_couaint_valueChanged)
        ####self.couaintl = QtGui.QLabel(self.tab)
        ####self.couaintl.setGeometry(QtCore.QRect(150, 110, 250, 27))
        ####self.couaintl.setText("Counter A integration time")       
        ####
        ####self.coubint = QtGui.QSpinBox(self.tab)
        ####self.coubint.setGeometry(QtCore.QRect(50, 140, 60, 27))
        ####self.coubint.setMinimum(1);
        ####self.coubint.setMaximum(240);
        ####self.coubint.setValue(1);
        ####self.coubint.setObjectName("coubint")
        ####self.coubint.valueChanged.connect(self.on_coubint_valueChanged)
        ####self.coubintl = QtGui.QLabel(self.tab)
        ####self.coubintl.setGeometry(QtCore.QRect(150, 140, 250, 27))
        ####self.coubintl.setText("Counter B integration time")       

        self.line1 = QtGui.QFrame(self.tab)
        self.line1.setGeometry(QtCore.QRect(50, 170, 250, 10))
        self.line1.setFrameShape(QtGui.QFrame.HLine)
        self.line1.setFrameShadow(QtGui.QFrame.Sunken)



        
        self.ttl = QtGui.QRadioButton(self.tab)
        self.ttl.setGeometry(QtCore.QRect(50, 180, 200, 27))
        self.ttl.setAutoExclusive(False)
        self.ttl.setText("Set QMA out to the TTL input")
        self.ttl.setObjectName("ttl")
        self.ttl.toggled.connect(self.on_ttl_toggled)
        
        self.freq = QtGui.QSpinBox(self.tab)
        self.freq.setGeometry(QtCore.QRect(50, 210, 60, 27))
        self.freq.setMinimum(100);
        self.freq.setMaximum(50000);
        self.freq.setValue(500);
        self.freq.setObjectName("freq")      
        self.freq.valueChanged.connect(self.on_freq_valueChanged)       
        self.freql = QtGui.QLabel(self.tab)
        self.freql.setGeometry(QtCore.QRect(150, 210, 250, 27))
        self.freql.setText("Frequence of test output kHZ")       


        self.line2 = QtGui.QFrame(self.tab)
        self.line2.setGeometry(QtCore.QRect(50, 290, 250, 10))
        self.line2.setFrameShape(QtGui.QFrame.HLine)
        self.line2.setFrameShadow(QtGui.QFrame.Sunken)

        self.cntareset = QtGui.QPushButton(self.tab)
        self.cntareset.setGeometry(QtCore.QRect(50, 300, 80, 27))
        self.cntareset.setAutoExclusive(False)
        self.cntareset.setText("Reset Count")
        self.cntareset.setObjectName("cntareset")
        self.cntareset.clicked.connect(self.on_cntareset_clicked) 
        self.cntal = QtGui.QLabel(self.tab)
        self.cntal.setGeometry(QtCore.QRect(150, 300, 250, 27))
        self.cntal.setText("0")       
       
        self.cntbreset = QtGui.QPushButton(self.tab)
        self.cntbreset.setGeometry(QtCore.QRect(50, 330, 80, 27))
        self.cntbreset.setAutoExclusive(False)
        self.cntbreset.setText("Reset Count")
        self.cntbreset.setObjectName("cntbreset")
        self.cntbreset.clicked.connect(self.on_cntbreset_clicked)
        self.cntbl = QtGui.QLabel(self.tab)
        self.cntbl.setGeometry(QtCore.QRect(150, 330, 250, 27))
        self.cntbl.setText("0")       

        self.freqal = QtGui.QLabel(self.tab)
        self.freqal.setGeometry(QtCore.QRect(50, 360, 250, 27))
        self.freqal.setText("Frequency Counter A = 0")       
       
        self.freqbl = QtGui.QLabel(self.tab)
        self.freqbl.setGeometry(QtCore.QRect(50, 390, 250, 27))
        self.freqbl.setText("Frequency Counter B = 0")       

        self.framea = QtGui.QFrame(self.tab)
        self.framea.setGeometry(QtCore.QRect(300, 0, 610, 350))
        self.framea.setFrameShape(QtGui.QFrame.StyledPanel)
        self.framea.setFrameShadow(QtGui.QFrame.Raised)
        self.framea.setObjectName("framea")
        self.frameb = QtGui.QFrame(self.tab)
        self.frameb.setGeometry(QtCore.QRect(300, 330, 610, 350))
        self.frameb.setFrameShape(QtGui.QFrame.StyledPanel)
        self.frameb.setFrameShadow(QtGui.QFrame.Raised)
        self.frameb.setObjectName("frameb")

        self.local_widgeta = QtGui.QWidget(self.framea)
        self.la = QtGui.QVBoxLayout(self.local_widgeta)
        self.dca = MyMplCanvas(self.local_widgeta, width=8, height=4, dpi=80)
        self.la.addWidget(self.dca)
        self.local_widgetb = QtGui.QWidget(self.frameb)
        self.lb = QtGui.QVBoxLayout(self.local_widgetb)
        self.dcb = MyMplCanvas(self.local_widgetb, width=8, height=4, dpi=80)
        self.lb.addWidget(self.dcb)

        
        