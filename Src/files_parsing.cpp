#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <files_parsing.h>
#include <sys/stat.h>

/*
#include <libexif/exif-mnote-data.h>
#include <libexif/exif-data.h>
#include <libexif/exif-ifd.h>
#include <libexif/exif-utils.h>
#include <libexif/exif-loader.h>
#include <libexif/exif-log.h>
#include <libexif/exif-content.h>
*/
#include <time.h>
#include <QExifImageHeader.h>


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

int isMovieFromExt(char *name) {
  int n,i,m;
#define NMDTYPES 6
  char types[NMDTYPES][5] = { "3gp","3GP","mp4","MP4","avi","AVI"};
  n=strlen(name);
  for(i=0;i<NMDTYPES;i++) {
    m=strlen(types[i]);
    if (strcmp(&name[n-m],types[i])==0) return 0;
  }
  return 1;
}

time_t readTime(QExifValue entry) {
  char asciitime[NAME_MAX_LENGTH];
  struct tm *tm;
  time_t tt;
  int yr,mon,day,hr,min,s;
  time(&tt);
  tm = localtime(&tt);
  strcpy(asciitime,(char *)entry.toString().toAscii().data());
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
float readAngle(QExifValue entry){
  float sec;
  QVector<QExifURational> vals = entry.toRationalVector();
  sec=0;
  //printf("ok we are getting an entry with: %i\n",entry.count());
 // First actual seconds
  QExifURational r = vals.at(2);
 sec = (float)r.first/(float)r.second;
 // Minutes
 r=vals.at(1);
  sec += ((float)r.first/(float)r.second)*60.;
 //Actual degrees
  r=vals.at(0);
  return (float)r.first/(float)r.second + sec/3600.;
}

void _scanDir(char *path, struct pix_entries *entries,unsigned int sub,int maxsub) {
  DIR *diri;
 struct dirent *dire;
 struct stat buf;
 QExifImageHeader *QExifFile;
 int err;
 char fnm[NAME_MAX_LENGTH];
 struct pix_entries *pixEntry;
 int nentries;

 fprintf(stderr,"Looking at : %s\n",path);
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
   //fprintf(stderr,"\tname: %s\n",dire->d_name);
   strcat(fnm,dire->d_name);
   err = stat(fnm,&buf);
   if (err==-1) continue;
   // Directory?
   if (S_ISDIR(buf.st_mode)) {
     if ((strcmp(&fnm[strlen(fnm)-2],"/.")==0)||(strcmp(&fnm[strlen(fnm)-3],"/..")==0)) {
       continue;
     };
     if (((int)sub<maxsub)||(maxsub<0)) _scanDir(fnm,entries,sub+1,maxsub);
     continue;
   }
   // Ok now trying to load the exif info
   QExifFile = new QExifImageHeader();
   //printf("Ok testing: %s, %i\n",fnm,isMovieFromExt(fnm));
   bool okExif;
   if (isMovieFromExt(fnm)!=0) {
     okExif = QExifFile->loadFromJpeg(fnm);
   } 
   else {
     okExif = false;
   }
   if ((isOkFromExt(dire->d_name)==0)||(okExif)) {
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
     if (okExif) {
       //printf("FOUND EXIF\n");
       pixEntry->entry.hasExif=true;
       if (QExifFile->contains(QExifImageHeader::GpsLatitude)) {
	 pixEntry->entry.lat = readAngle(QExifFile->value(QExifImageHeader::GpsLatitude));
       }
       if (QExifFile->contains(QExifImageHeader::GpsLongitude)) {
	 pixEntry->entry.lon = readAngle(QExifFile->value(QExifImageHeader::GpsLongitude));
       }
       if (QExifFile->contains(QExifImageHeader::DateTime)) {
	 pixEntry->entry.time = readTime(QExifFile->value(QExifImageHeader::DateTime));
       }
     }
     nentries+=1;
   };
 }
 //printf("Closing: %s\n",path);
 closedir(diri);
//printf("Discovered: %i valid files\n",nentries);
}
void scanDir(char *path, struct pix_entries *entries, int maxsub) {
  return _scanDir(path,entries,0,maxsub);
}

