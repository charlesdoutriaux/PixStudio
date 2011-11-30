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
  char asciitime[30];
  struct tm tm;
  time_t tt;
  strcpy(asciitime,entry->data);
  //printf("DATE: %s\n",asciitime);
  sscanf(asciitime,"%4i:%2i:%2i %2i:%2i:%2i",&tm.tm_year,&tm.tm_mon,&tm.tm_mday,&tm.tm_hour,&tm.tm_min,&tm.tm_sec);
  tm.tm_year-=1900;
  tt = mktime(&tm);
  //printf("DATE2: %s\n",asctime(&tm));
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
void scanDir(char *path, struct pix_entries *entries,unsigned int sub) {
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
     if (sub>=1) scanDir(fnm,entries,sub+1);
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
       pixEntry->next = malloc(sizeof(struct pix_entries));
       pixEntry->next->prev=pixEntry;
       pixEntry=pixEntry->next;
       pixEntry->next= NULL;
     }
     strcpy(pixEntry->entry.name,fnm);
     pixEntry->entry.hasExif=0;
     pixEntry->entry.lat=1.e20;
     pixEntry->entry.lon=1.e20;
     pixEntry->entry.time=buf.st_mtime; // by default time of last modif
     // Ok we have exif
     if (dExif!=NULL) {
       pixEntry->entry.hasExif=1;
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
		 if (strcmp(eExif->data,"S")==0) pixEntry->entry.lat=-pixEntry->entry.lat;
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
int entrieslen(struct pix_entries *entries) {
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
    printf("%i: %s--\n",i,entries->entry.name);
    i+=1;
    entries=entries->next;
  }
  printf("%i: %s--\n",i,entries->entry.name);
  return i+1;
}

struct pix_entries *entriesget(struct pix_entries *entries, int index) {
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

void entriesswap(struct pix_entries *entries,int i1,int i2) {
  struct pix_entries *tmp1,*tmp2,*tmp;
  int i3;
  i3 = entrieslen(entries);
  if (i2>=i3) return;
  if (i1<0) return;
  if (i2==i1) return;
  if (i2<i1){
    i3=i1;
    i2=i1;
    i1=3;
  }

  tmp1 = entriesget(entries,i1);
  tmp2 = entriesget(entries,i2);
  // Connect i1-1
  tmp=tmp1->prev;
  if (tmp!=NULL) { // ok it did have a previous elt
    tmp->next=tmp2;
  }
  tmp=tmp1->next;
  tmp->prev=tmp2;
  tmp=tmp2->prev;
  tmp->next = tmp1;
  tmp=tmp2->next;
  if (tmp!=NULL) {
    tmp->prev=tmp1;
  }
  
  tmp=tmp1->prev;
  tmp1->prev=tmp2->prev;
  tmp2->prev=tmp;
  tmp=tmp1->next;
  tmp1->next=tmp2->next;
  tmp2->next=tmp;
}


int main(int argc, char **argv) {
  char pathin[NAME_MAX_LENGTH];
  int i;
  struct pix_entries all,*iter;
  all.next=NULL;
  all.prev=NULL;
  strcpy(all.entry.name,"");
  printf("Nargs: %i, %s\n",argc,all.entry.name);
  if (argc==1) {
    strcpy(pathin,"/Users/doutriaux1/Desktop");
  }
  else {
    strcpy(pathin,argv[1]);
  }
  iter=&all;
  printf("pointer: %p,%p\n",iter,all.next);
  scanDir(pathin,&all,1);
  scanDir(pathin,&all,1);
  iter =&all;
  printf("Back we think we have: %i entries\n",entrieslen(iter));
  iter = entriesget(&all,5);
  printf("Got: %s \n",iter->entry.name);
  entriesswap(&all,1,10);
  printf("------------------------\n");
  printf("Back we think we have: %i entries\n",entrieslen(iter));
}

