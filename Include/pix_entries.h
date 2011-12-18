#include <time.h>
//#include <Qt>
//#include <QtGui>
#ifndef PIX_ENTRIES_H
#define PIX_ENTRIES_H
#define NAME_MAX_LENGTH 2048
struct pix_entry {
  //char original_md5[50];
  char name[NAME_MAX_LENGTH];
  float lat;
  float lon;
  time_t time;
  bool hasExif;
};

struct pix_entries {
  struct pix_entries *prev;
  struct pix_entry entry;
  struct pix_entries *next;
};
extern int entriesLen(struct pix_entries *entries);
extern void entriesPrint(struct pix_entries *entries);
extern void entriesQuicksort(struct pix_entries *entries,int left, int right);
extern struct pix_entries ** entriesSplitPerTime(struct pix_entries *entries, int delta);
extern void entriesFree(struct pix_entries *out);
extern void entriesInit(struct pix_entries *pix);

#endif
