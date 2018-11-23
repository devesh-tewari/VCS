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

void add_to_map(string ,string ,IndexEntry& ,int );
void match_commit (string ,string,string);


bool sortinrev(const pair<int,string> &a,const pair<int,string> &b)
{
       return (a.first > b.first);
}

unordered_map<string, string> umap;
vector< pair <int,string> > depth;


string build_tree(Index &INDEX, string HOME)
{
   
    int k = HOME.find_last_of("/");
    string home_folder = HOME.substr(k + 1, HOME.size() - 1);

    int i;
    for(i = 0; i < INDEX.entries.size(); i++)
    {
      
        string temp = home_folder + "/" + INDEX.entries[i].path;
        k = temp.find_first_of("/");
        string dir_name = temp.substr(0, k);
        string remaining_part = temp.substr(k+1,temp.size()-k);
        int temp_depth = 0;
        if(umap[dir_name].compare(""))
        {
            add_to_map(dir_name, remaining_part, INDEX.entries[i], temp_depth);
        }
	      else
        {
            umap[dir_name] = "dummy";
            depth.push_back( make_pair(temp_depth, dir_name) );
            add_to_map(dir_name, remaining_part, INDEX.entries[i], temp_depth);
        }
    }


     sort(depth.begin(), depth.end(), sortinrev);

     string ret_val;
     for(i = 0; i < depth.size(); i++)
     {
        Tree tr;
        string tree_obj_data = umap[ depth[i].second ];
        tree_obj_data = tree_obj_data.substr(5, tree_obj_data.size() - 5);  //dummy removed
       

        tr.sha1 = get_string_sha1(tree_obj_data);
        tr.name = depth[i].second;

        struct stat st;
        // depth[i].second also contains the name of home folder so remove it
        string actual_path = depth[i].second.substr(home_folder.size(),
                                                    depth[i].second.size() - 1);
        if(actual_path[0] == '/')
          actual_path = actual_path.substr(1, actual_path.size() - 1);
        //cout << ":" << actual_path << endl;
        if(actual_path == "")
          actual_path = ".";
        stat(&actual_path[0], &st);
        tr.type_and_permissions = st.st_mode;
        string line;
        istringstream iss(tree_obj_data);
        while (getline(iss, line))
        {
            istringstream iss2(line);
            vector<string> results(istream_iterator<string>{iss2},
                                   istream_iterator<string>());
            tr.pointer_perm.push_back( results[0] );
            tr.sha1_pointers.push_back( results[2] );
            tr.pointer_paths.push_back( results[3] );

            if(results[1] == "blob")
            {
              tr.type.push_back(false);
              long int mtime = atol(results[4].c_str());
              cout << mtime << endl;
              tr.mtime.push_back( (long int)mtime );
              cout << tr.mtime.size() << endl;
            }
            else
            {
              tr.type.push_back(true);
              tr.mtime.push_back( 0 );
            }
        }
        k = depth[i].second.find_last_of("/");
        string parent = depth[i].second.substr(0, k);
        umap[parent] = umap[parent]
                       + to_string(tr.type_and_permissions) + " "
                       + "tree "
                       + tr.sha1 + " " + depth[i].second
                       + "\n";

        save_tree(tr, HOME);

        ret_val = tr.sha1;
     }
     return ret_val;
}


void add_to_map(string prev_path, string current_path, IndexEntry& entry, int cur_depth)
{
      string temp = current_path;
      int k = temp.find_first_of("/");
      if(k > 0)
      {
          string dir_name = temp.substr(0, k);
          string remaining_part = temp.substr(k + 1, temp.size() - k);
          //cout<<remaining_part<<"\n";
          string map_entry = prev_path + "/" + dir_name;
          if(umap[map_entry].compare(""))
          {
              add_to_map(map_entry, remaining_part, entry, cur_depth + 1);
          }
          else
          {
              umap[map_entry] = "dummy";
              depth.push_back( make_pair(cur_depth+1, prev_path + "/" + dir_name) );
              add_to_map(map_entry, remaining_part, entry, cur_depth + 1);
          }
      }
      else
      {
          umap[prev_path] = umap[prev_path]
                            + to_string(entry.type_and_permissions) + " "
                            + "blob "
                            + entry.sha1 + " " + prev_path + "/" + current_path
                            + " " + to_string(entry.mtime) + "\n";
      }
}


void commit(string HOME,string commit_msg)
{
  Index INDEX;
  struct stat st;
  struct passwd *user;

  string index_path = HOME + "/.vcs/INDEX";

  if(stat(&index_path[0], &st) == 0)
  {
    load_index(INDEX, HOME);
  }
  else
  {
    cout << "nothing added to commit" << endl;
    return;
  }
  if(stat(index_path.c_str(), &st ) == 0)
    user = getpwuid(st.st_uid);
  Commit cm;
  cm.tree_sha1 = build_tree(INDEX, HOME);
  tm* cm_time = localtime(&cm.timestamp);
  //cout << asctime(cm_time) << endl;

  ifstream head (".vcs/HEAD");
  string head_str;
  getline(head, head_str);
  head.close();

  ifstream branch_read (head_str);
  getline(branch_read, cm.parent_sha1);
  branch_read.close();
  cm.author=user->pw_name;
  cm.committer=user->pw_name;
  cm.message=commit_msg;
  string commit_sha_str = "tree " + cm.tree_sha1 + "\n"
                          + "parent " + cm.parent_sha1 + "\n"
                          + "author " + cm.author + "\n"
                          + "committer " + cm.committer + "\n"
                          + cm.message + "\n"
                          + asctime(cm_time);

  cout << commit_sha_str << endl;

  cm.sha1 = get_string_sha1 (commit_sha_str);
  ofstream branch_write (head_str, ios::out | ios::trunc);
  branch_write << cm.sha1;
  branch_write.close();
  save_commit(cm, HOME);
  Commit cmparent;
  load_commit(cmparent, cm.parent_sha1,HOME);
  match_commit(cm.tree_sha1,cmparent.tree_sha1,HOME);

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
                  string delta = diff(parent_blob.data,curr_blob.data);
                  parent_blob.data=delta;
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