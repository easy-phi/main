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

class slot(QtGui.QMainWindow):
    def __init__(self, ui, address, port, tab): 
        self.tab = tab
        self.tab_active = False
        self.empty = True

    def initialize(self):pass
    def on_timer(self): pass

    def graphical_intf(self):       
        self.empty = QtGui.QLabel(self.tab)
        self.empty.setGeometry(QtCore.QRect(50, 50, 250, 27))
        self.empty.setText("")       
        
        