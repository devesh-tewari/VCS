#include "init.h"
#include "objects.h"
#include "status.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char** argv)
{
  if(argc == 0)
    return 0;

  char H[PATH_MAX];
  getcwd(H, PATH_MAX);
  string HOME(H);
  //cout<<HOME<<endl;
//cout << argc << endl;
  if(strcmp(argv[1], "init") == 0)
  {
    if(argc > 2)
    {
      init(argv[2]);
    }
    else
    {
      init(HOME);
    }
    return 0;
  }

  int i;
  if(strcmp(argv[1], "add") == 0)
  {
    struct stat st;
    string vcs_folder = HOME + "/.vcs";
    //cout << vcs_folder;
    stat(&vcs_folder[0], &st);
    if( !(S_ISDIR(st.st_mode)) )
    {
        cout << "fatal: not a vcs repository: .vcs" << endl;
        return 0;
    }
    if(argc > 2)
    {
      vector<string> v;
      for(i = 2; i < argc; i++)
        v.push_back(argv[i]);
      add(v, HOME);
    }
    return 0;
  }

  if(strcmp(argv[1], "status") == 0)
  {
    struct stat st;
    string vcs_folder = HOME + "/.vcs";
    //cout << vcs_folder;
    stat(&vcs_folder[0], &st);
    if( !(S_ISDIR(st.st_mode)) )
    {
        cout << "fatal: not a vcs repository: .vcs" << endl;
        return 0;
    }

    status(HOME);

    return 0;
  }

  return 0;
}
