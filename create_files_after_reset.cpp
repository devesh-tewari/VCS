#include <bits/stdc++.h>
#include <unordered_map>
#include <dirent.h>
#include <openssl/sha.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include "objects.h"
#include "serialize.h"
using namespace std;
#define DEFAULT_MODE      S_IRWXU | S_IRGRP |  S_IXGRP | S_IROTH | S_IXOTH

void create_tree_files_into_wd(string tree_sha,string HOME)
{
    Tree curr_tree;
    load_tree(curr_tree, tree_sha, HOME);
    for (int i = 0; i < curr_tree.pointer_paths.size(); i++)
    {
        if (curr_tree.type[i] == false)  //if its a blob
        {
            Blob curr_blob;
            load_blob(curr_blob,curr_tree.sha1_pointers[i],HOME);
            string path=HOME + "/../" + curr_tree.pointer_paths[i];
            ofstream file(path, std::ios::out | std::ios::trunc);
            file << curr_blob.data;
            file.close();
        }
        else
        {
            string dir_path=HOME + "/../" + curr_tree.pointer_paths[i];
            mkdir(&dir_path[0],DEFAULT_MODE);
            create_tree_files_into_wd (curr_tree.sha1_pointers[i],HOME);            
        }
    }
}