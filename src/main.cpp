#include "classInStaticLib.h"
#include "classInDynamicLib.h"

#include <iostream>

#include <pthread.h>
#include <string.h>

#include <dlfcn.h>

typedef int(*SumFunc)(int, int);

void* threadFunc(void*)
{
  pthread_t tid = pthread_self();
  std::cout << "threadFunc : id=" << tid << std::endl;

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
  void* handle = dlopen("./dso/libdynamicLib2.so", RTLD_LAZY);
  if ( handle == NULL )
  {
    std::cerr << "dlopen() failed." << std::endl;
  }
  else
  {
    SumFunc func = reinterpret_cast<SumFunc>(dlsym(handle, "mySum"));
    if(func)
    {
      const int a = 1, b = 2;
      printf("mySum(%d, %d) = %d\n", a, b, func(a,b));
    }
    else
    {
      std::cerr << "dlsym() failed." << std::endl;
    }

    dlclose(handle);
  }

  return 0;
}
