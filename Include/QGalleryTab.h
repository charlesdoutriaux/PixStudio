#ifndef QGALLERY_TAB_H
#define QGALLERY_TAB_H
#include <QtGui>
#include <QGallery.h>
#include <pix_entries.h>
#include <preferences.h>

class QGalleryTab : public QWidget
{
 Q_OBJECT
   public :
  QGalleryTab( QWidget * parent, struct pix_entries *pix);
  QLabel *iconsSizeLabel;
  QSlider *iconsSizeSlider;
  QGallery *gallery;
  QLineEdit *name;
  QPrefs *prefs;
  QTabWidget *tabs;
  public slots:
  void newIconSize(int value);
  void renamePix(bool b);
};

#endif
