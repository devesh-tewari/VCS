#include "objects.h"
#include "init.h"
#include "add.h"
#include "status.h"
#include "commit.h"
#include "reset.h"
#include "revert.h"
#include "objects.h"
#include "serialize.h"
#include "vcsdiff.h"
#include "merge.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
  if(argc == 1)
    return 0;

  char H[PATH_MAX];
  getcwd(H, PATH_MAX);
  string HOME = "";
  HOME = H;

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
//cout<<endl<<HOME<<endl;
    status(HOME);

    return 0;
  }

  if(strcmp(argv[1], "commit") == 0)
  {
//cout<<endl<<HOME<<endl;
    if(argc == 4 && strcmp(argv[2], "-m") == 0)
        {
          string message= argv[3];
          commit(HOME,message);
        }
        else{
          cout << "Nothing specified, nothing committed." << endl;
          cout << "Maybe you wanted to say '  vcs commit -m 'commit message'  '?" << endl;
        }
    return 0;
  }

  if(strcmp(argv[1], "branch") == 0)
  {
    string branch;

    if (argc < 3)
    {
      ifstream head (".vcs/HEAD");
      getline(head, branch);
      int k = branch.find_last_of("/");
      branch = branch.substr(k + 1, branch.size() - k - 1);
      head.close();
      cout << branch << endl;
      return 0;
    }

    ofstream head (".vcs/HEAD", std::ios::trunc | std::ios::out);
    string br = argv[2];
    string new_ref = ".vcs/refs/" + br;
    head << new_ref;
    head.close();

    return 0;
  }

  if(strcmp(argv[1], "diff") == 0)
  {

    if (argc < 3) // simple vcs diff to compare stagging area(index) and cwd
    {
        vcsdiff(HOME);
    }

    else
    {
      if(strcmp(argv[2], "--cached") == 0) // compare stagging area(index) and latest commit
        {
            vcsdiff_cached(HOME);
        }
      if(strcmp(argv[2], "--no-index") == 0)  // compare 2 files in cwd
        {
            if(argc == 5)
            {
                vcsdiff(HOME,argv[3],argv[4]);
            }
        }
    }

    return 0;
  }

  if(strcmp(argv[1], "reset") == 0)
  {
    if (argc == 2)  // clear staging area
    {
      struct stat st;
      string index_path = HOME + "/.vcs/INDEX";
      if(stat(&index_path[0], &st) == 0)
      {
        if( remove(&index_path[0]) != 0 )
          perror( "Error deleting file" );
      }
      return 0;
    }

    string destination_sha = argv[3];
    cout<<"Reached in VCS.cpp \n";

    ifstream head (".vcs/HEAD");
    string current_branch,current_sha;
    getline(head, current_branch);
    head.close();

    ifstream branch_read (current_branch);
    getline(branch_read, current_sha);
    branch_read.close();

    string option = "";
    cout<<current_sha<<endl<<HOME<<endl<<destination_sha<<endl;

    reset( destination_sha , current_sha , option , HOME);
    // devi_reset( destination_sha , current_sha ,  HOME);
  }

  if(strcmp(argv[1], "revert") == 0)
  {

    
    string destination_sha = argv[2];
    cout<<"Reached in VCS.cpp \n";

    ifstream head (".vcs/HEAD");
    string current_branch,current_sha;
    getline(head, current_branch);
    head.close();

    ifstream branch_read (current_branch);
    getline(branch_read , current_sha);
    branch_read.close();

    cout<<current_sha<<endl<<HOME<<endl<<destination_sha<<endl;
    Commit destination_commit;
    load_commit(destination_commit, destination_sha, HOME);
    string destination_parent_sha = destination_commit.parent_sha1;
    if (destination_parent_sha=="")
      {
        cout<<"Cannot revert 1st commit\n";
        return 1;
      }
    ofstream file(".vcs/temp/t.txt");
    string command = "bash -c \"vcs add .vcs/temp/t.txt\"";
    cout << command << endl;
    system (command.c_str ());
    commit(HOME,"revert commit");
    revert( destination_sha , current_sha ,  HOME);
    remove(".vcs/temp/t.txt");
  }


  if(strcmp(argv[1], "checkout") == 0 || (strcmp(argv[1], "-b") == 0 && strcmp(argv[2], "checkout") == 0))
  {
    string branch_path (argv[ argc-1 ]);
    branch_path = ".vcs/refs/" + branch_path;

    if (argc == 3) // check if branch exists
    {
      struct stat st;
      if(stat(&branch_path[0], &st) != 0)
      {
        cout << "No such branch: " << branch_path << endl;
        return 0;
      }
    }

    ifstream head (".vcs/HEAD");
    string head_str;
    getline (head, head_str);
    head.close ();

    if (argc == 4)
    {
      ifstream branch_read (head_str);
      string current_sha;
      getline(branch_read, current_sha);
      branch_read.close ();

      ofstream new_branch (branch_path, ios::out);
      new_branch << current_sha;
      new_branch.close ();
    }

    ofstream head_write (".vcs/HEAD", ios::out | ios::trunc);
    head_write << branch_path;
    head_write.close ();
  }

  if(strcmp(argv[1], "merge") == 0)
  {
    string other_branch (argv[ argc-1 ]);
    string branch_path = ".vcs/refs/" + other_branch;

    struct stat st;
    if(stat(&branch_path[0], &st) != 0)
    {
      cout << "No such branch: " << branch_path << endl;
      return 0;
    }

    merge (other_branch, HOME);
  }

  return 0;
}
