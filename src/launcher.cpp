#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // execl

#include <string.h> // strrchr

#include <limits.h> // PATH_MAX


#if defined _WIN32 || defined _WIN64
#include <windows.h>
static const char PATH_SEP = '\\';
#else
static const char PATH_SEP = '/';
#endif


int main()
{
  char absPath[PATH_MAX];

#if defined _WIN32 || defined _WIN64
  DWORD r = GetModuleFileNameA(NULL, absPath, PATH_MAX);
  if( r == 0 )
  {
    fprintf(stderr, "GetModuleFileName failed with 0x%x\n", GetLastError());
    exit(EXIT_FAILURE);
  }
  else if( r == PATH_MAX )
  {
    fprintf(stderr, "The path length exceed PATH_MAX\n");
    exit(EXIT_FAILURE);
  }
#else
  // get executable file path
  ssize_t r = readlink("/proc/self/exe", absPath, PATH_MAX);
  if ( r == -1 )
  {
    perror("readlink");
    exit(EXIT_FAILURE);
  }
  else if (r >= PATH_MAX)
  {
    fprintf(stderr, "The path length exceed PATH_MAX\n");
    exit(EXIT_FAILURE);
  }
#endif
  absPath[r] = '\0';

  // change working directory to where executable resides. 
  char* lastSlash = strrchr(absPath, PATH_SEP);
  if ( lastSlash != NULL )
  {
    if ( lastSlash == absPath )  { absPath[1] = '\0'; }
    else                         { *lastSlash = '\0'; }

    if ( 0 != chdir(absPath) )
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
