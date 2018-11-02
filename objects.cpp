#include "objects.h"

using namespace std;

int copyfile(string source,string destination)
{
  int k = source.find_last_of("/");
  string f_name=source.substr(k+1,source.size()-1);
  string f_path = destination+"/"+f_name;
  std::ifstream src(&source[0], std::ios::binary);
  std::ofstream dst(&f_path[0], std::ios::binary);
  dst << src.rdbuf();
  return 0;
}

int copy(string source, string destination)

void add()
{
  struct stat srt;
  stat(&source[0], &srt);
  if(S_ISDIR(srt.st_mode))                                    //check if folder
  {
    int k = source.find_last_of("/");
    string d_name=source.substr(k+1,source.size()-1);
    string dest_path = destination+"/"+d_name;
    mkdir(&dest_path[0], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);//create folder at destination
    struct dirent *de;
    const char* dir_path_c =&source[0];


    Tree tr = new Tree(d_name, sha1, type_and_permissions, );

    DIR *dr = opendir(dir_path_c);
    if (dr == NULL)                                             // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        cout << source<<"lag gayi";
        return 0;
    }

    struct stat st;
    int no_of_files=0;
	  vector<string> files;
    vector<string> files_wo_path;
    while ((de = readdir(dr)) != NULL)
    {
      string file_path=source +"/" + (de->d_name);
      const char* file_path_c=&file_path[0];
		  if(stat(file_path_c, &st) != 0)
		  {
        cout<<"b\n";
        return 0;
    	}
	    files.push_back(file_path_c);
	    files_wo_path.push_back(de->d_name);
	    no_of_files++;
    }

    for(int i=0;i<no_of_files;i++)
    {
      if(files_wo_path[i].compare("..") && files_wo_path[i].compare("."))
      {
               //cout << files[i];
        copy(files[i],dest_path);
      }
    }
  }
  else
  {
    copyfile(source,destination);
  }
}
