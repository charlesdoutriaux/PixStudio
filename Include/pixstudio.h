#include <time.h>
#define NAME_MAX_LENGTH 256
struct pix_entry {
  int original_md5;
  char name[NAME_MAX_LENGTH];
  float lat;
  float lon;
  time_t time;
  int hasExif;
};
