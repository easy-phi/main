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

import empty_slot as slot_1
import empty_slot as slot_2
import empty_slot as slot_3
import empty_slot as slot_4
import empty_slot as slot_5

class StartQT4(QtGui.QMainWindow):
    def __init__(self, parent=None):
        self.logactive = 0
        self.resetactive = 0
        self.port = "5555"
        self.ip = "192.168.250.251"
        self.ip = "10.42.2.1"
        ##self.ip = "10.42.1.1"
        ##self.ip = "129.194.118.114"
        ##self.ip = "10.194.116.98"
        self.nb_slots = 5
        self.context = zmq.Context()
        self.socket = self.context.socket(zmq.REQ)
        self.socket.connect("tcp://%s:%s" % (self.ip,self.port))
        QtGui.QWidget.__init__(self, parent)
        self.graphical_intf()

        self.slots = [] 
        self.frames = [] 
        for i in range(self.nb_slots):
            self.frames.append(QtGui.QFrame(self.tab[i + 1]))
            self.frames[i].setGeometry(QtCore.QRect(0, 50, 950, 750))
            self.frames[i].setStyleSheet("QWidget {background-color: #EEEEFF }")
            exec('self.slots.append(slot_%d.slot(self, "10.194.116.98", str(5556+i), self.frames[i]))' % (i+1))
            self.slots[i].graphical_intf()

        timer = QtCore.QTimer(self)       
        QtCore.QObject.connect(timer, QtCore.SIGNAL("timeout()"), self.on_timer)
        timer.start(200) ### 0.2 second


    def activate_tab(self):
        for i in range(self.nb_slots): 
            if ((i+1) == self.tabwidget.currentIndex()):
                self.slots[i].tab_active = True
            else:
                self.slots[i].tab_active = False
                
    def request_serial(self, message):
        self.socket.send(message) 
        text = self.socket.recv()        
        return text
       
    def on_timer(self):
        for i in range(self.nb_slots): 
            status = self.request_serial("sl:%d:status?" % (i + 1))
            type = self.request_serial("sl:%d:type?" % (i + 1))
            lock = self.request_serial("sl:%d:locked?" % (i + 1))
            self.slotstext[i].setText(("Slot %d:    " % (i+1)) + status + "  " + type + "    " + lock)

            if (status == "Power On"): 
                if (self.slots[i].empty):
                    if (lock == "unlocked") : 
                        self.slotload[i].setDisabled(0)
                    else : 
                        self.slotload[i].setDisabled(1)
                else:    
                    self.slots[i].on_timer()
            else:        
                if not(self.slots[i].empty):   ###### slot removed
                    self.release_slot(i)
                
    def cmd_line(self):
        if (self.writetext.text() != ''):
            text = self.request_serial(str(self.writetext.text()))
            self.readtext.setText(text)       

    def load_active_slot(self):
        self.load_slot(self.tabwidget.currentIndex() - 1)
        
    def release_active_slot(self):
        self.release_slot(self.tabwidget.currentIndex() - 1)

    def force_unlock(self):
        self.release_slot(self.unlockport.value() -1 )

    def force_lock(self):
        lock = self.request_serial("sl:%d:locked 1" % (self.lockport.value()))
        
    def load_slot(self, i):
        self.frames[i].setParent(None) 
        urllib.urlretrieve(("http://%s/sd_cards/slot_%d/Python/slot.py" % (self.ip, i+1)),("slot_%d.py" % (i+1)))
        exec("import slot_%d as slot_%d" % (i+1, i+1))
        self.frames[i] = (QtGui.QFrame(self.tab[i+1]))
        self.frames[i].setGeometry(QtCore.QRect(0, 50, 950, 750))
        self.frames[i].setStyleSheet("QWidget {background-color: #EEEEFF }")
        exec('self.slots[i] = slot_%d.slot(self, "%s", str(5556+i), self.frames[i])' % ((i+1), self.ip))
        lock = self.request_serial("sl:%d:locked 1" % (i + 1))
        self.slots[i].graphical_intf()
        self.slots[i].initialize()
        self.slotrelease[i].setDisabled(0)       
        self.slotload[i].setDisabled(1)       
        self.frames[i].show()
        self.activate_tab()

    def release_slot(self, i):
        self.frames[i].setParent(None) 
        exec("import empty_slot as slot_%d" % (i+1))
        self.frames[i] = (QtGui.QFrame(self.tab[i+1]))
        self.frames[i].setGeometry(QtCore.QRect(0, 50, 950, 750))
        self.frames[i].setStyleSheet("QWidget {background-color: #EEEEFF }")
        exec('self.slots[i] = slot_%d.slot(self, "%s", str(5556+i), self.frames[i])' % ((i+1), self.ip))
        lock = self.request_serial("sl:%d:locked 0" % (i + 1))
        self.slots[i].graphical_intf()
        self.slotrelease[i].setDisabled(1)       
        self.slotload[i].setDisabled(1)       
        self.frames[i].show()

        #self.errortext = QtGui.QLabel(self.tab[3])
        #self.errortext.setGeometry(QtCore.QRect(50, 100, 800, 100))
        #self.errortext.setText("Error found on slot 3")       
        #self.errortext.setStyleSheet('QLabel{background-color: white; color: red; font-size: 50pt; text-align: center; }')
        #self.errortext.setParent(None)
                      
    def graphical_intf(self):       
        self.setObjectName("self")
        self.resize(1000, 800)
        self.setWindowTitle("Easy-phi client")

        self.tabwidget = QtGui.QTabWidget(self)
        self.tabwidget.setGeometry(QtCore.QRect(0, 10, 1000, 850))
        self.tabwidget.setObjectName("tabwidget")
        self.tab = []
        self.tab.append(QtGui.QWidget())
        self.tabwidget.addTab(self.tab[0], "CNTRL")
        self.tabwidget.currentChanged.connect(self.activate_tab)

        self.slotstext = []
        for i in range(self.nb_slots):
            self.slotstext.append(QtGui.QLabel(self.tab[0]))
            self.slotstext[i].setGeometry(QtCore.QRect(50, (100 + 30 * i), 500, 27))
            self.slotstext[i].setObjectName("slot%d_text" % i)   
            self.slotstext[i].setText("Slot %d no connected" % i)       

        self.unlock = QtGui.QPushButton(self.tab[0])
        self.unlock.setGeometry(QtCore.QRect(50, 50, 120, 27))
        self.unlock.setText("Force unlock Slot:")
        self.unlock.setObjectName("unlock")
        self.unlock.clicked.connect(self.force_unlock) 
        self.unlockport = QtGui.QSpinBox(self.tab[0])
        self.unlockport.setGeometry(QtCore.QRect(200, 50, 60, 27))
        self.unlockport.setMinimum(1);
        self.unlockport.setMaximum(self.nb_slots)
        self.unlockport.setValue(0);

        self.lock = QtGui.QPushButton(self.tab[0])
        self.lock.setGeometry(QtCore.QRect(300, 50, 120, 27))
        self.lock.setText("Force lock Slot:")
        self.lock.setObjectName("lock")
        self.lock.clicked.connect(self.force_lock) 
        self.lockport = QtGui.QSpinBox(self.tab[0])
        self.lockport.setGeometry(QtCore.QRect(450, 50, 60, 27))
        self.lockport.setMinimum(1);
        self.lockport.setMaximum(self.nb_slots)
        self.lockport.setValue(0);

        
        
        self.write = QtGui.QPushButton(self.tab[0])
        self.write.setGeometry(QtCore.QRect(50, 650, 80, 27))
        self.write.setText("Write")
        self.write.setObjectName("write")
        self.write.clicked.connect(self.cmd_line) 
        self.writetext = QtGui.QLineEdit(self.tab[0])
        self.writetext.setGeometry(QtCore.QRect(150, 650, 250, 27))
        self.writetext.setObjectName("writetext")   
        self.writetext.setText("")       
        self.writetext.returnPressed.connect(self.cmd_line) 
        self.readtext = QtGui.QLabel(self.tab[0])
        self.readtext.setGeometry(QtCore.QRect(550, 650, 400, 27))
        self.readtext.setText("")       
       
        self.slotload = []
        self.slotrelease = []
        for i in range(self.nb_slots):
            number = i + 1
            self.tab.append(QtGui.QWidget())
            self.tabwidget.addTab(self.tab[number], ("SLOT %d" % number))
            self.slotload.append(QtGui.QPushButton(self.tab[number]))
            self.slotload[i].setGeometry(QtCore.QRect(10, 10, 80, 27))
            self.slotload[i].setObjectName("slot%d_text" % i)   
            self.slotload[i].setText("LOAD")       
            self.slotload[i].setDisabled(1)       
            self.slotload[i].clicked.connect(self.load_active_slot) 
            self.slotrelease.append(QtGui.QPushButton(self.tab[number]))
            self.slotrelease[i].setGeometry(QtCore.QRect(110, 10, 80, 27))
            self.slotrelease[i].setObjectName("slot%d_text" % i)   
            self.slotrelease[i].setText("RELEASE")       
            self.slotrelease[i].clicked.connect(self.release_active_slot) 
            self.slotrelease[i].setDisabled(1)       

        
        QtCore.QMetaObject.connectSlotsByName(self)
                
app = QtGui.QApplication(sys.argv)
myapp = StartQT4()
myapp.show()

sys.exit(app.exec_())
