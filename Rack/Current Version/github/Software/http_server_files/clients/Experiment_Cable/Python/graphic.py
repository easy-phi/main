#######-------------------------------------------------------------------------------------
#######Easy-phi project: http://easy-phi.ch/
#######Developer : Raphael Houlmann, University of Geneva 2014
#######Licensed under GPL, for details report to: http://easy-phi.ch/index.php/about/licensing
#######Do NOT remove this notice
#######-------------------------------------------------------------------------------------
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure
from PyQt4 import QtCore, QtGui
from pylab import *
from ctypes import *

   
class MyMplCanvas(FigureCanvas):
    def __init__(self, parent=None, width=11, height=4, dpi=100):
        fig = Figure(figsize=(width, height), dpi=dpi)
        self.axes = fig.add_subplot(111)     

        FigureCanvas.__init__(self, fig)
	# We want the axes cleared every time plot() is called
        self.axes.hold(False)
        #self.axes.set_autoscale_on(True)
        #self.axes.set_autoscaley_on(True)
        #self.axes.set_ybound(lower=-10, upper=-5)

        ####self.up = 100
        ####self.down = 1
        ####self.a = array([i for i in range(self.up)], dtype=int64)
        ####self.b = array([i for i in range(self.up)], dtype=int64)
        ####for i in range(self.up): self.a[i] = i
        ####for i in range(self.up): self.b[i] = i+1

        ###self.axes.plot(self.b, self.a, '-')


    def update_figure(self, rangex, xvalues, yvalues):
        self.axes.plot(xvalues, yvalues, '-')
        self.axes.set_ylim(0, 120)
        self.draw()

