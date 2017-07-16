#include <stdio.h>      /* printf */
#include <stdlib.h>     /* getenv */

int main ()
{
  char* pPath;
  pPath = getenv ("ESCDELAY");
  if (pPath!=NULL)
    printf ("variable is: %s",pPath);
  return 0;
}
