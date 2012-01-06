#include <QtGui>
#include <QMutex>
#include <QGalleryTab.h>
#include <pix_entries.h>
extern struct pix_entries *entriesGet(struct pix_entries *e,int i);
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
  setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
  reArrange();
}


void QGallery::redraw() {
  this->resizeMutex.lock();
  cleanUp();
  //this->update();
  reArrange();
  this->resizeMutex.unlock();
};

void QGallery::resizeEvent(QResizeEvent *event) {
  //printf("Ok I am catching a resize event!: %i, %i\n",this->sizePolicy().horizontalPolicy(),this->sizePolicy().verticalPolicy());
  redraw();
}

void QGallery::setupIcons() {

  QCheckBox *b;
  struct pix_entries *pe;
  QIcon icon;
  int i;
  int N = entriesLen(this->pix);
  int W=((QGalleryTab *)this->parentWidget())->iconsSizeSlider->maximum();
  char tip[256];
  for (i=0;i<N;i++) {
    	b = new QCheckBox();
	pe = entriesGet(this->pix,i);
	QImageReader reader; 
	sprintf(tip,"%s\n%sLon:%.2f\nLat:%.2f",pe->entry.name,asctime(localtime(&pe->entry.time)),pe->entry.lon,pe->entry.lat);
	// Set image name
	reader.setFileName(pe->entry.name);
	
	// Read image current size
	QSize imageSize = reader.size();
	
	// Scale image to fit to screen
	imageSize.scale(QSize(W,W), Qt::KeepAspectRatio);
	
	// Set wanted image size for reader
	reader.setScaledSize(imageSize);
	
	// Read image
	QImage image = reader.read();
	
	// Make QPixmap (if needed)
	QPixmap pixmap = QPixmap::fromImage(image);
	
	icon= QIcon(pixmap);
	b->setIcon(icon);
	//b->setIconSize( QSize(W,W));
	b->setChecked(Qt::Checked);
	b->setToolTip(tip);
	this->childs.append(b);
  }
}

void QGallery::cleanUp() {

  int i;
  QSize s = this->size();
  //fprintf(stderr,"REARRANGE: %i elts into a %ix%i frame\n",this->childs.size(),s.width(),s.height());
  for (i=0;i<this->grid->count();i++) {
    QLayoutItem *it = this->grid->itemAt(i);
    this->grid->removeItem(it);
  }
  for(i=0;i<this->childs.size();i++) {
    childs[i]->hide();
  }
}
  
void QGallery::reArrange() {

  int i,j,n,w;  
  int W=((QGalleryTab *)this->parentWidget())->iconsSizeSlider->value();
  QSize s = this->parentWidget()->size();
  float S = .75*s.width();
  
  n=0;
  i=0;
  while (n<this->childs.size()) {
    w= 0;
    j=0;
    //fprintf(stderr,"i: %i, w\n",i);
    while ((n<this->childs.size()) && (((float)w<S-(float)W) || (w==0))) {
      this->grid->addWidget(childs[n],i,j);
      ((QCheckBox *)(childs)[n])->setIconSize(QSize(W,W));
      childs[n]->show();
      //fprintf(stderr,"Adding a widget at: %i, %i\n",i,j);
      w+=W;
      j++;
      n++;
    }
    i++;
  }
  //fprintf(stderr,"ok we now have : %i widgets\n",n);
  //setLayout(g);
  //}
}

