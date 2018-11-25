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
#include "revert.h"
using namespace std;


string patch( string curr_blob_data, string parent_file_patch)
  //curr_blob.data,parent_blob.data)
{
  cout<<"Pathch"<<endl; 
  string path1 = ".vcs/temp/data";
  string path2 = ".vcs/temp/patchfile";
  ofstream file3(path1, std::ios::binary | std::ios::out | std::ios::trunc);
  ofstream file4(path2, std::ios::out | std::ios::trunc);
  file3 << curr_blob_data;
  file4 << parent_file_patch;
  file3.close();
  file4.close();
  string command = "bash -c \"patch -i .vcs/temp/data .vcs/temp/patchfile\"";
  cout << command << endl;
  system (command.c_str ());
  ifstream myfile (".vcs/temp/data");
  string data="";
  string line;
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      data+=line;
    }
    myfile.close();
  }

  else cout << "Unable to open file"; 

  return data;
  
}


void resetUtil (string curr_sha,string parent_sha,string HOME)
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
                  cout<<"patch se pehle"<<curr_blob.data<<endl<<parent_blob.data<<endl;
                  parent_blob.data = patch(curr_blob.data,parent_blob.data);
                  // parent_blob.data=curr_blob.data;
                  // cout<<"mai hu"<<parent_blob.data<<endl;
                  save_blob(parent_blob,HOME);
                }

          }
          else
          {
              resetUtil (curr_tree.sha1_pointers[i],parent_matched_sha,HOME);
          }
      }
  }


}

void reset( string destination_sha , string current_sha , string option ,string HOME)
{
  while(current_sha != destination_sha )
  {
    Commit current_commit,parent_commit;
    load_commit(current_commit, current_sha, HOME);
    string parent_sha = current_commit.parent_sha1;
    string current_tree_sha=current_commit.tree_sha1;
    load_commit(parent_commit, parent_sha, HOME);
    string parent_tree_sha=parent_commit.tree_sha1;
    resetUtil ( current_tree_sha, parent_tree_sha, HOME);
    current_sha = parent_sha;
    ifstream head (".vcs/HEAD");
    string head_str;
    getline(head, head_str);
    head.close();
    ofstream branch_write (head_str, ios::out | ios::trunc);
    branch_write << current_sha;
    branch_write.close();
  }
}
