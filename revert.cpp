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
#include "commit.h"
using namespace std;


string patch1( string curr_blob_data, string parent_file_patch)
{
  cout<<"Patch"<<endl;
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


void delete_tree_files_of_reverted_commit(string curr_sha, string parent_sha, string latest_sha, string HOME)
{
  Tree curr_tree,parent_tree,latest_tree;
  load_tree(latest_tree, latest_sha, HOME);
  load_tree(curr_tree, curr_sha, HOME);
  load_tree(parent_tree, parent_sha, HOME);
  for (int i = 0; i < curr_tree.pointer_paths.size(); i++)
  {
    auto itr=find(parent_tree.pointer_paths.begin(),parent_tree.pointer_paths.end(),curr_tree.pointer_paths[i]);
    if(itr != parent_tree.pointer_paths.end())   // old entry in current commit
      {
        auto it=find(latest_tree.pointer_paths.begin(),latest_tree.pointer_paths.end(),curr_tree.pointer_paths[i]);
          if(it == latest_tree.pointer_paths.end())   // old entry in child commit
          {
          }
          else
          {
          int itr_index=itr-parent_tree.pointer_paths.begin();
          string parent_matched_path=*itr;
          string parent_matched_sha=parent_tree.sha1_pointers[itr_index];
          if (curr_tree.type[i] == false)  //if its a blob
          {
                if(curr_tree.mtime[i] != parent_tree.mtime[itr_index])
                {

                }

          }
          else
          {
              delete_tree_files_of_reverted_commit (curr_tree.sha1_pointers[i],parent_matched_sha,latest_sha,HOME);
          }
      }
      }
    else
      {
          auto it=find(latest_tree.pointer_paths.begin(),latest_tree.pointer_paths.end(),curr_tree.pointer_paths[i]);
          if(it != latest_tree.pointer_paths.end())   // old entry in child commit
          {
              int it_index=it-latest_tree.pointer_paths.begin();
              string latest_matched_path=*it;
              string latest_matched_sha=latest_tree.sha1_pointers[it_index];
              if (curr_tree.type[i] == false)  //if its a blob
              {
                  if(curr_tree.mtime[i] == latest_tree.mtime[it_index])
                  {
                    string path;
                    path=HOME + "/../" + curr_tree.pointer_paths[i];
                    cout<<path<<endl;
                    delinit(path);
                  }
              }
          }
      }
  }
}

void RevertUtil (string curr_sha,string parent_sha,string HOME)
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
                if(curr_tree.mtime[i] != parent_tree.mtime[itr_index]
                && !parent_tree.is_binary_file[itr_index] )
                {
                  string file_to_patch_path,output;
                  file_to_patch_path = "../" + curr_tree.pointer_paths[i];
                  ifstream myfile (file_to_patch_path);
                  cout<<file_to_patch_path<<endl;
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
                  cout<<"file data  :"<<data<<endl;
                  Blob parent_blob;
                  load_blob(parent_blob,parent_matched_sha,HOME);
                  output = patch(data,parent_blob.data);
                  ofstream writefile(file_to_patch_path,ios::out|ios::trunc);
                  writefile << output;
                  writefile.close();
                }

          }
          else
          {
              RevertUtil (curr_tree.sha1_pointers[i],parent_matched_sha,HOME);
          }
      }
    else
      {
          string path;
          path=HOME + "/../" + curr_tree.pointer_paths[i];
          cout<<path<<endl;
          delinit(path);
      }
  }
}

// void revertUtil (string curr_sha,string parent_sha,string HOME)
// {
//   Tree curr_tree,parent_tree;
//   load_tree(curr_tree, curr_sha, HOME);
//   load_tree(parent_tree, parent_sha, HOME);
//   for (int i = 0; i < curr_tree.pointer_paths.size(); i++)
//   {
//     auto itr=find(parent_tree.pointer_paths.begin(),parent_tree.pointer_paths.end(),curr_tree.pointer_paths[i]);
//     if(itr != parent_tree.pointer_paths.end())   // old entry in current commit
//       {
//           int itr_index=itr-parent_tree.pointer_paths.begin();
//           string parent_matched_path=*itr;
//           string parent_matched_sha=parent_tree.sha1_pointers[itr_index];
//           if (curr_tree.type[i] == false)  //if its a blob
//           {
//                 if(curr_tree.mtime[i] != parent_tree.mtime[itr_index])
//                 {
//                   Blob curr_blob,parent_blob;
//                   load_blob(curr_blob,curr_tree.sha1_pointers[i],HOME);
//                   load_blob(parent_blob,parent_matched_sha,HOME);
//                   cout<<"patch se pehle current data\n"<<curr_blob.data<<endl<<parent_blob.data<<endl;
//                   curr_blob.data = patch1(curr_blob.data,parent_blob.data);
//                   cout<<"patch ke baad\n"<<curr_blob.data<<endl;
//                   save_blob(curr_blob,HOME);
//                 }

//           }
//           else
//           {
//               revertUtil (curr_tree.sha1_pointers[i],parent_matched_sha,HOME);
//           }
//       }

//   }


// }
void revert( string destination_commit_sha, string current_commit_sha, string HOME)
{
    if ( !match_commit_and_cwd(HOME) )
    {
      cout << "Working tree and commit do not match. Commit changes first\n";
      exit (1);
    }

    string latest_commit_sha=current_commit_sha;
    Commit destination_commit;
    load_commit(destination_commit, destination_commit_sha, HOME);
    string destination_tree_sha=destination_commit.tree_sha1;
    string destination_parent_sha = destination_commit.parent_sha1;
    string child_commit_sha;
  while(current_commit_sha != destination_parent_sha )
  {
    Commit current_commit,parent_commit;
    load_commit(current_commit, current_commit_sha, HOME);
    string current_parent_sha = current_commit.parent_sha1;
    child_commit_sha = current_commit_sha;
    current_commit_sha = current_parent_sha;
  }
  Commit curr_commit,dest_commit,child_commit;
  load_commit(curr_commit,latest_commit_sha, HOME);
  string current_tree_sha=curr_commit.tree_sha1;
  load_commit(dest_commit,current_commit_sha, HOME);
  string destination_parent_tree_sha=dest_commit.tree_sha1;
  load_commit(child_commit,child_commit_sha, HOME);
  string child_tree_sha=child_commit.tree_sha1;
  RevertUtil(current_tree_sha,destination_parent_tree_sha,HOME);


  // create_tree_files_into_wd(current_tree_sha, HOME);
  // delete_tree_files_of_reverted_commit(destination_tree_sha,destination_parent_tree_sha,current_tree_sha,HOME);

}
