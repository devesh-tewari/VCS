#include "objects.h"
#include <iostream>
#include <string>
using namespace std;

string get_blob_sha1(Blob bl, string path)
{
  // get sha1 for :
    //  filename filesize nullcharacter content(bin)
}

string get_tree_sha1(Tree tr, string path)
{
  // get sha1 for :
    //  dirname dirsize nullcharacter content(bin)
}

int set_type_and_permissions(string type, int permissions)
{
  int type_and_permissions = 511 & permissions;  /* Extracting nine permission
                                                    bits */
  if(type == "blob")
    type_and_permissions = (1<<15) | type_and_permissions;
  else if(type == "tree")
    type_and_permissions = (1<<14) | type_and_permissions;
  else if(type == "commit")
    type_and_permissions = (1<<13) | type_and_permissions;
}

void add(string source)
{
  struct stat srt;
  stat(&source[0], &srt);
  string obj_type;

  if(S_ISDIR(srt.st_mode))    //check if folder, if so build tree
  {
    int k = source.find_last_of("/");
    string dir_name = source.substr(k+1, source.size()-1);

    const char* dir_path_c = &source[0];

    int type_and_permissions = set_time_and_permissions("tree", srt.st_mode);

    //Tree tr = new Tree(dir_name, type_and_permissions);

    DIR *dr = opendir(dir_path_c);

    if (dr == NULL)      // opendir returns NULL if couldn't open directory
    {
      printf("Could not open directory " + d_name);
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
        add(files[i]);
      }
    }

    //tr.sha1 = get_tree_sha1(tr);
    //return tr.sha1;
  }
  else   //if its a file, build blob
  {
    int k = source.find_last_of("/");
    string file_name = source.substr(k+1, source.size()-1);

    int type_and_permissions = set_time_and_permissions("blob", srt.st_mode);

    Blob bl = new Blob(file_name, type_and_permissions);

    bl.sha1 = get_blob_sha1(bl);
    return bl.sha1;
  }
}

int main()
{
  add(".");
}
