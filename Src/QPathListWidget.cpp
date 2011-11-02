#include <QtGui>
#include <Qt>
#include <QPathListWidget.h>

QPathListWidget::QPathListWidget() {
};

void QPathListWidget::keyPressEvent(QKeyEvent *event) {
  int i;
  QListWidgetItem *item;
  QList<QListWidgetItem *> items;
  if ((event->key() == Qt::Key_Delete) || (event->key()==Qt::Key_Backspace)){
    items = this->selectedItems();
    for (i=0;i<items.count();i++) {
      item = items.at(i);
      if (item->isSelected()) {
	item->setSelected(false);
	this->takeItem(this->row(item));
      }
    }
  }
  QListWidget::keyPressEvent(event);
}
