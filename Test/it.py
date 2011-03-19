import PixStudio
import sys
from PyQt4 import QtCore, QtGui

#print dir(PixStudio.EXIF)
fnm = "/Users/doutriaux1/Desktop/IMAG0211.jpg"
#fnm = "/Users/doutriaux1/Desktop/VIDEO0094.3gp"

f= open(fnm)

tags = PixStudio.EXIF.process_file(f)

print tags.keys()

for k in tags.keys():
    if k not in ('JPEGThumbnail', 'TIFFThumbnail', 'Filename',
                   'EXIF MakerNote'):
        print k,tags[k]
#print myexif


app = QtGui.QApplication(sys.argv)

Main = QtGui.QMainWindow()


scene = QtGui.QGraphicsScene()
view = QtGui.QGraphicsView(scene);
item  = QtGui.QGraphicsPixmapItem(QtGui.QPixmap(fnm));
scene.addItem(item);
view.show();

sys.exit(app.exec_())
