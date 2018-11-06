#include <openssl/sha.h>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
int main()
{
  string str ="raju";
  char *text;
  text=&str[0];
  unsigned char hash[SHA_DIGEST_LENGTH];

    SHA1((unsigned char *)text, str.size(), hash);
    printf("\"");
    // do some stuff with the hash
    printf("raw: %s \n", hash);

    char sha1string[SHA_DIGEST_LENGTH*2 +1];
    for(int i = 0; i < SHA_DIGEST_LENGTH; ++i)
    {
        sprintf(&sha1string[i*2], "%02x", (unsigned int)hash[i]);
    }
    printf("string: %s \n", sha1string);
}
