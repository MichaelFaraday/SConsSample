#include "classInStaticLib.h"
#include "classInDynamicLib.h"

#include <cstdio>
#include <iostream>

#include <pthread.h>
#include <string.h>

#if defined _WIN32 || defined _WIN64
#include <windows.h>  // LoadLibrary, FreeLibrary, GetProcAddress
#else
#include <dlfcn.h>    // dlopen, dlclose, dlsym
#endif



typedef int(*SumFunc)(int, int);

void* threadFunc(void*)
{
  std::cout << "threadFunc" << std::endl;

  return NULL;
}

int main(int argc, char* argv[])
{
  std::cout << "main" << std::endl;

  // case : link to static library created in project
  ClassInStaticLib cisl;
  cisl.print();

  // case : link to dynamic library created in project
  ClassInDynamicLib cidl;
  cidl.print();

  // case: link to system library ( dynamic )
  pthread_t tid;
  int ret = pthread_create(&tid, NULL, threadFunc, NULL);
  if ( 0 == ret )
  {
    pthread_join(tid, NULL);
  }
  else
  {
    std::cerr << "pthread_create() failed : " << strerror(ret) << std::endl;
  }

  // case: load dynamic library in runtime ( like "LoadLibrary()" in Windows )
#if defined _WIN32 || defined _WIN64
  HMODULE handle = LoadLibraryA("./dso/dynamicLib2.dll");
#else
  void* handle = dlopen("./dso/libdynamicLib2.so", RTLD_LAZY);
#endif
  if ( handle == NULL )
  {
    std::cerr << "dlopen() failed." << std::endl;
  }
  else
  {
#if defined _WIN32 || defined _WIN64
    SumFunc func = reinterpret_cast<SumFunc>(GetProcAddress(handle, "mySum"));
#else
    SumFunc func = reinterpret_cast<SumFunc>(dlsym(handle, "mySum"));
#endif
    if(func)
    {
      const int a = 1, b = 2;
      printf("mySum(%d, %d) = %d\n", a, b, func(a,b));
    }
    else
    {
      std::cerr << "dlsym()/GetProcAddress() failed." << std::endl;
    }

#if defined _WIN32 || defined _WIN64
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
  }

  return 0;
}
