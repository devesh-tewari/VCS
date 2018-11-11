#include "objects.h"
#include "status.h"
#include "serialize.h"
#include <unordered_map>
#include <vector>
#include <iostream>
#include <sys/stat.h>
#include <dirent.h>

void check_untracked_files(unordered_map<string, bool> &paths, string source)
{
  struct stat srt;
  stat(&source[0], &srt);
  string obj_type;

  if(S_ISDIR(srt.st_mode))    //check if folder, if so build tree
  {
    int k = source.find_last_of("/");
    string dir_name = source.substr(k+1, source.size()-1);

    const char* dir_path_c = &source[0];

    //int type_and_permissions = set_time_and_permissions("tree", srt.st_mode);

    //Tree tr = new Tree(dir_name, type_and_permissions);

    DIR *dr = opendir(dir_path_c);
    struct dirent *de;

    if (dr == NULL)      // opendir returns NULL if couldn't open directory
    {
      cout << "Could not open directory " << dir_name;
      return;
    }

    struct stat st;
    int no_of_files=0;
	  vector<string> files;
    vector<string> files_wo_path;
    while ((de = readdir(dr)) != NULL)
    {
      string file_path = source + "/" + (de->d_name);
      const char* file_path_c = &file_path[0];
		  if(stat(file_path_c, &st) != 0)
		  {
        cout<<"File does not exist!\n";
        return;
    	}
	    files.push_back(file_path_c);
	    files_wo_path.push_back(de->d_name);
	    no_of_files++;
    }

    for(int i=0; i < no_of_files; i++)
    {
      if(files_wo_path[i].compare("..") && files_wo_path[i].compare(".")
         && files_wo_path[i].compare(".vcs"))
      {
        check_untracked_files(paths, files[i]);
      }
    }

    //tr.sha1 = get_tree_sha1(tr);
    //return tr.sha1;
  }
  else   //if its a file, build blob
  {
    if(paths.find(source) == paths.end())
    {
      cout << "Untracked File: " << source << endl;
    }
  }
}

void status(string HOME)
{
  int i;
  Index INDEX;
  struct stat st;
  string index_path = HOME + "/INDEX";

  bool add = false;
  bool commit = false;
  if(stat(&HOME[0], &st) == 0)
  {
    add = true;
    INDEX = load_index(HOME);
  }

  /* check for commit from HEAD */

  if(!add && !commit)
  {
    //print all files as untracked
  }

  if(add && !commit)
    {
      string file_path;
      unordered_map<string, bool> paths;
      for(i = 0; i < INDEX.entries.size(); i++)
        {
          file_path = HOME + "/" + INDEX.entries[i].path;
          struct stat st;
          if(stat(&file_path[0], &st) == 0)
            {
              if((unsigned long)st.st_mtime == INDEX.entries[i].mtime)
                {
                  continue;
                }
              else
                {
                  cout << "Modified file: " << INDEX.entries[i].path << endl;
                }
            }
          else
            {
              cout << "Deleted file: " << INDEX.entries[i].path << endl;
            }
          paths.insert( make_pair(file_path, true) );
        }

      check_untracked_files(paths, HOME);
    }
}
