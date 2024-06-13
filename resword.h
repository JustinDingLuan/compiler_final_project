/********************** resword.h ******************/
#include <stdlib.h>
#define RESWORDMAX 13
char reswords[RESWORDMAX][10] =
    {
        "{", "CALL", "CONST", "DO", "END", "if", "PROCEDURE",
        "PROGRAM", "scanf", "THEN", "VAR", "while", "printf"};
int isResword(char *s)
{
  int i;
  for (i = 0; i < RESWORDMAX; i++)
  {
    if (strcmp(s, reswords[i]) == 0)
      return i;
  }
  return -1;
}
