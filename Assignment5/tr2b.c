/*
 Xiwei Ma
 LAB 5: tr2b.c
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, const char * argv[]) {
  if(argc != 3)/* operand errors must be diagnosed */
    {
      fprintf(stderr, "Wrong number of arguments");
      exit(1);
    }
  /* create byte string from and to */
  const char *from = argv[1];
  const char *to = argv[2];
    
  /* should report an error if from and to are not the same length
     or some operand is empty */
  size_t len1 = strlen(from);
  size_t len2 = strlen(to);
  if(len1==0 || len2 ==0)
    {
      fprintf(stderr, "Empty operands");
      exit(1);
    }
  if(len1 != len2)
    {
      fprintf(stderr, "Not the same length");
      exit(1);
    }
    
  /* report an error if from has duplicate bytes*/
  int i;
  for (i = 0; i< len1; i++) {
    int j;
    for(j=i+1; j < len1; j++)
      {
	if(from[i] == from[j])
	  {
	    fprintf(stderr, "From has duplicate btyes");
	    exit(1);
	  }
      }
  }
    
  /* use getchar()*/
  char curr = getchar();
  while(curr != EOF)
    {
      if(ferror(stdin))
        {
	  fprintf(stderr, "Error while reading file");
	  exit(1);
        }
      size_t a;
      int put = 0;/*indicate whether current byte already processed or not*/
      for(a = 0; a < len1; a ++)
        {
	  if(from[a]==curr)
            {
	      putchar(to[a]);
	      put = 1;
            }
        }
      if(put == 0)/*if not yet been put*/
        {
	  putchar(curr);
        }
      if(ferror(stdout))
        {
	  fprintf(stderr, "Error while writing file");
	  exit(1);
        }
      curr = getchar();
    }
  return 0;
}
