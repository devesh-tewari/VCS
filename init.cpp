#include <unistd.h>
#include <limits.h>
#include <iostream>
#include <sys/stat.h>
#include <fstream>
using namespace std;

void init()
{
  //char cwd[PATH_MAX];
  //getcwd(cwd, sizeof(cwd));

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

  cout<<"Initialized empty VCS repository/n";
}

int main()
{
  init();
  return 0;
}
