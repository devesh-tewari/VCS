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


string patch( string curr_blob_data, string parent_file_patch)
  //curr_blob.data,parent_blob.data)
{
  cout<<"Pathch"<<endl; 
  string path1 = ".vcs/temp/data";
  string path2 = ".vcs/temp/patchfile";
  ofstream file1(path1, std::ios::binary | std::ios::out | std::ios::trunc);
  ofstream file2(path2, std::ios::binary | std::ios::out | std::ios::trunc);
  file1 << curr_blob_data;
  file2 << parent_file_patch;
  file1.close();
  file2.close();
  string command = "bash -c \"patch -i .vcs/temp/data .vcs/temp/patchfile\"";
  cout << command << endl;
  string result;
  result = system (command.c_str ());
  cout<<result<<endl;
  return result;
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

  // ifstream head (".vcs/HEAD");
  // string head_str;
  // ofstream branch_write (head_str, ios::out | ios::trunc);
  // branch_write << parent_sha;
  // branch_write.close();
  // save_commit(cm, HOME);

}

void reset( string destination_sha , string current_sha , string option ,string HOME)
  // string curr_sha, string parent_sha, string HOME , )
{
     cout<<"reset nhi ho rha";

  while(destination_sha != current_sha )
  {
    Commit current_commit;
     cout<<"load nhi ho rha";
    load_commit(current_commit, current_sha, HOME);
    cout<<"load ho rha";
    string parent_sha = current_commit.parent_sha1;
    resetUtil ( current_sha, parent_sha, HOME);
    current_sha = parent_sha;
    // save_commit(current_commit,HOME);
    ifstream head (".vcs/HEAD");
    string head_str;
    getline(head, head_str);
    head.close();
    ofstream branch_write (head_str, ios::out | ios::trunc);
    branch_write << current_sha;
    branch_write.close();
    // save_commit(cm, HOME);
  }
}
