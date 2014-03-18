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
        self.filename = ""
        QtGui.QWidget.__init__(self)
        
    def request_serial(self, message):
        self.socket.send(message) 
        text = self.socket.recv()        
        return text

        
    def initialize(self):
        answer = self.request_serial("CONFigure:OUT1?") 
        if re.match("PLL1", answer): self.out1.setCurrentIndex(1)
        elif re.match("PLL2", answer): self.out1.setCurrentIndex(2)
        elif re.match("MUX", answer): self.out1.setCurrentIndex(3)
        
        answer = self.request_serial("CONFigure:OUT2?") 
        if re.match("PLL1", answer): self.out2.setCurrentIndex(1)
        elif re.match("PLL2", answer): self.out2.setCurrentIndex(2)
        elif re.match("MUX", answer): self.out2.setCurrentIndex(3)

        answer = self.request_serial("CONFigure:OUT3?") 
        if re.match("PLL1", answer): self.out3.setCurrentIndex(1)
        elif re.match("PLL2", answer): self.out3.setCurrentIndex(2)
        elif re.match("MUX", answer): self.out3.setCurrentIndex(3)

        answer = self.request_serial("CONFigure:OUT4?") 
        if re.match("PLL1", answer): self.out4.setCurrentIndex(1)
        elif re.match("PLL2", answer): self.out4.setCurrentIndex(2)
        elif re.match("MUX", answer): self.out4.setCurrentIndex(3)

        answer = self.request_serial("CONFigure:MUX?") 
        if re.match("REF_CLK", answer): self.mux.setCurrentIndex(1)
        
        answer = self.request_serial("CONFigure:PLLin?") 
        if re.match("REF_CLK", answer): self.pll.setCurrentIndex(1)

        answer = self.request_serial("CONFigure:FREQuence?") 
        self.freq.setValue((int(answer)/1000))

        
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

    def muxchange(self): 
        text = ("CONFigure:MUX %s" % str(self.mux.currentText())) 
        answer = self.request_serial(text) 

    def pllchange(self): 
        text = ("CONFigure:PLLin %s" % str(self.pll.currentText())) 
        answer = self.request_serial(text) 
        
        
    def on_freq_valueChanged(self):
        answer = self.request_serial("CONFigure:FREQuence %d" % (self.freq.value() * 1000))

        
    @QtCore.pyqtSignature("") ## to avoid function to be fired twice....
    def on_file_clicked(self): 
        self.filename = QtGui.QFileDialog.getOpenFileName(self, 'Open file', '.', 'All files (*.*)')
        self.configfilename.setText(self.filename)        
    
    @QtCore.pyqtSignature("") ## to avoid function to be fired twice....
    def on_config_clicked(self): 
        if (self.filename != ""):
            
            line = 1
            f = open(self.filename, 'r')
            for line in f:        
                if re.search('#', line): 
                    pass
                else:
                    data_addr =  line.replace(',', '').replace('h', '').split()
                    address = int(data_addr[0], 10)
                    data = int(data_addr[1], 16)
                    answer = self.request_serial("CONFigure:CLKgen:SETRegister %d" % address) 
                    answer = self.request_serial("CONFigure:CLKgen:WRItedata %d" % data) 


 #{.pattern = "CONFigure:CLKgen:SETRegister", .callback = SCPI_TestSPISend,},\
 #{.pattern = "CONFigure:CLKgen:WRItedata", .callback = SCPI_TestSPIReceive,},\
 #{.pattern = "CONFigure:CLKgen:READdata?", .callback = SCPI_TestSPIReceive,},\
 

    @QtCore.pyqtSignature("") ## to avoid function to be fired twice....
    def on_SETRegister_clicked(self): 
        answer = self.request_serial("CONFigure:CLKgen:SETRegister %d" % self.SETRegisterval.value()) 

    @QtCore.pyqtSignature("") ## to avoid function to be fired twice....
    def on_WRItedata_clicked(self): 
        answer = self.request_serial("CONFigure:CLKgen:WRItedata %d" % self.WRItedataval.value()) 
            
    @QtCore.pyqtSignature("") ## to avoid function to be fired twice....
    def on_READdata_clicked(self): 
        answer = self.request_serial("CONFigure:CLKgen:READdata?")
        self.READdatal.setText(answer)
        
       
    
    def graphical_intf(self):       
        self.out1l = QtGui.QLabel(self.tab)
        self.out1l.setGeometry(QtCore.QRect(50, 50, 250, 27))
        self.out1l.setText("Set output 1:")       
        self.out1 = QtGui.QComboBox(self.tab)
        self.out1.setGeometry(QtCore.QRect(160, 50, 50, 24))
        self.out1.insertItem(0,"IN")
        self.out1.insertItem(1,"PLL1")
        self.out1.insertItem(2,"PLL2")
        self.out1.insertItem(3,"MUX")
        self.out1.activated.connect(self.out1change) 
        
        self.out2l = QtGui.QLabel(self.tab)
        self.out2l.setGeometry(QtCore.QRect(50, 80, 250, 27))
        self.out2l.setText("Set output 2:")       
        self.out2 = QtGui.QComboBox(self.tab)
        self.out2.setGeometry(QtCore.QRect(160, 80, 50, 24))
        self.out2.insertItem(0,"IN")
        self.out2.insertItem(1,"PLL1")
        self.out2.insertItem(2,"PLL2")
        self.out2.insertItem(3,"MUX")
        self.out2.activated.connect(self.out2change) 
        
        self.out3l = QtGui.QLabel(self.tab)
        self.out3l.setGeometry(QtCore.QRect(50, 110, 250, 27))
        self.out3l.setText("Set output 3:")       
        self.out3 = QtGui.QComboBox(self.tab)
        self.out3.setGeometry(QtCore.QRect(160, 110, 50, 24))
        self.out3.insertItem(0,"IN")
        self.out3.insertItem(1,"PLL1")
        self.out3.insertItem(2,"PLL2")
        self.out3.insertItem(3,"MUX")
        self.out3.activated.connect(self.out3change) 
        
        self.out4l = QtGui.QLabel(self.tab)
        self.out4l.setGeometry(QtCore.QRect(50, 140, 250, 27))
        self.out4l.setText("Set output 4:")       
        self.out4 = QtGui.QComboBox(self.tab)
        self.out4.setGeometry(QtCore.QRect(160, 140, 50, 24))
        self.out4.insertItem(0,"IN")
        self.out4.insertItem(1,"PLL1")
        self.out4.insertItem(2,"PLL2")
        self.out4.insertItem(3,"MUX")
        self.out4.activated.connect(self.out4change) 


        self.muxl = QtGui.QLabel(self.tab)
        self.muxl.setGeometry(QtCore.QRect(50, 200, 250, 27))
        self.muxl.setText("Configure MUX:")       
        self.mux = QtGui.QComboBox(self.tab)
        self.mux.setGeometry(QtCore.QRect(160, 200, 100, 24))
        self.mux.insertItem(0,"UC")
        self.mux.insertItem(1,"REF_CLK")
        self.mux.activated.connect(self.muxchange) 
        
        self.plll = QtGui.QLabel(self.tab)
        self.plll.setGeometry(QtCore.QRect(50, 230, 250, 27))
        self.plll.setText("Set PLL Input:")       
        self.pll = QtGui.QComboBox(self.tab)
        self.pll.setGeometry(QtCore.QRect(160, 230, 100, 24))
        self.pll.insertItem(0,"IN")
        self.pll.insertItem(1,"REF_CLK")
        self.pll.activated.connect(self.pllchange) 

        self.freq = QtGui.QSpinBox(self.tab)
        self.freq.setGeometry(QtCore.QRect(50, 290, 60, 27))
        self.freq.setMinimum(100);
        self.freq.setMaximum(50000);
        self.freq.setValue(500);
        self.freq.setObjectName("freq")      
        self.freq.valueChanged.connect(self.on_freq_valueChanged)       
        self.freql = QtGui.QLabel(self.tab)
        self.freql.setGeometry(QtCore.QRect(150, 290, 250, 27))
        self.freql.setText("Frequence of test output kHZ")       
        
        self.configfile = QtGui.QPushButton(self.tab)
        self.configfile.setGeometry(QtCore.QRect(50, 370, 85, 27))
        self.configfile.setObjectName("configfile")
        self.configfile.setText("Load config file")
        self.configfile.clicked.connect(self.on_file_clicked) 
        self.configfilename = QtGui.QLabel(self.tab)
        self.configfilename.setGeometry(QtCore.QRect(150, 370, 800, 27))
        self.configfilename.setObjectName("configfilename")
        self.configfilename.setText("None")
        
        self.loadconfig = QtGui.QPushButton(self.tab)
        self.loadconfig.setGeometry(QtCore.QRect(50, 400, 85, 27))
        self.loadconfig.setObjectName("loadconfig")
        self.loadconfig.setText("Load Config")
        self.loadconfig.clicked.connect(self.on_config_clicked) 
                
        self.SETRegister = QtGui.QPushButton(self.tab)
        self.SETRegister.setGeometry(QtCore.QRect(50, 450, 80, 27))
        self.SETRegister.setAutoExclusive(False)
        self.SETRegister.setText("Write Addr SPI")
        self.SETRegister.setObjectName("SETRegister")
        self.SETRegister.clicked.connect(self.on_SETRegister_clicked) 
        self.SETRegisterval = QtGui.QSpinBox(self.tab)
        self.SETRegisterval.setGeometry(QtCore.QRect(150, 450, 60, 27))
        self.SETRegisterval.setMinimum(0);
        self.SETRegisterval.setMaximum(65535);
        self.SETRegisterval.setValue(0);
        self.SETRegisterval.setObjectName("SETRegisterval")

        self.WRItedata = QtGui.QPushButton(self.tab)
        self.WRItedata.setGeometry(QtCore.QRect(50, 480, 80, 27))
        self.WRItedata.setAutoExclusive(False)
        self.WRItedata.setText("Write Data SPI")
        self.WRItedata.setObjectName("WRItedata")
        self.WRItedata.clicked.connect(self.on_WRItedata_clicked) 
        self.WRItedataval = QtGui.QSpinBox(self.tab)
        self.WRItedataval.setGeometry(QtCore.QRect(150, 480, 60, 27))
        self.WRItedataval.setMinimum(0);
        self.WRItedataval.setMaximum(65535);
        self.WRItedataval.setValue(0);
        self.WRItedataval.setObjectName("WRItedataval")
        
        
        self.READdata = QtGui.QPushButton(self.tab)
        self.READdata.setGeometry(QtCore.QRect(50, 510, 80, 27))
        self.READdata.setAutoExclusive(False)
        self.READdata.setText("Read SPI")
        self.READdata.setObjectName("READdata")
        self.READdata.clicked.connect(self.on_READdata_clicked) 
        self.READdatal = QtGui.QLabel(self.tab)
        self.READdatal.setGeometry(QtCore.QRect(150, 510, 100, 27))
        self.READdatal.setText("0")       
        
