from PyQt5 import   QtWidgets, uic
from PyQt5.QtGui import QIcon
from PyQt5.QtCore import pyqtSlot
from PyQt5.QtGui import QColor
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QColorDialog
from PyQt5.QtWidgets import QMessageBox
import sys
import hidapi
import time

#
class  tempUI(QtWidgets.QMainWindow):
    temperatureWidget=None
    def __init__(self):
        super(tempUI,self).__init__()
        uic.loadUi('ui/temperature.ui',self)
        self.temperatureWidget=self.findChild(QtWidgets.QLCDNumber,'lcdNumber')
        print(self.temperatureWidget)
        if(self.temperatureWidget is None):
            print("No widget")
            exit(-1)
        self.updateTemp(21.22)
        self.show()
    def updateTemp(self,temperature):
        self.temperatureWidget.display(temperature)
    

#
vid = 0x1eaf	# Change it for your device
pid = 0x24	# Change it for your device

app=QtWidgets.QApplication(sys.argv)
#
bluepill=hidapi.Device(None,None,vid,pid,None,True)
if(bluepill is None):
     QMessageBox.about(None, "Error", "Sensor not found")
     exit(-1)
ui=tempUI()
#
rep=bytes(1)
length_id=2
round=0
while True:
    print("Read "+str(round))
    round=round+1
    out=bluepill.read(4,1000,True)
    if(out is not None):
        temp=out[0]*1000+out[1]*100+out[2]*10+out[3]
        temp=temp/100.
        print(temp)
        ui.updateTemp(temp)
        time.sleep(1)
    app.processEvents()
print("All done")

