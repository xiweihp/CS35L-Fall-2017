/*
 Xiwei Ma
 LAB5: tr2u.c
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char * argv[])
{
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
    
  char curr;
  while(read(STDIN_FILENO, &curr, 1) > 0)
    /*return a non-negative integer if no error*/
    {
      int a;
      for(a = 0; a < len1; a ++)
        {
	  if(from[a]==curr)
            {
	      curr = to[a];
            }
        }
      int check = write(STDOUT_FILENO, &curr, 1);
      if(check < 0)
        {
	  fprintf(stderr, "Error when writing");
	  exit(1);
        }
    }
    return 0;
    
}
