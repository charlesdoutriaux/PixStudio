#include <QtGui>

#ifndef QPATHLISTWIDGET_H
#define QPATHLISTWIDGET_H

class QPathListWidget : public QListWidget
{
 Q_OBJECT
   public :
  QPathListWidget();
  void keyPressEvent(QKeyEvent *event);
};


#endif
