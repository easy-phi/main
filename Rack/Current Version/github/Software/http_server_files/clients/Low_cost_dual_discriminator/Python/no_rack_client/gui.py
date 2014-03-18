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

        
        self.couplingal = QtGui.QLabel(Form)
        self.couplingal.setGeometry(QtCore.QRect(50, 50, 250, 27))
        self.couplingal.setText("Coupling Channel A:")       
        self.couplinga = QtGui.QComboBox(Form)
        self.couplinga.setGeometry(QtCore.QRect(160, 50, 50, 24))
        self.couplinga.setObjectName("couplinga")
        self.couplinga.insertItem(0,"AC")
        self.couplinga.insertItem(1,"DC")

        self.couplingbl = QtGui.QLabel(Form)
        self.couplingbl.setGeometry(QtCore.QRect(50, 80, 250, 27))
        self.couplingbl.setText("Coupling Channel B:")       
        self.couplingb = QtGui.QComboBox(Form)
        self.couplingb.setGeometry(QtCore.QRect(160, 80, 50, 24))
        self.couplingb.setObjectName("couplingb")
        self.couplingb.insertItem(0,"AC")
        self.couplingb.insertItem(1,"DC")
        
        self.offaen = QtGui.QRadioButton(Form)
        self.offaen.setGeometry(QtCore.QRect(50, 110, 200, 27))
        self.offaen.setAutoExclusive(False)
        self.offaen.setText("Enable Offset on channel A")
        self.offaen.setObjectName("offaen")
        
        self.offben = QtGui.QRadioButton(Form)
        self.offben.setGeometry(QtCore.QRect(50, 140, 200, 27))
        self.offben.setAutoExclusive(False)
        self.offben.setText("Enable Offset on channel B")
        self.offben.setObjectName("offben")
        
        self.offaval = QtGui.QSpinBox(Form)
        self.offaval.setGeometry(QtCore.QRect(50, 170, 60, 27))
        self.offaval.setMinimum(-2500);
        self.offaval.setMaximum(2500);
        self.offaval.setValue(0);
        self.offaval.setObjectName("offaval")
        self.offavall = QtGui.QLabel(Form)
        self.offavall.setGeometry(QtCore.QRect(120, 170, 250, 27))
        self.offavall.setText("[mV] Channel A Offset")       
        
        self.offbval = QtGui.QSpinBox(Form)
        self.offbval.setGeometry(QtCore.QRect(50, 200, 60, 27))
        self.offbval.setMinimum(-2500);
        self.offbval.setMaximum(2500);
        self.offbval.setValue(0);
        self.offbval.setObjectName("offbval")
        self.offbvall = QtGui.QLabel(Form)
        self.offbvall.setGeometry(QtCore.QRect(120, 200, 250, 27))
        self.offbvall.setText("[mV] Channel B Offset")       


        self.thrsaval = QtGui.QSpinBox(Form)
        self.thrsaval.setGeometry(QtCore.QRect(50, 230, 60, 27))
        self.thrsaval.setMinimum(-5000);
        self.thrsaval.setMaximum(5000);
        self.thrsaval.setValue(0);
        self.thrsaval.setObjectName("thrsaval")
        self.thrsavall = QtGui.QLabel(Form)
        self.thrsavall.setGeometry(QtCore.QRect(120, 230, 250, 27))
        self.thrsavall.setText("[mV] Channel A Threshold")       
        
        self.thrsbval = QtGui.QSpinBox(Form)
        self.thrsbval.setGeometry(QtCore.QRect(50, 260, 60, 27))
        self.thrsbval.setMinimum(-5000);
        self.thrsbval.setMaximum(5000);

        self.thrsbval.setValue(0);
        self.thrsbval.setObjectName("thrsbval")
        self.thrsbvall = QtGui.QLabel(Form)
        self.thrsbvall.setGeometry(QtCore.QRect(120, 260, 250, 27))
        self.thrsbvall.setText("[mV] Channel B Threshold")       
        
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

        
        self.testwrite = QtGui.QPushButton(Form)
        self.testwrite.setGeometry(QtCore.QRect(50, 450, 80, 27))
        self.testwrite.setAutoExclusive(False)
        self.testwrite.setText("Test Write SPI")
        self.testwrite.setObjectName("testwrite")
        self.testwriteval = QtGui.QSpinBox(Form)
        self.testwriteval.setGeometry(QtCore.QRect(150, 450, 60, 27))
        self.testwriteval.setMinimum(0);
        self.testwriteval.setMaximum(65535);
        self.testwriteval.setValue(0);
        self.testwriteval.setObjectName("testwriteval")
        
        self.testread = QtGui.QPushButton(Form)
        self.testread.setGeometry(QtCore.QRect(50, 480, 80, 27))
        self.testread.setAutoExclusive(False)
        self.testread.setText("Test Read SPI")
        self.testread.setObjectName("testread")
        self.testreadl = QtGui.QLabel(Form)
        self.testreadl.setGeometry(QtCore.QRect(150, 480, 250, 27))
        self.testreadl.setText("0")       
        
        QtCore.QMetaObject.connectSlotsByName(Form)

