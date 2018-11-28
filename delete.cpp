#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <bits/stdc++.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;
#define DEFAULT_MODE      S_IRWXU | S_IRGRP |  S_IXGRP | S_IROTH | S_IXOTH

bool isDir(const char* p)
{
  struct stat statbuf;
  bool isdir=false;
  if(stat(p,&statbuf)!=-1)
  {
    if(S_ISDIR(statbuf.st_mode))
    {
      isdir=true;
    }
  }
  return isdir;
}


void deldir(string src)
{
  DIR *dp;
  struct dirent *entry;
  struct stat statbuf;
  dp = opendir(&src[0]);
  stat(&src[0],&statbuf);
  while((entry = readdir(dp))!= NULL)
  {
    if(entry->d_type!=4)
    {
      string source = src + "/" + entry->d_name;
      remove(&source[0]);
    }

    else if(entry->d_type==4)
    {
      if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
        continue;
      string sorc=src+"/"+entry->d_name;
      deldir(sorc);
      rmdir(&sorc[0]);                                 //Recursion
    }
  }
}

void delinit(string src)
{
  cout<<"in delete.cpp\n";
  if(isDir(&src[0]))
    {
      deldir(src);
      rmdir(&src[0]);
    }
  else
    remove(&src[0]);
}
