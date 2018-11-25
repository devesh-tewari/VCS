#include <string>
#include <iostream>
#include "objects.h"
#include "commit.h"
#include "serialize.h"
using namespace std;

void merge_files(Tree&, Tree&, Tree&, Tree&, string);
void add_files_not_in_t1(Tree&, Tree&, Tree&, string);

bool get_common_ancestor(Commit& ca, Commit& c1,Commit& c2)  //return true for fast forward merge
{
  unordered_map <string, int> m;
  m[c1.sha1]=1;
  string temp_parent_sha=c1.parent_sha1;
  while(temp_parent_sha.compare(""))
  {
    Commit temp_cm;
    load_commit(temp_cm, temp_parent_sha, HOME);
    m[temp_cm.sha1]=1;
    temp_parent_sha=temp_cm.parent_sha1;
  }

  temp_parent_sha=c2.sha1;
  while(temp_parent_sha.compare(""))
  {
    if(m[temp_parent_sha])
    {
      load_commit(ca, temp_parent_sha, HOME);
      break;
    }
    Commit temp_cm;
    load_commit(temp_cm, temp_parent_sha, HOME);
    temp_parent_sha=temp_cm.parent_sha1;
  }

  if(ca.sha1==c1.sha1 || ca.sha1==c2.sha1)
    return true;
  return false;
}

void bash_merge(Blob& cur, Blob& other, Blob& ca, Blob& new_blob)
{
  
}

void merge(string other_branch, string HOME)
{
  ifstream head (".vcs/HEAD");
  string head_str;
  getline(head, head_str);
  head.close();

  string cur_latest_commit;
  ifstream cur_branch (head_str);
  getline(cur_branch, cur_latest_commit);
  cur_branch.close();

  string other_latest_commit = ".vcs/refs/" + other_branch;
  ifstream othr_branch (head_str);
  getline(othr_branch, other_latest_commit);
  othr_branch.close();

  Commit cm1;
  load_commit(cm1, cur_latest_commit, HOME);

  Commit cm2;
  load_commit(cm2, other_latest_commit, HOME);

  cur_latest_commit = cm1.tree_sha1;
  other_latest_commit = cm2.tree_sha1;

  Tree t1, t2;
  load_tree(t1, cur_latest_commit, HOME);
  load_tree(t2, other_latest_commit, HOME);

  Commit common_ancestor;
  get_common_ancestor(common_ancestor, cm1, cm2);

  Tree t3;
  load_tree(t3, common_ancestor.tree_sha1, HOME);

  Tree t4;  // tree for new commit

  merge_files(t1, t2, t3, t4, HOME);

  add_files_not_in_t1(t1, t2, t4, HOME);

  merge_commit(cm1, cm2, common_ancestor, t4, HOME);
}

void merge_files(Tree& t1, Tree& t2, Tree& t3, Tree& t4, string HOME)
{
  t4.name = t1.name;
  t4.type_and_permissions = t1.type_and_permissions;

  for (int i = 0; i < t1.pointer_paths.size(); i++)
  {
    t4.type.push_back(t1.type[i]);
    t4.pointer_paths.push_back(t1.pointer_paths[i]);
    t4.pointer_perm.push_back(t1.pointer_perm[i]);

    auto itr2 = find(t2.pointer_paths.begin(), t2.pointer_paths.end(), t1.pointer_paths[i]);
    auto itr3 = find(t3.pointer_paths.begin(), t3.pointer_paths.end(), t1.pointer_paths[i]);
    if(itr2 != t2.pointer_paths.end() && itr3 != t3.pointer_paths.end())   // file present everywhere
      {
          int itr2_index = itr2 - t2.pointer_paths.begin();
          int itr3_index = itr3 - t3.pointer_paths.begin();
          //string parent_matched_path = *itr;
          //string parent_matched_sha = t2.sha1_pointers[itr_index];
          string matched2_sha = t2.sha1_pointers[itr2_index];
          string matched3_sha = t3.sha1_pointers[itr3_index];

          if (t1.type[i] == false)  //if its a blob
          {
                if(t1.mtime[i] != t3.mtime[itr2_index] && t2.mtime[i] != t3.mtime[itr3_index])
                {
                  Blob b1, b2, b3, b4;
                  load_blob(b1 ,t1.sha1_pointers[i], HOME);
                  load_blob(b2 ,matched2_sha, HOME);
                  load_blob(b3 ,matched3_sha, HOME);

                  bash_merge(b1, b2, b3, b4);

                  get_blob_sha1(b4);
                  t4.sha1_pointers.push_back( b4.sha1 );
                  //cout << delta << endl;
                  save_blob(b4 ,HOME);
                }
                else if(t2.mtime[itr2_index] != t3.mtime[itr3_index])
                {
                  t4.sha1_pointers.push_back( matched2_sha );
                }
                else
                {
                  t4.sha1_pointers.push_back( t1.sha1_pointers[i] );
                }
          }
          else
          {
              Tree t1_inner;
              Tree t2_inner;
              Tree t3_inner;
              Tree t4_inner;

              load_tree(t1_inner, t1.sha1_pointers[i], HOME);
              load_tree(t2_inner, matched2_sha, HOME);
              load_tree(t3_inner, matched3_sha, HOME);

              merge_files (t1, t2, t3, t4_inner, HOME);

              t4.sha1_pointers.push_back( t4_inner.sha1 );
          }
      }

    else   // file not present in CA or second branch (no merging)
      {
          t4.sha1_pointers.push_back( t1.sha1_pointers[i] );
      }
  }

  t4.sha1 = "";
  for(int i = 0; i < t4.pointer_paths.size(); i++)
  {
    t4.sha1 += t4.pointer_perm[i];

    if(t4.type[i] == false)
      t4.sha1 += "blob ";
    else
      t4.sha1 += "tree ";

    t4.sha1 += t4.sha1_pointers[i] + " ";
    t4.sha1 += t4.pointer_paths[i] + "\n";
  }

  t4.sha1 = get_string_sha1(t4.sha1);

  save_tree(t4, HOME);
}


void add_files_not_in_t1(Tree& t1, Tree& t2, Tree& t4, string HOME)
{
  t4.name = t1.name;
  t4.type_and_permissions = t1.type_and_permissions;

  for(int i = 0; i < t2.pointer_paths.size(); i++)
  {
    auto itr = find(t1.pointer_paths.begin(), t1.pointer_paths.end(), t2.pointer_paths[i]);
    int itr_index = itr - t1.pointer_paths.begin();
    string matched_sha = t1.sha1_pointers[itr_index];

    if(itr == t1.pointer_paths.end())   // file present in t2 but not in t1
      {
          t4.type.push_back(t1.type[i]);
          t4.pointer_paths.push_back(t1.pointer_paths[i]);
          t4.pointer_perm.push_back(t1.pointer_perm[i]);

          int itr_index = itr - t1.pointer_paths.begin();
          //string parent_matched_path = *itr;
          //string parent_matched_sha = t2.sha1_pointers[itr_index];
          //string matched_sha = t2.sha1_pointers[itr2_index];
          //string matched3_sha = t3.sha1_pointers[itr3_index];

          if (t1.type[i] == false)  //if its a blob
          {
              t4.sha1_pointers.push_back(t2.sha1_pointers[i]);
          }
          else
          {
              Tree t1_inner;
              Tree t2_inner;
              Tree t4_inner;

              load_tree(t1_inner, matched_sha, HOME);
              load_tree(t2_inner, t2.sha1_pointers[i], HOME);

              add_files_not_in_t1 (t1, t2, t4_inner, HOME);

              t4.sha1_pointers.push_back( t4_inner.sha1 );
          }
      }
  }

  t4.sha1 = "";
  for(int i = 0; i < t4.pointer_paths.size(); i++)
  {
    t4.sha1 += t4.pointer_perm[i];

    if(t4.type[i] == false)
      t4.sha1 += "blob ";
    else
      t4.sha1 += "tree ";

    t4.sha1 += t4.sha1_pointers[i] + " ";
    t4.sha1 += t4.pointer_paths[i] + "\n";
  }

  t4.sha1 = get_string_sha1(t4.sha1);

  save_tree(t4, HOME);
}
