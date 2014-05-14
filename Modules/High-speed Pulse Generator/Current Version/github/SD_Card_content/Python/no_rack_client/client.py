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


### CONFigure:DELay:A:ENable
### CONFigure:DELay:B:ENable
### CONFigure:DELay:A:VALue 
### CONFigure:DELay:B:VALue 


class StartQT4(QtGui.QMainWindow):
    def __init__(self, parent=None):
        QtGui.QWidget.__init__(self, parent)
        self.initUI()
        self.value_a = array([0 for i in range(100)], dtype=float)
        self.value_b = array([0 for i in range(100)], dtype=float)


        self.logactive = 0
        self.resetactive = 0
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect("tcp://%s:%s" % (self.address,self.port))
        QtGui.QWidget.__init__(self, parent)
        self.ui = Ui_Form()
        self.ui.setupUi(self)

        self.initialize()
        
        #### timer = QtCore.QTimer(self)       
        #### QtCore.QObject.connect(timer, QtCore.SIGNAL("timeout()"), self.on_timer)
        #### timer.start(100) #100 --> 0.1 second
        #### #timer.start(500) #500 --> 0.5 second
        #### #timer.start(1000) #1000 --> 1 second

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
        answer = self.request_serial("CONFigure:DELay:A:ENable?") 
        if (int(answer) == 1): self.ui.delaen.setChecked(True)
        
        answer = self.request_serial("CONFigure:DELay:B:ENable?") 
        if (int(answer) == 1): self.ui.delben.setChecked(True)
        
        answer = self.request_serial("CONFigure:DELay:A:VALue?") 
        self.ui.delaint.setValue(int(answer))
        
        answer = self.request_serial("CONFigure:DELay:B:VALue?") 
        self.ui.delbint.setValue(int(answer))
        
    def on_timer(self): pass
    
    def on_delaen_toggled(self): 
        if self.ui.delaen.isChecked() : 
            answer = self.request_serial("CONFigure:DELay:A:ENable 1") 
        else : 
            answer = self.request_serial("CONFigure:DELay:A:ENable 0")  
        
    def on_delben_toggled(self): 
        if self.ui.delben.isChecked() : 
            answer = self.request_serial("CONFigure:DELay:B:ENable 1")
        else : 
            answer = self.request_serial("CONFigure:DELay:B:ENable 0")

    def on_delaint_valueChanged(self):
        answer = self.request_serial("CONFigure:DELay:A:VALue %d" % (self.ui.delaint.value()))
    
    def on_delbint_valueChanged(self):
        answer = self.request_serial("CONFigure:DELay:B:VALue %d" % (self.ui.delbint.value()))

            
                    
app = QtGui.QApplication(sys.argv)
myapp = StartQT4()
myapp.show()

sys.exit(app.exec_())
