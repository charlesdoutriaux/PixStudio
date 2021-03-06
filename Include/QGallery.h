#ifndef QGALLERY_H
#define QGALLERY_H
#include <QtGui>
#include <time.h>

class QGallery : public QScrollArea
{
 Q_OBJECT
   public :
  QGallery( QWidget *parent,  struct pix_entries *pix);
  void setupIcons();
  struct pix_entries *pix;
  void resizeEvent(QResizeEvent *event);
  void reArrange();
  void cleanUp();
  void redraw();
  QList<QWidget*> childs;
 private:
  QMutex resizeMutex;
  QGridLayout *grid;
  QFrame *f;
  time_t lastUpdate; //timer to make sure we don't update too often
};
#endif
