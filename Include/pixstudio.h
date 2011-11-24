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

struct pix_entries {
  struct pix_entry entry;
  struct pix_entries *next;
};

