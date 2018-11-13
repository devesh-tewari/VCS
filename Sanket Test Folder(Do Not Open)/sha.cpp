#include <openssl/sha.h>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;



string get_tree_sha1(string str)
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


int main()
{
  string str ="raju";
  
  cout<<get_tree_sha1(str);
  return 0;
  
}