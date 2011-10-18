#include <QtGui>
#include "Include/preferences.h"

QPrefs::QPrefs() {
  setupUi();
};

void QPrefs::setupUi() {
  QVBoxLayout *vLayout = new QVBoxLayout();
  QLabel *prefsLabel = new QLabel("Preferences");
  vLayout->addWidget(prefsLabel);
  setLayout(vLayout);
}

