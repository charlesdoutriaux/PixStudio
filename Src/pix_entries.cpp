#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pix_entries.h>

#include <time.h>

#include <QMutex>

QMutex entriesMutex;

void entriesInit(struct pix_entries *pix) {
  pix->prev=NULL;
  pix->next=NULL;
  strcpy((char *)pix->entry.name,"");
  //strcpy((const char *)pix->entry.original_md5,"");
  pix->entry.lat=-999.;
  pix->entry.lon=-999.;
  pix->entry.time = (time_t) -9999999999999999;
  pix->entry.hasExif = false;
}

int entriesLen(struct pix_entries *entries) {
  int i;

  if (entries==NULL) return 0;

  // Now rewind
  while (entries->prev!=NULL) {
    entries=entries->prev;
  }

  // Empty ?
  if (strcmp(entries->entry.name,"")==0) {
    return 0;
  }
  i=0;
  while (entries->next!=NULL) {
    //printf("%i: %s--\n",i,entries->entry.name);
    i+=1;
    entries=entries->next;
  }
  //printf("%i: %s--\n",i,entries->entry.name);
  return i+1;
}
void entriesPrint(struct pix_entries *entries) {
  int i;
  time_t now;

  time(&now);
  if (entries==NULL) return;

  // Now rewind
  while (entries->prev!=NULL) {
    entries=entries->prev;
  }

  // Empty ?
  if (strcmp(entries->entry.name,"")==0) {
    return;
  }
  i=0;
  while (entries->next!=NULL) {
    printf("%i: %s--%s\n",i,entries->entry.name,ctime(&entries->entry.time));
    i+=1;
    entries=entries->next;
  }
  printf("%i: %s--%s\n",i,entries->entry.name,ctime(&entries->entry.time));
  return;
}

struct pix_entries *entriesGet(struct pix_entries *entries, int index) {
  int i;
  if (entries==NULL) return NULL;

  // Now rewind
  while (entries->prev!=NULL) {
    entries=entries->prev;
  }

  // Empty ?
  if (strcmp(entries->entry.name,"")==0) {
    return entries;
  }
  i=0;
  while ((i<index)&&(entries->next!=NULL)) {
    //printf("%i: %s--\n",i,iter->entry.name);
    i+=1;
    entries=entries->next;
  }
  if (i==index) {
    return entries;
  }
  else {
    return NULL;
  }

}

void entriesSwap(struct pix_entries *entries,int i1,int i2) {
  struct pix_entries *e1,*e2;
  int i3;
  struct pix_entry tmp;
  i3 = entriesLen(entries);
  if (i2>=i3) {
    return;
  }
  if (i1<0) {
    return;
  }
  if (i2==i1) return;

  /* First gets thepointers */
  e1 = entriesGet(entries,i1);
  e2 = entriesGet(entries,i2);
  tmp = (struct pix_entry) e1->entry;
  e1->entry = e2->entry;
  e2->entry=(struct pix_entry) tmp;
}


int entriesPartition(struct pix_entries *entries, int left, int right, int ipivot) {
  struct pix_entries *tmp;
  struct pix_entry pivot;
  int i,istore;
  double diff;
  tmp = entriesGet(entries,ipivot);
  pivot = tmp->entry;
  entriesSwap(entries,ipivot,right);
  istore=left;
  for (i=left;i<right;i++) {
    tmp = entriesGet(entries,i);
    // For now time comparison only
    diff = difftime(tmp->entry.time,pivot.time);
    if (diff<0.) {
      entriesSwap(entries,i,istore);
      istore+=1;
    }
  }
  entriesSwap(entries,istore,right);
  return istore;
}
void entriesFree(struct pix_entries *out) {
  struct pix_entries *tmp,*tmp2;
  tmp = out;
  int i=0;
  while (tmp->next!=NULL) {
    tmp2=tmp->next;
    //printf("tmp2a is: %p\n",tmp2);
    free(tmp);
    //printf("tmp2b is: %p\n",tmp2);
    tmp=NULL;
    tmp=tmp2;
    //printf("tmp is: %p\n",tmp);
    i++;
  }
  free(tmp);
  i++;
  //printf("WE freed: %i\n",i);
}

void entriesQuicksort(struct pix_entries *entries,int left, int right) {
  int ipivot,inewpivot;
  //struct pix_entries *tmp;
  if (left<right) {
    ipivot = left + (right-left)/2;
    //ipivot=left+1;
    inewpivot = entriesPartition(entries,left,right,ipivot);
    entriesQuicksort(entries,left,inewpivot-1);
    entriesQuicksort(entries,inewpivot+1,right);
  }
}

struct pix_entries ** entriesSplitPerTime(struct pix_entries *entries, int delta) {
  int i;
  bool anew=true,cont=true;
  struct pix_entries *current,**out;

  entriesMutex.lock();
  // Sadly it seems we need to first loop once to figure out the number of packs and then malloc this, seems inefficient
  i=1;
  current=entries;
  fprintf(stderr,"All right or entries is: %p\n",current);
  while (current->next!=NULL) {
    if (difftime(current->next->entry.time,current->entry.time)>delta) {
      i+=1;
    }
    current=current->next;
  }
  out = (struct pix_entries **) malloc(sizeof(struct pix_entries *)*(i+1));
  i=-1;
  while (cont) {
    if (anew) {
      i+=1;
      out[i] = (struct pix_entries *) malloc(sizeof(struct pix_entries));
      entriesInit(out[i]);
      current=out[i];
    }
    else {
      current->next=(struct pix_entries *)malloc(sizeof(struct pix_entries));
      entriesInit(current->next);
      current->next->prev=current;
      current=current->next;
    }
    current->entry=entries->entry;

    if (entries->next==NULL) {
      cont=false;
    }
    else {
      if (difftime(entries->next->entry.time,entries->entry.time)>delta) {
	anew=true;
      }
      else {
	anew=false;
      }
    }
    entries=entries->next;
  }
  i+=1;
  out[i]=NULL;
  entriesMutex.unlock();
  return out;
}

