#include <QtGui>
#include <QGalleryTab.h>
#include <pixstudio.h>

QGallery::QGallery(QWidget * parent,  struct pix_entries *pix)

  :QScrollArea(parent) {
  time(&this->lastUpdate);
  this->pix=pix;
  this->f = new QFrame();
  this->grid = new QGridLayout();
  this->f->setLayout(this->grid);
  setWidget(this->f);
  setWidgetResizable(true);
  
  setupIcons();
}

void QGallery::resizeEvent(QResizeEvent *event) {
  printf("Ok I am catching a resize event!\n");
  reArrange();
}

void QGallery::setupIcons() {

  QCheckBox *b;
  QIcon icon;
  int i;
  int N = entriesLen(this->pix);
  int W=100;
  for (i=0;i<N;i++) {
    	b = new QCheckBox();
	icon= QIcon(":Icons/refresh.png");
	b->setIcon(icon);
	b->setIconSize( QSize(W,W));
	b->setChecked(Qt::Checked);
	this->childs.append(b);
  }
}

void QGallery::reArrange() {

  int i,j,n,w,W=100;
  QSize s = this->f->size();
  fprintf(stderr,"REARRANGE: %i\n",this->childs.size());
  for (i=0;i<this->grid->count();i++) {
    this->grid->removeItem(this->grid->itemAt(i));
  }

    n=0;
    i=0;
    while (n<this->childs.size()) {
      w= 0;
      j=0;
      while ((n<this->childs.size()) && (w<s.width())) {
	this->grid->addWidget(childs[n],i,j);
	fprintf(stderr,"Adding a widget at: %i, %i\n",i,j);
	w+=W;
	j++;
	n++;
      }
      i++;
    }
    //setLayout(g);
    //}
}

