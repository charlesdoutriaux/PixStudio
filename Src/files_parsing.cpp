#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <pixstudio.h>
#include <sys/stat.h>

#include <libexif/exif-mnote-data.h>
#include <libexif/exif-data.h>
#include <libexif/exif-ifd.h>
#include <libexif/exif-utils.h>
#include <libexif/exif-loader.h>
#include <libexif/exif-log.h>
#include <libexif/exif-content.h>

#include <time.h>


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
int isOkFromExt(char *name) {
  int n,i,m;
#define NDTYPES 10
  char types[NDTYPES][5] = { "jpg","jpeg","JPG","JPEG","3gp","3GP","mp4","MP4","avi","AVI"};
  n=strlen(name);
  for(i=0;i<NDTYPES;i++) {
    m=strlen(types[i]);
    if (strcmp(&name[n-m],types[i])==0) return 0;
  }
  return 1;
}

time_t readTime(ExifEntry *entry) {
  char asciitime[NAME_MAX_LENGTH];
  struct tm *tm;
  time_t tt;
  int yr,mon,day,hr,min,s;
  time(&tt);
  tm = localtime(&tt);
  strcpy(asciitime,(char *)entry->data);
  //printf("DATE: %s\n",asciitime);
  sscanf(asciitime,"%4d:%2d:%2d %2d:%2d:%2d",&yr,&mon,&day,&hr,&min,&s);
  //printf("read: %i, %i, %i, %i, %i, %i,\n",yr,mon,day,hr,min,s);
  tm->tm_year=yr-1900;
  tm->tm_mon=mon-1;
  tm->tm_mday=day;
  tm->tm_hour=hr;
  tm->tm_min=min;
  tm->tm_sec=s;
  tt = mktime(tm);
  //printf("DATE2: %s\n",asctime(tm));
  return tt;
}
float readAngle(ExifEntry *entry){
  float sec;
 ExifRational r;
 ExifByteOrder o;
 o = exif_data_get_byte_order (entry->parent->parent);
 sec=0;
 // First actual seconds
 r = exif_get_rational (entry->data + 8 * 2, o);
 sec = (float)r.numerator/(float)r.denominator;
 // Minutes
 r = exif_get_rational (entry->data + 8 * 1, o);
 sec += ((float)r.numerator/(float)r.denominator)*60.;
 //Actual degrees
 r = exif_get_rational (entry->data , o);
 return (float)r.numerator/(float)r.denominator + sec/3600.;
}

void _scanDir(char *path, struct pix_entries *entries,unsigned int sub,unsigned int maxsub) {
  DIR *diri,*dirtmp;
 struct dirent *dire;
 FILE *afile=NULL;
 struct stat buf;
 ExifData *dExif;
 ExifEntry *eExif;
 int err;
 unsigned int i,j;
 char fnm[NAME_MAX_LENGTH];
 struct pix_entries *pixEntry;
 int nentries;

 //printf("Lookinga at : %s\n",path);
 nentries = 0;
 diri = opendir(path);
 if (diri==NULL) {
   // Ok something bad happened
   return;
 }
 pixEntry = entries;
 while (pixEntry->next!=NULL) {
   pixEntry=pixEntry->next;
   nentries+=1;
 }
 //printf("ok we skipped: %i\n",nentries);
 nentries=0;
 while ((dire = readdir(diri))!=NULL) {
   strcpy(fnm,path);
   strcat(fnm,"/");
   //printf("name: %s\n",dire->d_name);
   strcat(fnm,dire->d_name);
   err = stat(fnm,&buf);
   if (err==-1) continue;
   // Directory?
   if (S_ISDIR(buf.st_mode)) {
     if ((strcmp(&fnm[strlen(fnm)-2],"/.")==0)||(strcmp(&fnm[strlen(fnm)-3],"/..")==0)) {
       continue;
     };
     if ((sub<maxsub)||(maxsub<0)) _scanDir(fnm,entries,sub+1,maxsub);
     continue;
   }
   // Ok now trying to load the exif info
   dExif = exif_data_new_from_file(fnm);
   if ((isOkFromExt(dire->d_name)==0)||(dExif!=NULL)) {
     //Ok it's a valid file, add an entry, go to last entry
     if (strcmp(pixEntry->entry.name,"")!=0) { /* already full need to createa new one */
       if (pixEntry->next!=NULL) {
	 printf("we have a huge problem weare in the middle of something !\n");
       }
       pixEntry->next = (struct pix_entries *)malloc(sizeof(struct pix_entries));
       entriesInit(pixEntry->next);
       pixEntry->next->prev=pixEntry;
       pixEntry=pixEntry->next;
     }
     //printf("FILE: %s\n",dire->d_name);
     strcpy(pixEntry->entry.name,fnm);
     pixEntry->entry.time=buf.st_mtime; // by default time of last modif
     // Ok we have exif
     if (dExif!=NULL) {
       //printf("FOUND EXIF\n");
       pixEntry->entry.hasExif=true;
       //eExif = exif_data_get_entry(dExif,EXIF_TAG_GPS_INFO_IFD_POINTER);
       //if (eExif!=NULL) printf("LAT TAG: %p, %i (%i), f:%i\n",eExif,eExif->tag,EXIF_TAG_GPS_LATITUDE,eExif->format);
       //for (i=0;i<EXIF_IFD_COUNT;i++) {
       //printf("%i: %s\n",i,exif_ifd_get_name(i));
       	 //cExif = dExif->ifd[i];
       	 for (j=0;j<dExif->ifd[EXIF_IFD_GPS]->count;j++) {
	   eExif = dExif->ifd[EXIF_IFD_GPS]->entries[j];
      	   if (eExif->tag == EXIF_TAG_GPS_LATITUDE) {
	     //exif_data_dump(dExif);
	     pixEntry->entry.lat=readAngle(eExif);
	     // ??? code for negative latitudes?  Need to Test
	     for (i=0;i<dExif->ifd[EXIF_IFD_GPS]->count;i++) {
	       eExif = dExif->ifd[EXIF_IFD_GPS]->entries[i];
	       if (eExif->tag==EXIF_TAG_GPS_LATITUDE_REF) {
		 if (strcmp((const char*)eExif->data,"S")==0) pixEntry->entry.lat=-pixEntry->entry.lat;
	       }
	     }
	   }
	   else if (eExif->tag == EXIF_TAG_GPS_LONGITUDE) {
	     pixEntry->entry.lon=readAngle(eExif);
	   }
	 }
	 for (j=0;j<dExif->ifd[EXIF_IFD_EXIF]->count;j++) {
	   eExif = dExif->ifd[EXIF_IFD_EXIF]->entries[j];
	   if (eExif->tag==EXIF_TAG_DATE_TIME_ORIGINAL) {
	     pixEntry->entry.time = readTime(eExif);
	   }
	 }
	 exif_data_free(dExif);
     }
     nentries+=1;
   };
 }
 //printf("Closing: %s\n",path);
 closedir(diri);
//printf("Discovered: %i valid files\n",nentries);
}
void scanDir(char *path, struct pix_entries *entries,unsigned int maxsub) {
  return _scanDir(path,entries,0,maxsub);
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
  int i,ipivot,inewpivot;
  struct pix_entries *tmp;
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

  // Sadly it seems we need to first loop once to figure out the number of packs and then malloc this, seems inefficient
  i=1;
  current=entries;
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
  return out;
}

