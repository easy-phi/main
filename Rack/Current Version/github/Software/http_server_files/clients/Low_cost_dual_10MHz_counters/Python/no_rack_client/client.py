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

##### CONFigure:COUnter:A:ENable  boolean   (Enable counter A)
##### CONFigure:COUnter:B:ENable  boolean   (Enable counter B)
##### CONFigure:COUnter:A:Read?  (Read counter A)
##### CONFigure:COUnter:B:Read?  (Read counter A)
##### CONFigure:COUnter:A:INTegrationtime   integer (Set integration time of counter A in ms)
##### CONFigure:COUnter:B:INTegrationtime   integer (Set integration time of counter B in ms)
##### CONFigure:TTLinputenable     booleen                   (Set QMA out to the TTL input)
##### CONFigure:FREQuence         integer    (Set frequency on QMA out)
##### CONFigure:DUTYcycle    integer    (Set duty cycle on QMA out)


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
        answer = self.request_serial("CONFigure:COUnter:A:ENable?") 
        if (int(answer) == 1): self.ui.couaen.setChecked(True)

        answer = self.request_serial("CONFigure:COUnter:B:ENable?") 
        if (int(answer) == 1): self.ui.couben.setChecked(True)

        answer = self.request_serial("CONFigure:COUnter:A:INTegrationtime?") 
        self.ui.couaint.setValue(int(answer))

        answer = self.request_serial("CONFigure:COUnter:B:INTegrationtime?") 
        self.ui.coubint.setValue(int(answer))

        answer = self.request_serial("CONFigure:TTLinputenable?") 
        if (int(answer) == 1): self.ui.ttl.setChecked(True)

        answer = self.request_serial("CONFigure:FREQuence?") 
        self.ui.freq.setValue((int(answer)/1000))

        
        
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
            
    def on_couaen_toggled(self): 
        if self.ui.couaen.isChecked() : 
            answer = self.request_serial("CONFigure:COUnter:A:ENable 1") 
        else : 
            answer = self.request_serial("CONFigure:COUnter:A:ENable 0")  
        
    def on_couben_toggled(self): 
        if self.ui.couben.isChecked() : 
            answer = self.request_serial("CONFigure:COUnter:B:ENable 1")
        else : 
            answer = self.request_serial("CONFigure:COUnter:B:ENable 0")

    def on_couaint_valueChanged(self):
        answer = self.request_serial("CONFigure:COUnter:A:INTegrationtime %d" % (self.ui.couaint.value()))
    
    def on_coubint_valueChanged(self):
        answer = self.request_serial("CONFigure:COUnter:B:INTegrationtime %d" % (self.ui.coubint.value()))

            
    def on_ttl_toggled(self): 
        if self.ui.ttl.isChecked() : 
            answer = self.request_serial("CONFigure:TTLinputenable 1")
        else : 
            answer = self.request_serial("CONFigure:TTLinputenable 0")
   

    def on_freq_valueChanged(self):
        answer = self.request_serial("CONFigure:FREQuence %d" % (self.ui.freq.value() * 1000))
    
                    
app = QtGui.QApplication(sys.argv)
myapp = StartQT4()
myapp.show()

sys.exit(app.exec_())
