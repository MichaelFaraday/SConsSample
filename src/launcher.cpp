#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // execl

#include <string.h> // strrchr

#include <limits.h> // PATH_MAX

int main()
{
  const char proc[] = "/proc/self/exe";

  // get executable file path
  char linkname[PATH_MAX];
  ssize_t r = readlink(proc, linkname, PATH_MAX);
  if (r == -1)
  {
    perror("readlink");
    exit(EXIT_FAILURE);
  }
  else if (r >= PATH_MAX)
  {
    fprintf(stderr, "The path length exceed PATH_MAX\n");
    exit(EXIT_FAILURE);
  }
  linkname[r] = '\0';

  // change working directory to where executable resides. 
  char* lastSlash = strrchr(linkname, '/');
  if ( lastSlash != NULL )
  {
    if ( lastSlash == linkname )  { linkname[1] = '\0'; }
    else                          { *lastSlash = '\0'; }

    if ( 0 != chdir(linkname) )
    {
      perror("chdir");
      exit(EXIT_FAILURE);
    }
  }

  // exec
  char exeFile[PATH_MAX];
  snprintf(exeFile, PATH_MAX, "./%s", PROG_NAME_MAIN);  // defined in SConscript
  if ( execl(exeFile, exeFile, NULL ) == -1 )
  {
    perror("execl");
  }

  exit(EXIT_SUCCESS);
}
