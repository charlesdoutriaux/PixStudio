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
void scanDir(char *path, struct pix_entries *entries) {
 DIR *diri;
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

 nentries = 0;
 diri = opendir(path);
 pixEntry = entries;
 while (pixEntry->next!=NULL) {
   pixEntry=pixEntry->next;
 }
while ((dire = readdir(diri))!=NULL) {
   strcpy(fnm,path);
   strcat(fnm,"/");
   strcat(fnm,dire->d_name);
   err = stat(fnm,&buf);
   if (err==-1) continue;
   // Directory?
   if (S_ISDIR(buf.st_mode)) {
     //printf("\tskipping subdirectory\n");
     continue;
   }
   // Ok now trying to load the exif info
   dExif = exif_data_new_from_file(fnm);
   if ((isOkFromExt(dire->d_name)==0)||(dExif!=NULL)) {
     //Ok it's a valid file, add an entry, go to last entry
     if (pixEntry->prev!=NULL) { /* already full need to createa new one */
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
  printf("Discovered: %i valid files\n",nentries);
}

int main(int argc, char **argv) {
  char pathin[NAME_MAX_LENGTH];
  int i;
  struct pix_entries all,*iter;
  all.next=NULL;
  all.prev=NULL;
  printf("Nargs: %i\n",argc);
  if (argc==1) {
    strcpy(pathin,"/Users/doutriaux1/Desktop");
  }
  else {
    strcpy(pathin,argv[1]);
  }
  scanDir(pathin,&all);
  iter =&all;
  i = 1 ;
  while (iter->next!=NULL) {
    printf("%i: %s--\n",i,iter->entry.name);
    i+=1;
    iter=iter->next;
  }
  printf("%i:%s--\n",i,iter->entry.name);
  printf("Back we think we have: %i entries\n",i);
}

