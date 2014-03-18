#######-------------------------------------------------------------------------------------
#######Easy-phi project: http://easy-phi.ch/
#######Developer : Raphael Houlmann, University of Geneva 2014
#######Licensed under GPL, for details report to: http://easy-phi.ch/index.php/about/licensing
#######Do NOT remove this notice
#######-------------------------------------------------------------------------------------
import time, sys
from PyQt4 import QtCore, QtGui
from gui import *
import zmq
import serial 
import struct
import re
from pylab import *


#{.pattern = "CONFigure:COUnter:A:Read?", .callback = SCPI_CounterA_Read,},\
#{.pattern = "CONFigure:COUnter:B:Read?", .callback = SCPI_CounterB_Read,},\
#{.pattern = "CONFigure:COUnter:A:INTegrationtime?", .callback = SCPI_GetCounterA_IT,},\
#{.pattern = "CONFigure:COUnter:A:INTegrationtime", .callback = SCPI_CounterA_IT,},\
#{.pattern = "CONFigure:COUnter:B:INTegrationtime?", .callback = SCPI_GetCounterB_IT,},\
#{.pattern = "CONFigure:COUnter:B:INTegrationtime", .callback = SCPI_CounterB_IT,},\


class StartQT4(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)
        self.initUI()
        self.value_a = array([0 for i in range(100)], dtype=float)
        self.value_b = array([0 for i in range(100)], dtype=float)

        self.freq_a = array([0 for i in range(4)], dtype=float)
        self.freq_b = array([0 for i in range(4)], dtype=float)

        self.cnta = 0
        self.cntb = 0
        self.logactive = 0
        self.resetactive = 0
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect("tcp://%s:%s" % (self.address,self.port))
        QtGui.QWidget.__init__(self, parent)
        self.ui = Ui_Form()
        self.ui.setupUi(self)

        self.initialize()
        
        timer = QtCore.QTimer(self)       
        QtCore.QObject.connect(timer, QtCore.SIGNAL("timeout()"), self.on_timer)
        timer.start(100) #100 --> 0.1 second
        #timer.start(500) #500 --> 0.5 second
        #timer.start(1000) #1000 --> 1 second
        
    def initUI(self):      
        text, ok = QtGui.QInputDialog.getText(self, 'Input Dialog', 'Address (IP) of easy-phi rack:')
        if ok: self.address = text       
        text, ok = QtGui.QInputDialog.getText(self, 'Input Dialog', 'Slot number (1 - 12) :')
        if ok: self.slot = int(text)
        self.port = str(5555 + self.slot)
        
        
    def request_serial(self, message):
        self.socket.send(message) 
        text = self.socket.recv()        
        if not(re.search("Error", text)): 
            return text
        else:    
            return "0"

        
    def initialize(self):
        answer = self.request_serial("CONFigure:CHAnnel:A:OFFset:ENable?")
        if (int(answer) == 1): self.ui.offaen.setChecked(True)
        
        answer = self.request_serial("CONFigure:CHAnnel:B:OFFset:ENable?") 
        if (int(answer) == 1): self.ui.offben.setChecked(True)
        
        answer = self.request_serial("CONFigure:CHAnnel:A:OFFset:VALue?") 
        self.ui.offaval.setValue(int(answer))
        
        answer = self.request_serial("CONFigure:CHAnnel:B:OFFset:VALue?") 
        self.ui.offbval.setValue(int(answer))
        
        answer = self.request_serial("CONFigure:CHAnnel:A:THReshold?") 
        self.ui.thrsaval.setValue(int(answer))
        
        answer = self.request_serial("CONFigure:CHAnnel:B:THReshold?") 
        self.ui.thrsbval.setValue(int(answer))
        
        answer = self.request_serial("CONFigure:CHAnnel:A:COUpling?") 
        if (answer[0:2] == "DC"): self.ui.couplinga.setCurrentIndex(1)
        
        answer = self.request_serial("CONFigure:CHAnnel:B:COUpling?") 
        if (answer[0:2] == "DC"): self.ui.couplingb.setCurrentIndex(1)
        
    def on_timer(self):
        answer = self.request_serial("CONFigure:COUnter:A:Read?") 
        if not(re.search("No new val", answer)): 
            self.value_a = roll(self.value_a, 1)
            self.value_a[0] = int(answer)
            self.ui.dca.update_figure(100, self.value_a)
            self.cnta = self.cnta + int(answer)
            self.ui.cntal.setText("%d" % self.cnta)       
            self.freq_a = roll(self.freq_a, 1)
            self.freq_a[0] = int(answer)
            self.ui.freqal.setText("Frequency Counter A = %d" % (self.freq_a[0] + self.freq_a[1] + self.freq_a[2] + self.freq_a[3]))      
        
        
        answer = self.request_serial("CONFigure:COUnter:B:Read?") 
        if not(re.search("No new val", answer)): 
            self.value_b = roll(self.value_b, 1)
            self.value_b[0] = int(answer)
            self.ui.dcb.update_figure(100, self.value_b)
            self.cntb = self.cntb + int(answer)
            self.ui.cntbl.setText("%d" % self.cntb)       
            self.freq_b = roll(self.freq_b, 1)
            self.freq_b[0] = int(answer)
            self.ui.freqbl.setText("Frequency Counter B = %d" % (self.freq_b[0] + self.freq_b[1] + self.freq_b[2] + self.freq_b[3]))       
            

    def on_cntareset_clicked(self): 
        self.cnta = 0
        self.ui.cntal.setText("0")       
        
    def on_cntbreset_clicked(self): 
        self.cntb = 0
        self.ui.cntbl.setText("0")                  
            
    def on_couplinga_activated(self): 
        text = ("CONFigure:CHAnnel:A:COUpling %s" % str(self.ui.couplinga.currentText())) 
        answer = self.request_serial(text) 

    def on_couplingb_activated(self): 
        text = ("CONFigure:CHAnnel:B:COUpling %s" % str(self.ui.couplingb.currentText())) 
        answer = self.request_serial(text) 

    
    def on_offaen_toggled(self): 
        if self.ui.offaen.isChecked() : 
            answer = self.request_serial("CONFigure:CHAnnel:A:OFFset:ENable 1") 
        else : 
            answer = self.request_serial("CONFigure:CHAnnel:A:OFFset:ENable 0")  
        
    def on_offben_toggled(self): 
        if self.ui.offben.isChecked() : 
            answer = self.request_serial("CONFigure:CHAnnel:B:OFFset:ENable 1")
        else : 
            answer = self.request_serial("CONFigure:CHAnnel:B:OFFset:ENable 0")

    def on_offaval_valueChanged(self):
        answer = self.request_serial("CONFigure:CHAnnel:A:OFFset:VALue %d" % (self.ui.offaval.value()))
    
    def on_offbval_valueChanged(self):
        answer = self.request_serial("CONFigure:CHAnnel:B:OFFset:VALue %d" % (self.ui.offbval.value()))
    
    def on_thrsaval_valueChanged(self):
        answer = self.request_serial("CONFigure:CHAnnel:A:THReshold %d" % (self.ui.thrsaval.value()))
    
    def on_thrsbval_valueChanged(self):
        answer = self.request_serial("CONFigure:CHAnnel:B:THReshold %d" % (self.ui.thrsbval.value()))

# {.pattern = "CONFigure:SPI:TestSend", .callback = SCPI_TestSPISend,},\
# {.pattern = "CONFigure:SPI:TestReceive", .callback = SCPI_TestSPIReceive,},\
    @QtCore.pyqtSignature("") ## to avoid function to be fired twice....
    def on_testwrite_clicked(self): 
            answer = self.request_serial("CONFigure:SPI:TestSend %d" % self.ui.testwriteval.value()) 
         
    @QtCore.pyqtSignature("") ## to avoid function to be fired twice....
    def on_testread_clicked(self): 
            answer = self.request_serial("CONFigure:SPI:TestReceive?")
            self.ui.testreadl.setText(answer)
    
                    
app = QtGui.QApplication(sys.argv)
myapp = StartQT4()
myapp.show()

sys.exit(app.exec_())
