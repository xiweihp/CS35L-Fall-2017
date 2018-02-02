/*
 Xiwei Ma
 LAB 5
*/

#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <ctype.h>

/*compare frobnicated arrays*/
int frobcmp(char const *a, char const *b)
{
  for(;; a++) /*Loop the entire words*/
    {
      if(*a == ' ' && *b == ' ')/*it they hit the end at the same time*/
	return 0;
      /*compare char or any one of them hit the end*/
      /*00101010 is 42*/
      else if (*a == ' ' || ((*a^42) < (*b^42)))
	return -1;
      else if (*b == ' ' || ((*a^42) > (*b^42)))
	return 1;
      b++;
    }
}

/*coordinate the pointer type between qsort and forbcmp*/
int cmp(const void* a1,const void* a2)
{
  const char* arr_a = *(char **)a1;
  const char* arr_b = *(char **)a2;
  return frobcmp(arr_a, arr_b);
}

/*compare frobnicated arrays when with f flag*/
int ffrobcmp(char const *a, char const *b)
{
  /*decypher the characters first*/
  for(;; a++) /*Loop the entire words*/
    {
      char char_a = (*a^42);
      char char_b = (*b^42);
        
      /*change to upper case*/
      if(char_a >= 0 && char_a <= UCHAR_MAX)
        /*should be either EOF or a nonnegative value that is at most UCHAR_MAX*/
	char_a = toupper(char_a);
      if(char_b >= 0 && char_b <= UCHAR_MAX)
	char_b = toupper(char_b);
        
      if(*a == ' ' && *b == ' ')/*they hit the end at the same time*/
	return 0;
      /*compare char or any one of them hit the end*/
      /*00101010 is 42*/
      else if (*a == ' ' || (char_a < char_b))
	return -1;
      else if (*b == ' ' || (char_a > char_b))
	return 1;
      b++;
    }
}

/*coordinate the pointer type between qsort and fforbcmp*/
int fcmp(const void* a1,const void* a2)
{
  const char* arr_a = *(char **)a1;
  const char* arr_b = *(char **)a2;
  return ffrobcmp(arr_a, arr_b);
}


void ReadError(ssize_t input)
{
  if(input < 0)
    {
      fprintf(stderr, "Read input error.");
      exit(1);
    }
  return;
}

void allocError(void *ptr)
{
  if (ptr == NULL)
    {
      fprintf(stderr, "Memory allocation error.");
      free(ptr);
      exit(1);
    }
  return;
}

int main(int argc, char* argv[])
{
  if (argc > 2){
    fprintf(stderr, "Error, too many arguments passed in");
    exit(1);
  }
    
  int flag = 0;
  if(argc == 2 && argv[1][1] == 'f' && argv[1][0] == '-')
    flag = 1;
    
    
  struct stat fileStat;
  /*use the fstat to determine information
    about a file based on its file descriptor*/
  if(fstat(STDIN_FILENO,&fileStat) < 0)
    {
      fprintf(stderr, "IO Error with fstat");
      exit(1);
    }
  char *allWord;
  char **words;
  off_t fSize;
  int wordsNum = 0;
  /* should allocate enough memory at once */
  if(S_ISREG(fileStat.st_mode))
    /* if it is regular file */
    {
      fSize = fileStat.st_size;
      allWord = (char*)malloc(fSize * sizeof(char));
      allocError(allWord);
      int countWords = 0;
      if(read(STDIN_FILENO, allWord, fSize)>0)
        {
	  /*count the words that need to be allocated*/
            size_t i;
            for(i=0; i < fSize; i++)
            {
                if(i == fSize-1)
                {
                    countWords++;
                }
                if(allWord[i] == ' ' && i != fSize-1)
                {
                    /*in case of duplicate space*/
                    if(allWord[i+1]==' ')
                        continue;
                    countWords++;
                }
            }
	  words = (char**)malloc(countWords * sizeof(char*));
	  allocError(words);
	  wordsNum = countWords;
        }
    }
  else
    {
      words = (char**)malloc(sizeof(char*));
      allocError(words);
      wordsNum = 1;
    }
    
  char *word = NULL;
  size_t AllPos = 0;
  size_t charPos = 0;
  size_t wordPos = 0;
  int count = 0;
  int size = 0;
  char curr;
  char next;
  ssize_t check1;
  ssize_t check2;
  check1 = read(STDIN_FILENO,&curr , 1);
  ReadError(check1);
  while(curr== ' ')
    {
      check1 = read(STDIN_FILENO,&curr , 1);
      ReadError(check1);
    }
  check2 = read(STDIN_FILENO, &next, 1);
  ReadError(check2);
    
  while(check1 > 0)
    {
      if (S_ISREG(fileStat.st_mode) && word == NULL)/*regular file*/
        {
	  if(curr == ' ')
            {
	      curr = next;
	      check1 = check2;
	      check2 = read(STDIN_FILENO, &next, 1);
	      ReadError(check2);
	      continue;
            }
	  count = 0;
	  size_t i = AllPos;
	  while(i< fSize && allWord[i] != ' ')
            {
	      count ++;
	      i++;
            }
	  word = (char*)malloc((count + 1) * sizeof(char));
	  allocError(word);
	  size = count + 1;
        }
        
      if((!S_ISREG(fileStat.st_mode)) && word == NULL)
        {
	  word = (char *) malloc(sizeof(char));
	  size = 1;
        }
        
      word[charPos] = curr;
      if(size < (charPos+2))
        {
	  size=charPos+2;
	  word = realloc(word, sizeof(char)*(size));
	  allocError(word);
        }
        
      if((curr != ' ') && (check2 > 0))/*not the end of this word*/
        {
	  curr = next;
	  check1 = check2;
	  check2 = read(STDIN_FILENO, &next, 1);
	  ReadError(check2);
	  charPos += 1;
	  AllPos++;
	  continue;
        }
        
      if(curr == ' ')/*end of this word*/
        {
            
	  if(charPos==0)
	    continue;
	  words[wordPos] = word;
	  if(wordsNum < (wordPos+2))
            {
	      wordsNum=wordPos+2;
	      words = realloc(words, sizeof(char*)*wordsNum);
	      allocError(words);
            }
	  wordPos++;
	  word = NULL;
	  charPos = 0;
	  AllPos++;
	  if(check2==0)/*hit the end of input*/
            {
	      break;
            }
	  if(next == ' ')/*extra whitespace*/
            {
	      while(curr == ' '&& (check1>0))
                {
		  check1 = read(STDIN_FILENO, &curr, 1);
		  AllPos++;
                }
	      check2 = read(STDIN_FILENO, &next, 1);
	      ReadError(check2);

	      AllPos++;
	      continue;
            }
	  curr = next;
	  check1 = check2;
	  check2 = read(STDIN_FILENO, &next, 1);
	  ReadError(check2);
        }
        
      if(check2 == 0)
	/*your program should behave as if a space were appended to the input.*/
        {
	  curr = ' ';
	  charPos++;
	  AllPos++;
	  continue;
        }
        
    }
  if(flag)
    qsort(words, wordPos, sizeof(char*), fcmp);
  else
    qsort(words, wordPos, sizeof(char*), cmp);
    
  //output the result
  size_t i;
  for(i=0; i<wordPos; i++)
    {
      char* temp = words[i];
      size_t j = 0;
      int countLetter = 0;
      while(temp[j] != ' ')
        {
	  countLetter++;
	  j++;
        }
      ssize_t checkOutput = write(STDOUT_FILENO,words[i], countLetter);
      if(checkOutput<=0)
        {
	  fprintf(stderr, "Writing error.");
	  size_t n;
	  for(n = 0; n < wordPos; n++)
	    {
	      free(words[n]);
	    }
	  free(words);
	  free(allWord);
	  exit(1);
        }
      char space = ' ';
      if(write(STDOUT_FILENO, &space, 1)<=0)
        {
	  fprintf(stderr, "Writing error.");
	  size_t n;
	  for(n = 0; n < wordPos; n++)
	    {
	      free(words[n]);
	    }
	  free(words);
	  free(allWord);
	  exit(1);
        }
    }
    
  /*destrcut the space allocated*/
  size_t num;
  for(num = 0; num < wordPos; num++)
    {
      free(words[num]);
    }
  free(words);
  if(S_ISREG(fileStat.st_mode))
    {
      free(allWord);
    }
    
  exit(0);
 
}
