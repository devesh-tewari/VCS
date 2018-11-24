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

  string init_at = HOME + "/.vcs";

  if (mkdir(&init_at[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1)
  {
      cout << "Cannot initialize VCS repository. ";
      cout << "Directory is already VCS initialized.\n";
      return;
  }

  chdir(&init_at[0]);

  ofstream config ("config");
  ofstream HEAD ("HEAD");
  HEAD << ".vcs/refs/master";
  config.close();
  HEAD.close();

  mkdir("objects", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  mkdir("refs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  mkdir("temp", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

  ofstream f1 ("temp/diff1");
  ofstream f2 ("temp/diff2");
  f1.close ();
  f2.close ();

  chdir("refs");
  ofstream branch ("master");
  branch.close();

  chdir(&HOME[0]);

  cout << "Initialized empty VCS repository.\n";
}
//
// int main()
// {
//   init();
//   return 0;
// }
