#include "objects.h"
#include "serialize.h"
#include <fstream>
#include <streambuf>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <vector>
#include <iostream>
#include <dirent.h>
using namespace std;

vector< pair<string, string> > INDEX;  // pair of path and all stuff (to sort)
string HOME;

void get_blob_sha1(Blob &bl)
{
  // get sha1 for :
    //  filename filesize nullcharacter content(bin)

  //**added by sanket on 6 nov start
    //string str ="raju";
  char *text;
  text = &bl.data[0];
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1((unsigned char *)text, bl.data.size(), hash);
  char sha1string[SHA_DIGEST_LENGTH*2 +1];
  for(int i = 0; i < SHA_DIGEST_LENGTH; ++i)
  {
    sprintf(&sha1string[i*2], "%02x", (unsigned int)hash[i]);
  }
  //printf("string: %s \n", sha1string);
  string sha(sha1string);
  bl.sha1 = sha;
  //cout<<"inside getBlob: "<<sha<<endl<<sha1string<<endl;
  //**added by sanket on 6 nov end
}

/*string get_tree_sha1(Tree tr, string path)
{

}*/

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

  return type_and_permissions;
}

bool check_presence(string path, Index &INDEX)
{
  for(int i = 0; i < INDEX.entries.size(); i++)
  {
    if(INDEX.entries[i].path == path)
      return true;
  }
  return false;
}

void build_index(string source, Index &INDEX)
{//cout<<source<<endl;
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
         && files_wo_path[i].compare(".vcs")
         && files_wo_path[i].compare("INDEX"))
      {
        build_index(files[i], INDEX);
      }
    }

    //tr.sha1 = get_tree_sha1(tr);
    //return tr.sha1;
  }
  else   //if its a file, build blob
  {
    int k = source.find_last_of("/");
    string file_name = source.substr(k+1, source.size()-1);

    //int type_and_permissions = set_time_and_permissions("blob", srt.st_mode);
int type_and_permissions = srt.st_mode;
    Blob bl(file_name, type_and_permissions);
//cout<<"here"<<" ";
    string txt;
    ifstream file(source);
    string str((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    bl.data = str;
    //cout<<"data: "<<bl.data<<"!"<<endl;
//cout<<"here"<<endl;
//cout<<"here"<<" ";
    get_blob_sha1(bl);
//<<<<<<< HEAD
//cout<<"here";
    save_blob(bl, HOME);
//cout<<"here"<<endl;
//=======
//cout<<"here";
    save_blob(bl, HOME);
//cout<<"here"<<endl;
//>>>>>>> 3c4cee69ec1ef18edc5cfc7a5fb14f5e375f33e9
//cout<<"here"<<" ";
    IndexEntry entry;
    entry.type_and_permissions = type_and_permissions;
    entry.sha1 = bl.sha1;
    if(source.substr(0, 2) == "./")
      source = source.substr(2, source.size()-2);
    entry.path = source;
    //string index_entry = bitset<8>(type_and_permissions).to_string() + " ";
//cout<<"here"<<endl;
    //index_entry += bl.sha1 + " ";
    //index_entry += source + "\t";  //save only path from vcs root
    entry.mtime = (unsigned long)srt.st_mtime;
    //unsigned long last_modified = (unsigned long)srt.st_mtime;
    //index_entry += to_string(last_modified); //also store file versions in three places (if required later)

    if( !check_presence(source, INDEX) )
      INDEX.entries.push_back( entry );
  }
}

bool cmp(IndexEntry e1, IndexEntry e2)
{
  if(strcmp( &e1.path[0], &e2.path[0] ) < 0)
    return true;
  return false;
}

void add(vector<string> sources, string home)
{
  HOME = home;
  Index INDEX;
  struct stat st;
  string index_path = HOME + "/.vcs/INDEX";
  if(stat(&index_path[0], &st) == 0)
  {
    load_index(INDEX, HOME);
  }

  string H;
  for(int i = 0; i < sources.size(); i++)
  {
    build_index(sources[i], INDEX);  //modify sources to be from HOME only
  }

  sort(INDEX.entries.begin(), INDEX.entries.end(), cmp);
  // store index.second to INDEX file

  for(int i = 0; i < INDEX.entries.size(); i++)
  {
    IndexEntry e = INDEX.entries[i];
    cout << e.path << "\t" << e.sha1 << " " << oct << e.type_and_permissions << "\t" << e.mtime << endl;
  }

  save_index(INDEX, HOME);
}
