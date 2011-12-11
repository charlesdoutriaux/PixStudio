#include <pixstudio.h>
#include <string>

int main(int argc, char **argv) {
  char pathin[NAME_MAX_LENGTH];
  int i,j;
  struct pix_entries all,*iter,**split;
  all.next=NULL;
  all.prev=NULL;
  strcpy(all.entry.name,"");
  if (argc==1) {
    strcpy(pathin,"/Users/doutriaux1/Desktop");
  }
  else {
    strcpy(pathin,argv[1]);
  }
  iter=&all;
  printf("pointer: %p,%p\n",iter,all.next);
  scanDir(pathin,&all,3);
  iter =&all;
  printf("Back we think we have: %i entries\n",entriesLen(iter));
  entriesPrint(&all);
  //iter = entriesget(&all,5);
  //printf("Got: %s \n",iter->entry.name);
  //entriesswap(&all,1,2);
  //printf("---------SWAPPED------------\n");
  //entriesprint(&all);
  printf("SORTING\n");
  //entriespartition(&all,0,13,6);
  entriesQuicksort(&all,0,entriesLen(&all)-1);
  printf("-------SORTED---------\n");
  entriesPrint(&all);
  printf("------ Splitting ------\n");
  split = entriesSplitPerTime(&all,3600);
  printf("----splitted: %i,%p,%p,%p\n",i,split,&split,&split[0]);
  //i=0;
  printf("Counting splits: %p, %p\n",split,&split);
  iter = split[0];
  i=0;
  while(iter!=NULL) {
    printf("Pack: %i, n%s\n",i,iter->entry.name);
    entriesFree(iter);
    //printf("len: %i\n",entriesLen(&split[i]));
    i+=1;
    iter=split[i];
  }
};
