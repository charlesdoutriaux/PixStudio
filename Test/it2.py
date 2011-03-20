import PixStudio
import sys
from PyQt4 import QtCore, QtGui#,QtOpenGL,phonon
from PyQt4.phonon import Phonon



app=QtGui.QApplication(sys.argv)
app.setApplicationName(u'd1')
app.setQuitOnLastWindowClosed(True)

## scene = QtGui.QGraphicsScene()
## view = QtGui.QGraphicsView(scene)
## view.setViewport(QtOpenGL.QGLWidget())
## view.resize(400, 300)
## view.show()
mediaObject = Phonon.MediaObject()
videoWidget = Phonon.VideoWidget()
audioOutput = Phonon.AudioOutput(Phonon.VideoCategory)
Phonon.createPath(mediaObject, videoWidget)
Phonon.createPath(mediaObject, audioOutput)
#audioOutput.setMuted(True)
main=QtGui.QMainWindow()
main.setCentralWidget(videoWidget)
fnm = "/Users/doutriaux1/Desktop/VIDEO0094.3gp"
mediaObject.setCurrentSource(Phonon.MediaSource(fnm))
#yaProxy.resize(videoWidget.size())
mediaObject.play()
#scene.addLine(50, 50, 700, 700, QtGui.QPen())
main.show()
sys.exit(app.exec_())
