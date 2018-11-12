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
  string path_from_HOME = "";

  while(HOME != "")
  {
    struct stat st;
    string vcs_folder = HOME + "/.vcs";
    //cout << vcs_folder;
    stat(&vcs_folder[0], &st);
    if( (S_ISDIR(st.st_mode)) )
    {
      break;
    }
    else
    {
      int k = HOME.find_last_of("/");
      path_from_HOME = HOME.substr(k+1, HOME.size()-1) + "/" + path_from_HOME;
      HOME = HOME.substr(0, k);
      //cout << HOME << endl;
      //cout << path_from_HOME << endl;
      chdir("..");
    }
  }
  if(path_from_HOME[ path_from_HOME.size() - 1 ] == '/')
    path_from_HOME = path_from_HOME.substr(0, path_from_HOME.size() - 1);

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
      {
        string add_file(argv[i]);
        if(add_file == ".")
          add_file = path_from_HOME;
        else
          add_file = path_from_HOME + "/" + add_file;
        //cout << "add: " <<add_file << endl;
        v.push_back(add_file);
      }
      add(v, HOME);
    }
    else
    {
      cout << "Nothing specified, nothing added." << endl;
      cout << "Maybe you wanted to say 'vcs add .'?" << endl;
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
