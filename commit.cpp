#include <bits/stdc++.h>
#include <unordered_map>
#include <openssl/sha.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "objects.h"
#include "serialize.h"
using namespace std;

void add_to_map(string ,string ,IndexEntry& ,int );


bool sortinrev(const pair<int,string> &a,
               const pair<int,string> &b)
{
       return (a.first > b.first);
}

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

unordered_map<string, string> umap;
vector< pair <int,string> > depth;


string build_tree(Index &INDEX, string HOME)
{
    /*a[0]="a/1.txt";
    a[1]="a/2.txt";
    a[2]="a/b/3.txt";
    a[3]="a/b/4.txt";
    a[4]="a/b/c/5.txt";
    a[5]="a/b/d/6.txt";
    a[6]="a/e/f/g/h/7.txt";

    sha[0]="a/1.txt";
    sha[1]="a/2.txt";
    sha[2]="a/b/3.txt";
    sha[3]="a/b/4.txt";
    sha[4]="a/b/c/5.txt";
    sha[5]="a/b/d/6.txt";
    sha[6]="a/e/f/g/h/7.txt";*/

    //Blob bl[ INDEX.entries.size() ];
    int k = HOME.find_last_of("/");
    string home_folder = HOME.substr(k + 1, HOME.size() - 1);

    int i;
    for(i = 0; i < INDEX.entries.size(); i++)
    {
        //load_blob(bl[i], INDEX.entries[i].sha1, HOME);

        string temp = home_folder + "/" + INDEX.entries[i].path;
        k = temp.find_first_of("/");
        string dir_name = temp.substr(0, k);
        string remaining_part = temp.substr(k+1,temp.size()-k);
        //cout<<remaining_part<<"\n";
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

     /*for (auto itr = umap.begin(); itr != umap.end(); itr++)
     {
        cout << itr->first << "  " << itr->second << endl;
     }*/

     sort(depth.begin(), depth.end(), sortinrev);

     string ret_val;
     for(i = 0; i < depth.size(); i++)
     {
        //cout << "here" << endl;
        Tree tr;
        //cout << depth[0].first << " " << depth[0].second << endl;
        string tree_obj_data = umap[ depth[i].second ];
        tree_obj_data = tree_obj_data.substr(5, tree_obj_data.size() - 5);  //dummy removed
        //cout << tree_obj_data << "\n";

        tr.sha1 = get_tree_sha1(tree_obj_data);
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

        //cout << depth[i].second << endl;
        //cout << tr.name << " " << oct << tr.type_and_permissions << endl;

        string line;
        istringstream iss(tree_obj_data);
        while (getline(iss, line))
        {
            istringstream iss2(line);
            vector<string> results(istream_iterator<string>{iss2},
                                   istream_iterator<string>());
            tr.pointer_perm.push_back( results[0] );
            tr.sha1_pointers.push_back( results[1] );
            tr.pointer_paths.push_back( results[2] );
        }

        cout << tree_obj_data << endl;
        cout << endl;
        /*while(token != NULL)
        {
          s = token;
          tr.pointer_perm.push_back( s );
          token = strtok(NULL, " ");

          s = token;
          tr.sha1_pointers.push_back( s );
          token = strtok(NULL, " ");

          s = token;
          tr.pointer_paths.push_back( s );
          token = strtok(NULL, " ");
        }*/

        //create file in object folder
        /*std::ofstream outfile ("objects/" + tree_obj_data_sha);
        outfile << tree_obj_data << std::endl;
        outfile.close();*/

        k = depth[i].second.find_last_of("/");
        string parent = depth[i].second.substr(0, k);
        umap[parent] = umap[parent]
                       + to_string(tr.type_and_permissions) + " "
                       + tr.sha1 + " " + depth[i].second
                       + "\n";

        //depth.erase(depth.begin());
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
                            + entry.sha1 + " " + prev_path + "/" + current_path
                            + "\n";
      }
}


void commit(string HOME)
{
  Index INDEX;
  struct stat st;
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

  Commit cm;
  cm.tree_sha1 = build_tree(INDEX, HOME);
  tm* ctm = localtime(&cm.timestamp);
  cout << asctime(ctm);
  //cout << tree_sha;
}
