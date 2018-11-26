#include <bits/stdc++.h>
#include <unordered_map>
#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <sys/types.h>
#include "objects.h"
#include "serialize.h"
#include "reset.h"
#include "delete.h"
#include "create_files_after_reset.h"
using namespace std;


string patch( string curr_blob_data, string parent_file_patch)
{
  cout<<"Pathch"<<endl; 
  string path1 = ".vcs/temp/data";
  string path2 = ".vcs/temp/patchfile.patch";
  ofstream file3(path1, std::ios::binary | std::ios::out | std::ios::trunc);
  ofstream file4(path2, std::ios::binary | std::ios::out | std::ios::trunc);
  file3 << curr_blob_data;
  file4 << parent_file_patch;
  file3.close();
  file4.close();
  string command = "bash -c \"patch .vcs/temp/data .vcs/temp/patchfile.patch\"";
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
      data+="\n";
    }
    myfile.close();
  }

  else cout << "Unable to open file"; 

  return data;
  
}


void revertUtil (string curr_sha,string parent_sha,string HOME)
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
                  curr_blob.data = patch(curr_blob.data,parent_blob.data);
                  // parent_blob.data=curr_blob.data;
                  cout<<"mai hu"<<curr_blob.data<<endl;
                  save_blob(curr_blob,HOME);
                }

          }
          else
          {
              resetUtil (curr_tree.sha1_pointers[i],parent_matched_sha,HOME);
          }
      }
    // else
    //   {
          
    //       string path;
    //       path=HOME + "/../" + curr_tree.pointer_paths[i];
    //       cout<<path<<endl;
    //       delinit(path);
    //       // string command = "bash -c \"rm -R *itr\"";
    //       // cout << command << endl;
    //       // system (command.c_str ());
    //   }
    
  }


}
void revert( string destination_commit_sha, string current_commit_sha, string HOME)
{
    string latest_commit_sha=current_commit_sha;
    Commit destination_commit;
    load_commit(destination_commit, destination_commit_sha, HOME);
    string destination_parent_sha = destination_commit.parent_sha1;

  while(current_commit_sha != destination_commit_sha )
  {
    Commit current_commit,parent_commit;
    load_commit(current_commit, current_commit_sha, HOME);
    string current_parent_sha = current_commit.parent_sha1;
    current_commit_sha = current_parent_sha;
  }
 
  revertUtil(latest_commit_sha,destination_commit_sha,HOME);

  Commit curr_commit;
  load_commit(curr_commit,latest_commit_sha, HOME);
  string tree_sha=curr_commit.tree_sha1;
  create_tree_files_into_wd(tree_sha, HOME);

}
