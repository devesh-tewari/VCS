#include <unistd.h>
#include <limits.h>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include "objects.h"
#include "init.h"
using namespace std;

void init(string HOME)
{
  chdir(&HOME[0]);

  if (mkdir("./.vcs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
  {
      cout<<"Cannot initialize VCS repository. Directory is already VCS initialized.\n";
      return;
  }

  chdir("./.vcs");

  ofstream config ("config");
  ofstream HEAD ("HEAD");
  config.close();
  HEAD.close();

  mkdir("objects", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  mkdir("refs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  chdir("..");

  cout<<"Initialized empty VCS repository.\n";
}
//
// int main()
// {
//   init();
//   return 0;
// }
