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
void scanDir(char *path, struct pix_entry *firstEntry, int nmax) {
 DIR *diri;
 struct dirent *dire;
 FILE *afile=NULL;
 struct stat buf;
 ExifLoader *lExif;
 ExifData *dExif;
 ExifContent *cExif;
 ExifEntry *eExif;
 int err;
 unsigned int i,j;
 char fnm[NAME_MAX_LENGTH];
 int goodExif=0;
 int nentries=0;
 struct pix_entry pixEntry;
 //struct pix_group *pixGrp, *iter;
 //lExif = exif_loader_new();
 diri = opendir(path);
 while ((dire = readdir(diri))!=NULL) {
   strcpy(fnm,path);
   strcat(fnm,"/");
   strcat(fnm,dire->d_name);
   err = stat(fnm,&buf);
   //printf("Ok we have: %s, %i\n",fnm,err);
   if (err==-1) continue;
   // Directory?
   if (S_ISDIR(buf.st_mode)) {
     //printf("\tskipping subdirectory\n");
     continue;
   }
   // Ok now trying to load the exif info
   //printf("Ok loading: %s\n",fnm);
   //dExif = exif_loader_write_file(lExif,fnm);
   dExif = exif_data_new_from_file(fnm);
   if ((isOkFromExt(dire->d_name)==0)||(dExif!=NULL)) {
     //Ok it's a valid file, add an entry
     pixEntry = firstEntry[nentries];
     strcpy(pixEntry.name,fnm);
     pixEntry.hasExif=0;
     pixEntry.lat=1.e20;
     pixEntry.lon=1.e20;
     pixEntry.time=buf.st_mtime; // by default time of last modif
     // Ok we have exif
     if (dExif!=NULL) {
       pixEntry.hasExif=1;
       //eExif = exif_data_get_entry(dExif,EXIF_TAG_GPS_INFO_IFD_POINTER);
       //if (eExif!=NULL) printf("LAT TAG: %p, %i (%i), f:%i\n",eExif,eExif->tag,EXIF_TAG_GPS_LATITUDE,eExif->format);
       //for (i=0;i<EXIF_IFD_COUNT;i++) {
       //printf("%i: %s\n",i,exif_ifd_get_name(i));
       	 //cExif = dExif->ifd[i];
       	 for (j=0;j<dExif->ifd[EXIF_IFD_GPS]->count;j++) {
	   eExif = dExif->ifd[EXIF_IFD_GPS]->entries[j];
      	   if (eExif->tag == EXIF_TAG_GPS_LATITUDE) {
	     //exif_data_dump(dExif);
	     pixEntry.lat=readAngle(eExif);
	     // ??? code for negative latitudes?  Need to Test
	     for (i=0;i<dExif->ifd[EXIF_IFD_GPS]->count;i++) {
	       eExif = dExif->ifd[EXIF_IFD_GPS]->entries[i];
	       if (eExif->tag==EXIF_TAG_GPS_LATITUDE_REF) {
		 if (strcmp(eExif->data,"S")==0) pixEntry.lat=-pixEntry.lat;
	       }
	     }
	   }
	   else if (eExif->tag == EXIF_TAG_GPS_LONGITUDE) {
	     pixEntry.lon=readAngle(eExif);
	   }
	 }
	 for (j=0;j<dExif->ifd[EXIF_IFD_EXIF]->count;j++) {
	   eExif = dExif->ifd[EXIF_IFD_EXIF]->entries[j];
	   if (eExif->tag==EXIF_TAG_DATE_TIME_ORIGINAL) {
	     pixEntry.time = readTime(eExif);
	   }
	 }
	 exif_data_free(dExif);
     }
     if (goodExif==0) {
       printf("\t\tINSERT CODE FOR FILE STAT ANALYSIS HERE\n");
     }
   //exif_data_dump(dExif);
     firstEntry[nentries]=pixEntry;
     nentries+=1;
     if (nentries==nmax) {
       printf("We found %i files, this the max, stopping here\n",nmax);
       return;
     };
   };
 }
  printf("Discovered: %i valid files\n",nentries);
}

int main(int argc, char **argv) {
  char pathin[NAME_MAX_LENGTH];
  int i;
  struct pix_entry all[512];
  printf("Nargs: %i\n",argc);
  if (argc==1) {
    strcpy(pathin,"/Users/doutriaux1/Desktop");
  }
  else {
    strcpy(pathin,argv[1]);
  }
  scanDir(pathin,&all[0],512);
}

