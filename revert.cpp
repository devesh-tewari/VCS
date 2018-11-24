#include <bits/stdc++.h>
#include <unordered_map>
#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include "objects.h"
#include "serialize.h"
using namespace std;


void patch(curr_blob.data,parent_blob.data)
{
	
}


void match_commit (string curr_sha,string parent_sha,string HOME)
{
  Tree curr_tree,parent_tree;
  load_tree(curr_tree, curr_sha, HOME);
  load_tree(parent_tree, parent_sha, HOME);
  for (int i = 0; i < curr_tree.pointer_paths.size(); i++)
  {
    auto itr=find(parent_tree.pointer_paths.begin(),parent_tree.pointer_paths.end(),curr_tree.pointer_paths[i]);
    if(itr != parent_tree.pointer_paths.end())   // old entry in current commit
      {
          int itr_index=itr-parent_tree.pointer_paths.begin();
          string parent_matched_path=*itr;
          string parent_matched_sha=parent_tree.sha1_pointers[itr_index];
          if (curr_tree.type[i] == false)  //if its a blob
          {
                if(curr_tree.mtime[i] != parent_tree.mtime[itr_index])
                {
                  Blob curr_blob,parent_blob;
                  load_blob(curr_blob,curr_tree.sha1_pointers[i],HOME);
                  load_blob(parent_blob,parent_matched_sha,HOME);
                  patch(curr_blob.data,parent_blob.data);
                  parent_blob.data=curr_blob.data;
                  save_blob(parent_blob,HOME);
                }
              
          }
          else  
          {
              match_commit (curr_tree.sha1_pointers[i],parent_matched_sha,HOME);
          }
      }  
  }

}