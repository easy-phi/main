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

        

        
        self.delaen = QtGui.QRadioButton(Form)
        self.delaen.setGeometry(QtCore.QRect(50, 50, 200, 27))
        self.delaen.setAutoExclusive(False)
        self.delaen.setText("Enable Delay A")
        self.delaen.setObjectName("delaen")

        self.delben = QtGui.QRadioButton(Form)
        self.delben.setGeometry(QtCore.QRect(50, 80, 200, 27))
        self.delben.setAutoExclusive(False)
        self.delben.setText("Enable Delay B")
        self.delben.setObjectName("delben")

        self.delaint = QtGui.QSpinBox(Form)
        self.delaint.setGeometry(QtCore.QRect(50, 110, 60, 27))
        self.delaint.setMinimum(0);
        self.delaint.setMaximum(5115);
        self.delaint.setValue(0);
        self.delaint.setObjectName("delaint")
        self.delaintl = QtGui.QLabel(Form)
        self.delaintl.setGeometry(QtCore.QRect(150, 110, 250, 27))
        self.delaintl.setText("Delay A value (ps)")       

        self.delbint = QtGui.QSpinBox(Form)
        self.delbint.setGeometry(QtCore.QRect(50, 140, 60, 27))
        self.delbint.setMinimum(0);
        self.delbint.setMaximum(5115);
        self.delbint.setValue(0);
        self.delbint.setObjectName("delbint")
        self.delbintl = QtGui.QLabel(Form)
        self.delbintl.setGeometry(QtCore.QRect(150, 140, 250, 27))
        self.delbintl.setText("Delay B value (ps)")       

        self.line2 = QtGui.QFrame(Form)
        self.line2.setGeometry(QtCore.QRect(50, 170, 250, 10))
        self.line2.setFrameShape(QtGui.QFrame.HLine)
        self.line2.setFrameShadow(QtGui.QFrame.Sunken)

        
        
        QtCore.QMetaObject.connectSlotsByName(Form)

