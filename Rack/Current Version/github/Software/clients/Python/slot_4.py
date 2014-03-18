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


#{.pattern = "CONFigure:COUnter:A:Read?", .callback = SCPI_CounterA_Read,},\
#{.pattern = "CONFigure:COUnter:B:Read?", .callback = SCPI_CounterB_Read,},\
#{.pattern = "CONFigure:COUnter:A:INTegrationtime?", .callback = SCPI_GetCounterA_IT,},\
#{.pattern = "CONFigure:COUnter:A:INTegrationtime", .callback = SCPI_CounterA_IT,},\
#{.pattern = "CONFigure:COUnter:B:INTegrationtime?", .callback = SCPI_GetCounterB_IT,},\
#{.pattern = "CONFigure:COUnter:B:INTegrationtime", .callback = SCPI_CounterB_IT,},\


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
        answer = self.request_serial("CONFigure:CHAnnel:A:OFFset:ENable?")
        if (int(answer) == 1): self.offaen.setChecked(True)
        
        answer = self.request_serial("CONFigure:CHAnnel:B:OFFset:ENable?") 
        if (int(answer) == 1): self.offben.setChecked(True)
        
        answer = self.request_serial("CONFigure:CHAnnel:A:OFFset:VALue?") 
        self.offaval.setValue(int(answer))
        
        answer = self.request_serial("CONFigure:CHAnnel:B:OFFset:VALue?") 
        self.offbval.setValue(int(answer))
        
        answer = self.request_serial("CONFigure:CHAnnel:A:THReshold?") 
        self.thrsaval.setValue(int(answer))
        
        answer = self.request_serial("CONFigure:CHAnnel:B:THReshold?") 
        self.thrsbval.setValue(int(answer))
        
        answer = self.request_serial("CONFigure:CHAnnel:A:COUpling?") 
        if (answer[0:2] == "DC"): self.couplinga.setCurrentIndex(1)
        
        answer = self.request_serial("CONFigure:CHAnnel:B:COUpling?") 
        if (answer[0:2] == "DC"): self.couplingb.setCurrentIndex(1)
        
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
            
    def on_couplinga_activated(self): 
        text = ("CONFigure:CHAnnel:A:COUpling %s" % str(self.couplinga.currentText())) 
        answer = self.request_serial(text) 

    def on_couplingb_activated(self): 
        text = ("CONFigure:CHAnnel:B:COUpling %s" % str(self.couplingb.currentText())) 
        answer = self.request_serial(text) 

    
    def on_offaen_toggled(self): 
        if self.offaen.isChecked() : 
            answer = self.request_serial("CONFigure:CHAnnel:A:OFFset:ENable 1") 
        else : 
            answer = self.request_serial("CONFigure:CHAnnel:A:OFFset:ENable 0")  
        
    def on_offben_toggled(self): 
        if self.offben.isChecked() : 
            answer = self.request_serial("CONFigure:CHAnnel:B:OFFset:ENable 1")
        else : 
            answer = self.request_serial("CONFigure:CHAnnel:B:OFFset:ENable 0")

    def on_offaval_valueChanged(self):
        answer = self.request_serial("CONFigure:CHAnnel:A:OFFset:VALue %d" % (self.offaval.value()))
    
    def on_offbval_valueChanged(self):
        answer = self.request_serial("CONFigure:CHAnnel:B:OFFset:VALue %d" % (self.offbval.value()))
    
    def on_thrsaval_valueChanged(self):
        answer = self.request_serial("CONFigure:CHAnnel:A:THReshold %d" % (self.thrsaval.value()))
    
    def on_thrsbval_valueChanged(self):
        answer = self.request_serial("CONFigure:CHAnnel:B:THReshold %d" % (self.thrsbval.value()))

# {.pattern = "CONFigure:SPI:TestSend", .callback = SCPI_TestSPISend,},\
# {.pattern = "CONFigure:SPI:TestReceive", .callback = SCPI_TestSPIReceive,},\
    @QtCore.pyqtSignature("") ## to avoid function to be fired twice....
    def on_testwrite_clicked(self): 
            answer = self.request_serial("CONFigure:SPI:TestSend %d" % self.testwriteval.value()) 
         
    @QtCore.pyqtSignature("") ## to avoid function to be fired twice....
    def on_testread_clicked(self): 
            answer = self.request_serial("CONFigure:SPI:TestReceive?")
            self.testreadl.setText(answer)
    
    def graphical_intf(self):       
        self.couplingal = QtGui.QLabel(self.tab)
        self.couplingal.setGeometry(QtCore.QRect(50, 50, 250, 27))
        self.couplingal.setText("Coupling Channel A:")       
        self.couplinga = QtGui.QComboBox(self.tab)
        self.couplinga.setGeometry(QtCore.QRect(160, 50, 50, 24))
        self.couplinga.setObjectName("couplinga")
        self.couplinga.insertItem(0,"AC")
        self.couplinga.insertItem(1,"DC")
        self.couplinga.activated.connect(self.on_couplinga_activated) 

        self.couplingbl = QtGui.QLabel(self.tab)
        self.couplingbl.setGeometry(QtCore.QRect(50, 80, 250, 27))
        self.couplingbl.setText("Coupling Channel B:")       
        self.couplingb = QtGui.QComboBox(self.tab)
        self.couplingb.setGeometry(QtCore.QRect(160, 80, 50, 24))
        self.couplingb.setObjectName("couplingb")
        self.couplingb.insertItem(0,"AC")
        self.couplingb.insertItem(1,"DC")
        self.couplingb.activated.connect(self.on_couplingb_activated) 
        
        self.offaen = QtGui.QRadioButton(self.tab)
        self.offaen.setGeometry(QtCore.QRect(50, 110, 200, 27))
        self.offaen.setAutoExclusive(False)
        self.offaen.setText("Enable Offset on channel A")
        self.offaen.setObjectName("offaen")
        self.offaen.toggled.connect(self.on_offaen_toggled) 
        
        self.offben = QtGui.QRadioButton(self.tab)
        self.offben.setGeometry(QtCore.QRect(50, 140, 200, 27))
        self.offben.setAutoExclusive(False)
        self.offben.setText("Enable Offset on channel B")
        self.offben.setObjectName("offben")
        self.offben.toggled.connect(self.on_offben_toggled) 
        
        self.offaval = QtGui.QSpinBox(self.tab)
        self.offaval.setGeometry(QtCore.QRect(50, 170, 60, 27))
        self.offaval.setMinimum(-2500);
        self.offaval.setMaximum(2500);
        self.offaval.setValue(0);
        self.offaval.setObjectName("offaval")
        self.offaval.valueChanged.connect(self.on_offaval_valueChanged)
        self.offavall = QtGui.QLabel(self.tab)
        self.offavall.setGeometry(QtCore.QRect(120, 170, 250, 27))
        self.offavall.setText("[mV] Channel A Offset")       
        
        self.offbval = QtGui.QSpinBox(self.tab)
        self.offbval.setGeometry(QtCore.QRect(50, 200, 60, 27))
        self.offbval.setMinimum(-2500);
        self.offbval.setMaximum(2500);
        self.offbval.setValue(0);
        self.offbval.setObjectName("offbval")
        self.offbval.valueChanged.connect(self.on_offbval_valueChanged)
        self.offbvall = QtGui.QLabel(self.tab)
        self.offbvall.setGeometry(QtCore.QRect(120, 200, 250, 27))
        self.offbvall.setText("[mV] Channel B Offset")       



        self.thrsaval = QtGui.QSpinBox(self.tab)
        self.thrsaval.setGeometry(QtCore.QRect(50, 230, 60, 27))
        self.thrsaval.setMinimum(-5000);
        self.thrsaval.setMaximum(5000);
        self.thrsaval.setValue(0);
        self.thrsaval.setObjectName("thrsaval")
        self.thrsaval.valueChanged.connect(self.on_thrsaval_valueChanged)
        self.thrsavall = QtGui.QLabel(self.tab)
        self.thrsavall.setGeometry(QtCore.QRect(120, 230, 250, 27))
        self.thrsavall.setText("[mV] Channel A Threshold")       
        
        self.thrsbval = QtGui.QSpinBox(self.tab)
        self.thrsbval.setGeometry(QtCore.QRect(50, 260, 60, 27))
        self.thrsbval.setMinimum(-5000);
        self.thrsbval.setMaximum(5000);
        self.thrsbval.setValue(0);
        self.thrsbval.setObjectName("thrsbval")
        self.thrsbval.valueChanged.connect(self.on_thrsbval_valueChanged)
        self.thrsbvall = QtGui.QLabel(self.tab)
        self.thrsbvall.setGeometry(QtCore.QRect(120, 260, 250, 27))
        self.thrsbvall.setText("[mV] Channel B Threshold")       
        
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
        la = QtGui.QVBoxLayout(self.local_widgeta)
        self.dca = MyMplCanvas(self.local_widgeta, width=8, height=4, dpi=80)
        la.addWidget(self.dca)
        self.local_widgetb = QtGui.QWidget(self.frameb)
        lb = QtGui.QVBoxLayout(self.local_widgetb)
        self.dcb = MyMplCanvas(self.local_widgetb, width=8, height=4, dpi=80)
        lb.addWidget(self.dcb)

        
        self.testwrite = QtGui.QPushButton(self.tab)
        self.testwrite.setGeometry(QtCore.QRect(50, 450, 80, 27))
        self.testwrite.setAutoExclusive(False)
        self.testwrite.setText("Test Write SPI")
        self.testwrite.setObjectName("testwrite")
        self.testwrite.clicked.connect(self.on_testwrite_clicked) 
        self.testwriteval = QtGui.QSpinBox(self.tab)
        self.testwriteval.setGeometry(QtCore.QRect(150, 450, 60, 27))
        self.testwriteval.setMinimum(0);
        self.testwriteval.setMaximum(65535);
        self.testwriteval.setValue(0);
        self.testwriteval.setObjectName("testwriteval")
        
        self.testread = QtGui.QPushButton(self.tab)
        self.testread.setGeometry(QtCore.QRect(50, 480, 80, 27))
        self.testread.setAutoExclusive(False)
        self.testread.setText("Test Read SPI")
        self.testread.setObjectName("testread")
        self.testread.clicked.connect(self.on_testread_clicked) 
        self.testreadl = QtGui.QLabel(self.tab)
        self.testreadl.setGeometry(QtCore.QRect(150, 480, 100, 27))
        self.testreadl.setText("0")       


        
