#######-------------------------------------------------------------------------------------
#######Easy-phi project: http://easy-phi.ch/
#######Developer : Raphael Houlmann, University of Geneva 2014
#######Licensed under GPL, for details report to: http://easy-phi.ch/index.php/about/licensing
#######Do NOT remove this notice
#######-------------------------------------------------------------------------------------
from PyQt4 import QtCore, QtGui
from graphic import *

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(1000, 680)
        Form.setWindowTitle(QtGui.QApplication.translate("Detector Test", "Detector Test", None, QtGui.QApplication.UnicodeUTF8))

        
        self.couaen = QtGui.QRadioButton(Form)
        self.couaen.setGeometry(QtCore.QRect(50, 50, 200, 27))
        self.couaen.setAutoExclusive(False)
        self.couaen.setText("Enable Counter A")
        self.couaen.setObjectName("couaen")
        
        self.couben = QtGui.QRadioButton(Form)
        self.couben.setGeometry(QtCore.QRect(50, 80, 200, 27))
        self.couben.setAutoExclusive(False)
        self.couben.setText("Enable Counter B")
        self.couben.setObjectName("couben")
        
        self.couaint = QtGui.QSpinBox(Form)
        self.couaint.setGeometry(QtCore.QRect(50, 110, 60, 27))
        self.couaint.setMinimum(1);
        self.couaint.setMaximum(240);
        self.couaint.setValue(1);
        self.couaint.setObjectName("couaint")
        self.couaintl = QtGui.QLabel(Form)
        self.couaintl.setGeometry(QtCore.QRect(150, 110, 250, 27))
        self.couaintl.setText("Counter A integration time")       
        
        self.coubint = QtGui.QSpinBox(Form)
        self.coubint.setGeometry(QtCore.QRect(50, 140, 60, 27))
        self.coubint.setMinimum(1);
        self.coubint.setMaximum(240);
        self.coubint.setValue(1);
        self.coubint.setObjectName("coubint")
        self.coubintl = QtGui.QLabel(Form)
        self.coubintl.setGeometry(QtCore.QRect(150, 140, 250, 27))
        self.coubintl.setText("Counter B integration time")       

        self.line1 = QtGui.QFrame(Form)
        self.line1.setGeometry(QtCore.QRect(50, 170, 250, 10))
        self.line1.setFrameShape(QtGui.QFrame.HLine)
        self.line1.setFrameShadow(QtGui.QFrame.Sunken)

        
        self.ttl = QtGui.QRadioButton(Form)
        self.ttl.setGeometry(QtCore.QRect(50, 180, 200, 27))
        self.ttl.setAutoExclusive(False)
        self.ttl.setText("Set QMA out to the TTL input")
        self.ttl.setObjectName("ttl")
        
        self.freq = QtGui.QSpinBox(Form)
        self.freq.setGeometry(QtCore.QRect(50, 210, 60, 27))
        self.freq.setMinimum(100);
        self.freq.setMaximum(50000);
        self.freq.setValue(500);
        self.freq.setObjectName("freq")      
        self.freql = QtGui.QLabel(Form)
        self.freql.setGeometry(QtCore.QRect(150, 210, 250, 27))
        self.freql.setText("Frequence of test output kHZ")       

        self.line2 = QtGui.QFrame(Form)
        self.line2.setGeometry(QtCore.QRect(50, 290, 250, 10))
        self.line2.setFrameShape(QtGui.QFrame.HLine)
        self.line2.setFrameShadow(QtGui.QFrame.Sunken)

        self.cntareset = QtGui.QPushButton(Form)
        self.cntareset.setGeometry(QtCore.QRect(50, 300, 80, 27))
        self.cntareset.setAutoExclusive(False)
        self.cntareset.setText("Reset Count")
        self.cntareset.setObjectName("cntareset")
        self.cntal = QtGui.QLabel(Form)
        self.cntal.setGeometry(QtCore.QRect(150, 300, 250, 27))
        self.cntal.setText("0")       
       
        self.cntbreset = QtGui.QPushButton(Form)
        self.cntbreset.setGeometry(QtCore.QRect(50, 330, 80, 27))
        self.cntbreset.setAutoExclusive(False)
        self.cntbreset.setText("Reset Count")
        self.cntbreset.setObjectName("cntbreset")
        self.cntbl = QtGui.QLabel(Form)
        self.cntbl.setGeometry(QtCore.QRect(150, 330, 250, 27))
        self.cntbl.setText("0")       

        self.freqal = QtGui.QLabel(Form)
        self.freqal.setGeometry(QtCore.QRect(50, 360, 250, 27))
        self.freqal.setText("Frequency Counter A = 0")       
       
        self.freqbl = QtGui.QLabel(Form)
        self.freqbl.setGeometry(QtCore.QRect(50, 390, 250, 27))
        self.freqbl.setText("Frequency Counter B = 0")       

        self.framea = QtGui.QFrame(Form)
        self.framea.setGeometry(QtCore.QRect(300, 0, 610, 350))
        self.framea.setFrameShape(QtGui.QFrame.StyledPanel)
        self.framea.setFrameShadow(QtGui.QFrame.Raised)
        self.framea.setObjectName("framea")
        self.frameb = QtGui.QFrame(Form)
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

        
        QtCore.QMetaObject.connectSlotsByName(Form)

