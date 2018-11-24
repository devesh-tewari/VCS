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
        cout << "File does not exist!\n";
        return;
    	}
	    files.push_back(file_path_c);
	    files_wo_path.push_back(de->d_name);
	    no_of_files++;
    }

    for(int i=0; i < no_of_files; i++)
    {
      if(files_wo_path[i].compare("..") && files_wo_path[i].compare(".")
         && files_wo_path[i].compare(".vcs")
         && files_wo_path[i].compare(".git"))
      {
        check_untracked_files(paths, files[i]);
      }
    }

    //tr.sha1 = get_tree_sha1(tr);
    //return tr.sha1;
  }
  else
  {
    int k = source.find_last_of("/");
    string file_name = source.substr(k+1, source.size()-1);
    if(file_name.compare("INDEX") && paths.find(source) == paths.end())
    {
      cout << "Untracked File: " << source << endl;
    }
  }
}

void match_tree (Tree tr, string HOME, unordered_map<string, bool>& paths)
{
  for (int i = 0; i < tr.sha1_pointers.size(); i++)
  {
    if (tr.type[i] == false)  //if its a blob
    {
      //cout << tr.pointer_paths[i] << endl;
      struct stat st;
      int k = tr.pointer_paths[i].find_first_of("/");  //paths currently have home folder included
      string path = tr.pointer_paths[i].substr(k+1, tr.pointer_paths[i].size()-1);
      //cout << path << endl;
      if(stat(&path[0], &st) == 0)
      {
        //cout << st.st_mtime << endl;
        //cout << tr.mtime.size();
        //cout << tr.type.size();
        if(tr.mtime[i] != st.st_mtime)
        {
          cout << "Modified: " << tr.pointer_paths[i] << endl;
        }
        //cout << path << endl;
      }
      else
      {
        cout << "Deleted: " << tr.pointer_paths[i] << endl;
      }
      paths.insert( make_pair(path, true) );
    }
    else  //if its a tree
    {
      //cout << endl;
      //cout << tr.name << endl;
      Tree inner;
      load_tree(inner, tr.sha1_pointers[i], HOME);
      match_tree (inner, HOME, paths);
    }
  }
}

void status(string HOME)
{
  int i;
  Index INDEX;
  struct stat st;
  string index_path = HOME + "/.vcs/INDEX";

  bool add = false;
  bool commit = false;
  if(stat(&index_path[0], &st) == 0)
  {
    add = true;
    //cout<<"here"<<endl;
    load_index(INDEX, HOME);
    //cout<<"here"<<endl;
  }

  string last_commit_sha;
  ifstream head (".vcs/HEAD");
  getline(head, last_commit_sha);
  head.close();

  ifstream branch_read (last_commit_sha);
  getline(branch_read, last_commit_sha);
  branch_read.close();

  if(last_commit_sha != "")
  {
    commit = true;
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

  if(add && commit)
    {
      Commit cm;
      load_commit(cm, last_commit_sha, HOME);
      Tree tr;
      load_tree(tr, cm.tree_sha1, HOME);

      unordered_map<string, bool> paths;
      match_tree (tr, HOME, paths);
      check_untracked_files(paths, HOME);
    }
}
