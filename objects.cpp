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

string get_string_sha1(string str)
{
    char *text;
    text=&str[0];
    unsigned char hash[SHA_DIGEST_LENGTH];

    SHA1((unsigned char *)text, str.size(), hash);

    char sha1string[SHA_DIGEST_LENGTH*2 +1];
    for(int i = 0; i < SHA_DIGEST_LENGTH; ++i)
    {
        sprintf(&sha1string[i*2], "%02x", (unsigned int)hash[i]);
    }
    return sha1string;
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

  return type_and_permissions;
}
