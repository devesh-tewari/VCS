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
  string HOME = "";
  HOME = H;
  //cout << HOME << endl;
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

  bool isVCSrepo = false;
  while(HOME != "/")
  {
    struct stat st;
    string vcs_folder = HOME + "/.vcs";
    //cout << vcs_folder;
    stat(&vcs_folder[0], &st);
    if( (S_ISDIR(st.st_mode)) )
    {
      isVCSrepo = true;
      break;
    }
    else
    {
      int k = HOME.find_last_of("/");
      path_from_HOME = HOME.substr(k, HOME.size()-1) + path_from_HOME;
      //HOME = HOME.substr(0, k+1);
      //cout << HOME << endl;
      //cout << path_from_HOME << endl;
      chdir("..");
      getcwd(H, PATH_MAX);
      HOME = H;
    }
  }

  if(!isVCSrepo)
    {
        cout << "fatal: not a vcs repository: .vcs" << endl;
        return 0;
    }

  if(path_from_HOME[ path_from_HOME.size() - 1 ] == '/')
    path_from_HOME = path_from_HOME.substr(0, path_from_HOME.size() - 1);
  if(path_from_HOME[ 0 ] == '/')
    path_from_HOME = path_from_HOME.substr(1, path_from_HOME.size() - 1);

  if(strcmp(argv[1], "add") == 0)
    {
      if(argc > 2)
        {
          vector<string> v;
          for(i = 2; i < argc; i++)
            {
              string add_file(argv[i]);
              if(path_from_HOME == "" && add_file == ".") // add . at home
                add_file = ".";
              else if(add_file == ".")  // add . not at home
                add_file = path_from_HOME;//path_from_HOME;
              else if(path_from_HOME == "")  // add abc at home
                add_file = add_file;
              else  // add abc not at home
                add_file = path_from_HOME + "/" + add_file;
cout << "add: " << add_file << endl;
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
cout<<endl<<HOME<<endl;
    status(HOME);

    return 0;
  }

  return 0;
}
