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
import urllib
from pylab import *
from graphic import *

class StartQT4(QtGui.QMainWindow):
    def __init__(self, parent=None):
        self.fullScreen = False
        self.count = 0
        self.count_prev = 0
        self.alternate = 0
        self.frequence = 10000
        self.calibrated = False
        
        self.first_x_val = 340*5  ## 340 mm in ps
        self.last_x_val = 450*5   ## 430 mm in ps
        
        self.Define_Resolution(5)

        ###self.ip = "10.194.116.98"
        self.ip = "192.168.250.251"
        
        self.port1 = "5556"
        self.port2 = "5557"
        self.port3 = "5558"
        self.port4 = "5559"
        self.port5 = "5560"

        self.context = zmq.Context()
        self.socket1 = self.context.socket(zmq.REQ)
        self.socket1.connect("tcp://%s:%s" % (self.ip,self.port1))
        self.socket2 = self.context.socket(zmq.REQ)
        self.socket2.connect("tcp://%s:%s" % (self.ip,self.port2))
        self.socket3 = self.context.socket(zmq.REQ)
        self.socket3.connect("tcp://%s:%s" % (self.ip,self.port3))
        self.socket4 = self.context.socket(zmq.REQ)
        self.socket4.connect("tcp://%s:%s" % (self.ip,self.port4))
        self.socket5 = self.context.socket(zmq.REQ)
        self.socket5.connect("tcp://%s:%s" % (self.ip,self.port5))
        QtGui.QWidget.__init__(self, parent)
        self.graphical_intf()


        timer = QtCore.QTimer(self)       
        QtCore.QObject.connect(timer, QtCore.SIGNAL("timeout()"), self.on_timer)
        timer.start(200) ### 0.2 second
        self.initialize()

        
    def Define_Resolution(self, Resolution):
        self.incr_x = Resolution
        self.nb_x_val = (self.last_x_val - self.first_x_val) / self.incr_x
        self.value_y = array([0 for i in range(self.nb_x_val)], dtype=float)
        self.value_x = array([((i*self.incr_x) + self.first_x_val) for i in range(self.nb_x_val)], dtype=float)
        self.value_x_inmm = array([((i*self.incr_x)/5) for i in range(self.nb_x_val)], dtype=float)
        self.Start_scanning()
        
    def Change_Resolution(self):
        self.Define_Resolution((self.Change_Res.currentIndex()+1)*5)
        
    def initialize(self):
        answer = self.request_serial_1("CONFigure:OUT1 IN")    
        answer = self.request_serial_1("CONFigure:OUT2 IN")
        answer = self.request_serial_1("CONFigure:OUT3 IN")
        answer = self.request_serial_1("CONFigure:OUT4 IN")
        answer = self.request_serial_1("CONFigure:MUX REF_CLK")  
        answer = self.request_serial_1("CONFigure:PLLin REF_CLK")    
        answer = self.request_serial_2("CONFigure:DELay:A:VALue 0") 
        answer = self.request_serial_2("CONFigure:DELay:B:VALue 0")
        answer = self.request_serial_2("CONFigure:DELay:A:ENable 1") 
        answer = self.request_serial_2("CONFigure:DELay:B:ENable 1") 
        answer = self.request_serial_3("CONFigure:OUT1 OR") 
        answer = self.request_serial_3("CONFigure:OUT2 IN1") 
        answer = self.request_serial_3("CONFigure:OUT3 IN1") 
        answer = self.request_serial_3("CONFigure:OUT4 IN1") 
        answer = self.request_serial_4("CONFigure:COUnter:A:ENable 1") 
        answer = self.request_serial_4("CONFigure:COUnter:B:ENable 1")
        answer = self.request_serial_4("CONFigure:TTLinputenable 0")
        answer = self.request_serial_4("CONFigure:FREQuence %d" % (self.frequence))
        
                
    def request_serial_1(self, message):
        self.socket1.send(message) 
        text = self.socket1.recv()        
        return text
    def request_serial_2(self, message):
        self.socket2.send(message) 
        text = self.socket2.recv()        
        return text
    def request_serial_3(self, message):
        self.socket3.send(message) 
        text = self.socket3.recv()        
        return text
    def request_serial_4(self, message):
        self.socket4.send(message) 
        text = self.socket4.recv()        
        return text
    def request_serial_5(self, message):
        self.socket5.send(message) 
        text = self.socket5.recv()        
        return text

        
    def on_timer(self):
        answer = ' '
        while not(re.search("No new val", answer)): 
            answer = self.request_serial_4("CONFigure:COUnter:A:Read?") 
            if not(re.search("No new val", answer)): 
                if (re.search("Error", answer, re.IGNORECASE)): break
                if  self.alternate == 2: 
                    self.value_y[self.count_prev] = ((int(answer)*4.0)/self.frequence*1.0)*100
                    self.pb.setValue(self.count*100.0/self.nb_x_val*1.0)
                    ###self.freqal.setText("Frequency A = %d" % (int(answer)*4.0))      
                    self.dca.update_figure(self.nb_x_val, self.value_x_inmm, self.value_y)
                    self.alternate = 0
                elif (self.alternate == 0):    
                    if (self.count == (self.nb_x_val - 1)):
                        self.count = 0
                        #self.value_y = array([0 for i in range(self.nb_x_val)], dtype=float)
                    else:
                        self.count_prev = self.count
                        self.count = self.count + 1
                        answer = self.request_serial_2("CONFigure:DELay:B:VALue %d" % (self.value_x[self.count]))
                        ###self.delayb.setText("Delay B = %d" % (self.value_x[self.count]))
                    self.alternate = self.alternate + 1
                else:    
                    self.alternate = self.alternate + 1

        
    ###def on_delaint_valueChanged(self):
    ###    answer = self.request_serial_2("CONFigure:DELay:A:VALue %d" % (self.delaint.value()))


    def get_threshold(self):
        for i in range(self.nb_x_val):
            if self.value_y[i] <= 50: 
                percent_y = (self.value_y[i-1]*1.0 - 50) / (self.value_y[i-1]*1.0 - self.value_y[i]*1.0)
                delta_x = self.value_x_inmm[i] - self.value_x_inmm[i-1]
                return (self.value_x_inmm[i-1] + (percent_y*(self.value_x_inmm[i]*1.0 - self.value_x_inmm[i-1]*1.0)))
                break
        return False 
        
    
    def Calibrate_data(self):
        self.cal_data = self.get_threshold()
        if self.cal_data:
            self.calibrated = True
            self.MesureL.setText("")       
            self.CableL.setText("")       
            self.CalibrateL.setText("Reference distance is: %.2f mm at 5 ns/m" % self.cal_data)       
        else:            
            self.CalibrateL.setText("Error: System not ready")       

    def Mesure_data(self): 
        if  self.calibrated: 
            mes_data = self.get_threshold()
            if mes_data:
                self.MesureL.setText("Mesured distance is: %.2f mm at 5 ns/m" % mes_data)       
                self.CableL.setText("Length is: %.2f mm" % abs(mes_data-self.cal_data))       
            else:            
                self.MesureL.setText("Error: System not ready")       
        else:            
            self.MesureL.setText("Error: System needs to be calibrated first")       

    def Start_scanning(self):
        self.value_y = array([0 for i in range(self.nb_x_val)], dtype=float)
        self.alternate == 0
        self.count_prev = 0
        self.count = 0

    def ToggleFullScreen(self):
        if self.fullScreen:
            self.showNormal()
            self.fullScreen = False
        else:
            self.showFullScreen() 
            self.fullScreen = True

            
    def graphical_intf(self):       
        self.setObjectName("self")
        self.resize(1500, 900)
        self.setWindowTitle("Easy-phi client")

        
        ###self.delaint = QtGui.QSpinBox(self)
        ###self.delaint.setGeometry(QtCore.QRect(50, 700, 60, 27))
        ###self.delaint.setMinimum(0);
        ###self.delaint.setMaximum(5115);
        ###self.delaint.setValue(0);
        ###self.delaint.setObjectName("delaint")
        ###self.delaint.valueChanged.connect(self.on_delaint_valueChanged)
        ###self.delaintl = QtGui.QLabel(self)
        ###self.delaintl.setGeometry(QtCore.QRect(150, 700, 250, 27))
        ###self.delaintl.setText("Delay A value (ps)")       
        ###
        ###
        ###self.delayb = QtGui.QLabel(self)
        ###self.delayb.setGeometry(QtCore.QRect(50, 630, 250, 27))
        ###self.delayb.setText("Delay B = 0")       
        ###
        ###self.freqal = QtGui.QLabel(self)
        ###self.freqal.setGeometry(QtCore.QRect(50, 660, 250, 27))
        ###self.freqal.setText("Frequency Counter A = 0")       
        self.uni = QtGui.QLabel(self)
        self.uni.setGeometry(QtCore.QRect(50, 20, 700, 40))
        self.uni.setText("University of Geneva 2014")       
        self.uni.setStyleSheet('QLabel {font-size: 28px;font-weight: bold}')
        self.ws = QtGui.QLabel(self)
        self.ws.setGeometry(QtCore.QRect(50, 60, 700, 40))
        self.ws.setText("www.easy-phi.ch")       
        self.ws.setStyleSheet('QLabel {font-size: 28px;font-weight: bold}')
        self.luni = QtGui.QLabel(self)
        self.luni.setPixmap(QtGui.QPixmap("logo_blanc.jpg")) 
        self.luni.setGeometry(QtCore.QRect(440, 20, 700, 70))
        self.ep = QtGui.QLabel(self)
        self.ep.setPixmap(QtGui.QPixmap("easy-phi.jpg")) 
        self.ep.setGeometry(QtCore.QRect(670, 20, 700, 70))
        self.qsit = QtGui.QLabel(self)
        self.qsit.setPixmap(QtGui.QPixmap("logoqsit.jpg")) 
        self.qsit.setGeometry(QtCore.QRect(900, 10, 750, 120))
       
        self.framea = QtGui.QFrame(self)
        self.framea.setGeometry(QtCore.QRect(50, 100, 900, 600))
        self.framea.setFrameShape(QtGui.QFrame.StyledPanel)
        self.framea.setFrameShadow(QtGui.QFrame.Raised)
        self.framea.setObjectName("framea")

        self.local_widgeta = QtGui.QWidget(self.framea)
        self.la = QtGui.QVBoxLayout(self.local_widgeta)
        self.dca = MyMplCanvas(self.local_widgeta, width=10, height=6, dpi=80)
        self.la.addWidget(self.dca)

        self.pb = QtGui.QProgressBar(self)
        self.pb.setGeometry(QtCore.QRect(160, 620, 650, 40))

        self.Start_scan = QtGui.QPushButton(self)
        self.Start_scan.setGeometry(QtCore.QRect(30, 620, 120, 40))
        self.Start_scan.setText("Start Scan")
        self.Start_scan.setObjectName("Start_scan")
        self.Start_scan.clicked.connect(self.Start_scanning) 
        self.Start_scan.setStyleSheet('QPushButton {font-size: 22px}')
       
        self.Calibrate = QtGui.QPushButton(self)
        self.Calibrate.setGeometry(QtCore.QRect(30, 700, 120, 40))
        self.Calibrate.setText("Calibrate")
        self.Calibrate.setObjectName("Calibrate")
        self.Calibrate.clicked.connect(self.Calibrate_data) 
        self.Calibrate.setStyleSheet('QPushButton {font-size: 22px}')

        self.CalibrateL = QtGui.QLabel(self)
        self.CalibrateL.setGeometry(QtCore.QRect(200, 700, 700, 40))
        self.CalibrateL.setText("")       
        self.CalibrateL.setStyleSheet('QLabel {font-size: 22px}')
        
        self.Mesure = QtGui.QPushButton(self)
        self.Mesure.setGeometry(QtCore.QRect(30, 750, 120, 40))
        self.Mesure.setText("Mesure")
        self.Mesure.setObjectName("Mesure")
        self.Mesure.clicked.connect(self.Mesure_data) 
        self.Mesure.setStyleSheet('QPushButton {font-size: 22px}')

        self.MesureL = QtGui.QLabel(self)
        self.MesureL.setGeometry(QtCore.QRect(200, 750, 700, 40))
        self.MesureL.setText("")       
        self.MesureL.setStyleSheet('QLabel {font-size: 22px}')
        
        self.CableL = QtGui.QLabel(self)
        self.CableL.setGeometry(QtCore.QRect(200, 800, 700, 40))
        self.CableL.setText("")       
        self.CableL.setStyleSheet('QLabel {font-size: 28px;font-weight: bold}')

        self.Change_Res = QtGui.QComboBox(self)
        self.Change_Res.setGeometry(QtCore.QRect(900, 150, 300, 40))
        self.Change_Res.insertItem(5, " 5 ps Time Resolution")
        self.Change_Res.insertItem(10,"10 ps Time Resolution")
        self.Change_Res.insertItem(20,"20 ps Time Resolution")
        self.Change_Res.insertItem(50,"50 ps Time Resolution")
        self.Change_Res.activated.connect(self.Change_Resolution) 
        self.Change_Res.setStyleSheet('QComboBox {font-size: 22px;}')
        
        self.FullScreen = QtGui.QPushButton(self)
        self.FullScreen.setGeometry(QtCore.QRect(1000, 800, 40, 40))
        self.FullScreen.setText("FS")
        self.FullScreen.clicked.connect(self.ToggleFullScreen) 
       
        QtCore.QMetaObject.connectSlotsByName(self)
                
app = QtGui.QApplication(sys.argv)
myapp = StartQT4()
myapp.show()

sys.exit(app.exec_())
