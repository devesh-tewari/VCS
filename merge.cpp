// #include <string>
// #include <iostream>
// #include <sys/stat.h>
// #include <unistd.h>
// #include "objects.h"
// #include "commit.h"
// #include "serialize.h"
// #include "reset.h"
// using namespace std;
//
// void merge_files(Tree&, Tree&, Tree&, Tree&, string);
// void add_files_not_in_t1(Tree&, Tree&, Tree&, string);
// void update_working_directory_file (string, Blob&);
//
// bool get_common_ancestor(Commit& ca, Commit& c1, Commit& c2, string HOME)  //return true for fast forward merge
// {
//   //cout << c1.sha1 << endl;
//   //cout << c2.sha1 << endl;
//   unordered_map <string, int> m;
//   m[c1.sha1]=1;
//   string temp_parent_sha=c1.parent_sha1;
//   while(temp_parent_sha.compare(""))
//   {
//     Commit temp_cm;
//     load_commit(temp_cm, temp_parent_sha, HOME);
//     m[temp_cm.sha1]=1;
//     temp_parent_sha=temp_cm.parent_sha1;
//   }
//
//   temp_parent_sha=c2.sha1;
//   while(temp_parent_sha.compare(""))
//   {
//     if(m[temp_parent_sha] == 1)
//     {
//       load_commit(ca, temp_parent_sha, HOME);
//       break;
//     }
//     Commit temp_cm;
//     load_commit(temp_cm, temp_parent_sha, HOME);
//     temp_parent_sha=temp_cm.parent_sha1;
//   }
//
//   //cout << ca.sha1 << endl;
//   //cout << c1.sha1 << endl;
//   //cout << c2.sha1 << endl;
//   if(ca.sha1==c1.sha1 || ca.sha1==c2.sha1)
//     return true;
//   return false;
// }
//
// void bash_merge(Blob& cur, Blob& other, string ca_path, Blob& new_blob)
// {
//   string path1 = ".vcs/temp/merge1";
//   string path3 = ".vcs/temp/merge3";
//   string output_path = ".vcs/temp/merge_op";
//
//   ofstream file1(path1, std::ios::binary | std::ios::out | std::ios::trunc);
//   ofstream file3(path3, std::ios::binary | std::ios::out | std::ios::trunc);
//   ofstream file4(output_path, std::ios::binary | std::ios::out | std::ios::trunc);
//
//   file4.close ();
//
//   file1 << cur.data;
//   file3 << other.data;
//
//   file1.close();
//   file3.close();
//
//   string command = "bash -c \"merge -p .vcs/temp/merge1 " + ca_path + " .vcs/temp/merge3 > .vcs/temp/merge_op\"";
//   cout << command << endl;
//   //cout << command << endl;
//   system (command.c_str ());
//
//   ifstream op_file (".vcs/temp/merge_op");
//   string str((istreambuf_iterator<char>(op_file)), istreambuf_iterator<char>());
//   op_file.close ();
//
//   new_blob.data = str;
// }
//
// void merge(string other_branch, string HOME)
// {
//   ifstream head (".vcs/HEAD");
//   string head_str;
//   getline(head, head_str);
//   head.close();
//
//   string cur_latest_commit;
//   ifstream cur_branch (head_str);
//   getline(cur_branch, cur_latest_commit);
//   cur_branch.close();
//
//   string other_latest_commit = ".vcs/refs/" + other_branch;
//   ifstream othr_branch (other_latest_commit);
//   getline(othr_branch, other_latest_commit);
//   othr_branch.close();
//
//   Commit cm1;
//   load_commit(cm1, cur_latest_commit, HOME);
//
//   Commit cm2;
//   load_commit(cm2, other_latest_commit, HOME);
//
//   Commit common_ancestor;
//   if (get_common_ancestor (common_ancestor, cm1, cm2, HOME)) //fast forward merge
//   {
//     if (common_ancestor.sha1 == cm1.sha1)
//     {
//       ofstream cur_write (head_str, ios::out | ios::trunc);
//       cur_write << other_latest_commit;
//       cur_write.close ();
//     }
//     else  // ca == cm2
//     {
//       string othr_path = ".vcs/refs/" + other_branch;
//       ofstream othr_write (othr_path, ios::out | ios::trunc);
//       othr_write << cur_latest_commit;
//       othr_write.close ();
//     }
//
//     cout << "Fast Forward Merge" << endl;
//     cout << common_ancestor.message<<endl;
//     return;
//   }
//   cout << common_ancestor.message << endl;
//   cur_latest_commit = cm1.tree_sha1;
//   other_latest_commit = cm2.tree_sha1;
//
//   Tree t1, t2;
//   load_tree(t1, cur_latest_commit, HOME);
//   load_tree(t2, other_latest_commit, HOME);
//
//   Tree t3;
//   load_tree(t3, common_ancestor.tree_sha1, HOME);
//
//   int k = HOME.find_last_of ("/");
//   string dir_name = HOME.substr (k+1, HOME.size()-1);
//   //cout << dir_name;
//   string command = "bash -c \"mkdir .vcs/temp/" + dir_name + "\nrsync -Rr ./ .vcs/temp/" + dir_name + "\"";
//   system (command.c_str ());
//   //cout << command << endl;
//   common_ancestor_reset(common_ancestor.sha1, cm1.sha1, HOME);
//
//   Tree t4;  // tree for new commit
//
//   //cout << "\n\n\n\n\n\nSIZE\n\n\n";
//   //cout << t1.sha1_pointers.size() << endl;
//   merge_files(t1, t2, t3, t4, HOME);
//
//   add_files_not_in_t1(t1, t2, t4, HOME);
//
//   merge_commit(cm1, cm2, common_ancestor, t4, HOME);
//
//   command = "bash -c \"rm -R .vcs/temp/" + dir_name + "\"";
//   system (command.c_str ());
// }
//
// void merge_files(Tree& t1, Tree& t2, Tree& t3, Tree& t4, string HOME)
// {
//   t4.name = t1.name;
//   t4.type_and_permissions = t1.type_and_permissions;
//
//   for (int i = 0; i < t1.pointer_paths.size(); i++)
//   {
//     t4.type.push_back(t1.type[i]);
//     t4.pointer_paths.push_back(t1.pointer_paths[i]);
//     t4.pointer_perm.push_back(t1.pointer_perm[i]);
//
//     auto itr2 = find(t2.pointer_paths.begin(), t2.pointer_paths.end(), t1.pointer_paths[i]);
//     auto itr3 = find(t3.pointer_paths.begin(), t3.pointer_paths.end(), t1.pointer_paths[i]);
//     if(itr2 != t2.pointer_paths.end() && itr3 != t3.pointer_paths.end())   // file present everywhere
//       {
//           int k = t1.pointer_paths[i].find_first_of ("/");
//           string actual_path = t1.pointer_paths[i].substr (k+1, t1.pointer_paths[i].size()-k);
// //cout << "ACTUAL: " << actual_path <<endl;
//           int itr2_index = itr2 - t2.pointer_paths.begin();
//           int itr3_index = itr3 - t3.pointer_paths.begin();
//           //string parent_matched_path = *itr;
//           //string parent_matched_sha = t2.sha1_pointers[itr_index];
//           string matched2_sha = t2.sha1_pointers[itr2_index];
//           string matched3_sha = t3.sha1_pointers[itr3_index];
//
//           if (t1.type[i] == false)  //if its a blob
//           {
//                 if (t1.mtime[i] != t3.mtime[itr2_index]
//                     && t2.mtime[i] != t3.mtime[itr3_index]) /* If file changed
//                                                               in both branches*/
//                 {
//                   cout << t1.pointer_paths[i] << endl;
//                   cout << t2.pointer_paths[itr2_index] << endl;
//                   cout << t3.pointer_paths[itr3_index] << endl;
//
//                   Blob b1, b2, b4;
//                   load_blob(b1 ,t1.sha1_pointers[i], HOME);
//                   load_blob(b2 ,t2.sha1_pointers[itr2_index], HOME);
//
//                   string ca_file_path = ".vcs/temp/" + t1.pointer_paths[i];
//
//                   bash_merge(b1, b2, ca_file_path, b4);
//
//                   get_blob_sha1(b4);
//                   t4.sha1_pointers.push_back( b4.sha1 );
//                   //cout << delta << endl;
//                   save_blob(b4 ,HOME);
//
//                   update_working_directory_file (actual_path, b4);
//
//                   struct stat st;
//                   string merged_blob_path = ".vcs/objects/" + b4.sha1;
//                   stat(&merged_blob_path[0], &st);
//                   t4.mtime.push_back ((unsigned long)st.st_mtime);
//                 }
//                 // if changed only in other branch
//                 else if(t2.mtime[itr2_index] != t3.mtime[itr3_index])
//                 {
//                   t4.sha1_pointers.push_back( matched2_sha );
//                   t4.mtime.push_back (t2.mtime[itr2_index]);
//
//                   Blob b;
//                   //cout << "load:" << matched2_sha;
//                   load_blob (b, matched2_sha, HOME);
//                   update_working_directory_file (actual_path, b);
//                 }
//                 // if not changed or only changed in current branch
//                 else
//                 {
//                   t4.sha1_pointers.push_back( t1.sha1_pointers[i] );
//                   t4.mtime.push_back (t1.mtime[i]);
//                 }
//           }
//           else  // if it's a tree
//           {
//               Tree t1_inner;
//               Tree t2_inner;
//               Tree t3_inner;
//               Tree t4_inner;
//
//               //cout << t1.sha1 << endl;
//               //cout << t1.sha1_pointers[i] << endl;
//               //cout << matched2_sha << endl;
//               //cout << matched3_sha << endl;
//
//               load_tree(t1_inner, t1.sha1_pointers[i], HOME);
//               load_tree(t2_inner, matched2_sha, HOME);
//               load_tree(t3_inner, matched3_sha, HOME);
//
//               merge_files (t1_inner, t2_inner, t3_inner, t4_inner, HOME);
//
//               t4.sha1_pointers.push_back( t4_inner.sha1 );
//               t4.mtime.push_back (t1.mtime[i]);
//           }
//       }
//
//     else   // file not present in CA or second branch (no merging)
//       {
//           t4.sha1_pointers.push_back( t1.sha1_pointers[i] );
//           t4.mtime.push_back (t1.mtime[i]);
//       }
//   }
//
//   t4.sha1 = "";
//   for(int i = 0; i < t4.pointer_paths.size(); i++)
//   {
//     t4.sha1 += t4.pointer_perm[i];
//
//     if(t4.type[i] == false)
//       t4.sha1 += "blob ";
//     else
//       t4.sha1 += "tree ";
//
//     t4.sha1 += t4.sha1_pointers[i] + " ";
//     t4.sha1 += t4.pointer_paths[i] + "\n";
//   }
//
//   t4.sha1 = get_string_sha1(t4.sha1);
//
//   save_tree(t4, HOME);
// }
//
//
// void add_files_not_in_t1(Tree& t1, Tree& t2, Tree& t4, string HOME)
// {
//   t4.name = t1.name;
//   t4.type_and_permissions = t1.type_and_permissions;
//
//   for(int i = 0; i < t2.pointer_paths.size(); i++)
//   {
//     auto itr = find(t1.pointer_paths.begin(), t1.pointer_paths.end(), t2.pointer_paths[i]);
//     int itr_index = itr - t1.pointer_paths.begin();
//     string matched_sha = t1.sha1_pointers[itr_index];
//
//     if(itr == t1.pointer_paths.end())   // file present in t2 but not in t1
//       {
//           int k = t2.pointer_paths[i].find_first_of ("/");
//           string actual_path = t2.pointer_paths[i].substr (k+1, t1.pointer_paths[i].size()-1);
//
//           t4.type.push_back(t2.type[i]);
//           t4.pointer_paths.push_back(t2.pointer_paths[i]);
//           t4.pointer_perm.push_back(t2.pointer_perm[i]);
//           t4.mtime.push_back (t2.mtime[i]);
//
//           int itr_index = itr - t1.pointer_paths.begin();
//           //string parent_matched_path = *itr;
//           //string parent_matched_sha = t2.sha1_pointers[itr_index];
//           //string matched_sha = t2.sha1_pointers[itr2_index];
//           //string matched3_sha = t3.sha1_pointers[itr3_index];
//
//           if (t1.type[i] == false)  //if its a blob
//           {
//               //cout << "IN OTHER BRANCH\n" << t2.pointer_paths[i] << endl;
//               t4.sha1_pointers.push_back(t2.sha1_pointers[i]);
//
//               Blob b;
//               //cout << "load:" << t2.sha1_pointers[i] <<endl;
//               load_blob (b, t2.sha1_pointers[i], HOME);
//               update_working_directory_file (actual_path, b);
//           }
//           else
//           {
//               Tree t1_inner;
//               Tree t2_inner;
//               Tree t4_inner;
//
//               load_tree(t1_inner, matched_sha, HOME);
//               load_tree(t2_inner, t2.sha1_pointers[i], HOME);
//
//               add_files_not_in_t1 (t1_inner, t2_inner, t4_inner, HOME);
//
//               t4.sha1_pointers.push_back( t4_inner.sha1 );
//
//               struct stat st;
//               int status = 0;
//               if (stat (&actual_path[0], &st) != 0)
//               {
//                   mkdir(&actual_path[0], 0777);
//               }
//           }
//       }
//   }
//
//   t4.sha1 = "";
//   for(int i = 0; i < t4.pointer_paths.size(); i++)
//   {
//     t4.sha1 += t4.pointer_perm[i];
//
//     if(t4.type[i] == false)
//       t4.sha1 += "blob ";
//     else
//       t4.sha1 += "tree ";
//
//     t4.sha1 += t4.sha1_pointers[i] + " ";
//     t4.sha1 += t4.pointer_paths[i] + "\n";
//   }
//
//   t4.sha1 = get_string_sha1(t4.sha1);
//
//   save_tree(t4, HOME);
// }
//
// void update_working_directory_file (string path, Blob& b)
// {
//   cout << "PATH: " << path << endl;
//   ofstream file (path, ios::trunc | ios::out);
//   file << b.data;
//   file.close ();
// }


#include <string>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include "objects.h"
#include "commit.h"
#include "serialize.h"
#include "reset.h"
using namespace std;

void merge_files(Tree&, Tree&, Tree&, Tree&, string);
void add_files_not_in_t1(Tree&, Tree&, Tree&, string);
void update_working_directory_file (string, Blob&);

bool get_common_ancestor(Commit& ca, Commit& c1, Commit& c2, string HOME)  //return true for fast forward merge
{
  //cout << c1.sha1 << endl;
  //cout << c2.sha1 << endl;
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
    if(m[temp_parent_sha] == 1)
    {
      load_commit(ca, temp_parent_sha, HOME);
      break;
    }
    Commit temp_cm;
    load_commit(temp_cm, temp_parent_sha, HOME);
    temp_parent_sha=temp_cm.parent_sha1;
  }

  //cout << ca.sha1 << endl;
  //cout << c1.sha1 << endl;
  //cout << c2.sha1 << endl;
  if(ca.sha1==c1.sha1 || ca.sha1==c2.sha1)
    return true;
  return false;
}

void bash_merge(Blob& cur, Blob& other, Blob& ca, Blob& new_blob)
{
  string path1 = ".vcs/temp/merge1";
  string path2 = ".vcs/temp/merge2";
  string path3 = ".vcs/temp/merge3";
  string op_path = ".vcs/temp/merge_op";

  ofstream file1(path1, std::ios::binary | std::ios::out | std::ios::trunc);
  ofstream file2(path2, std::ios::binary | std::ios::out | std::ios::trunc);
  ofstream file3(path3, std::ios::binary | std::ios::out | std::ios::trunc);
  ofstream file4(op_path, std::ios::binary | std::ios::out | std::ios::trunc);

  file4.close ();

  file1 << cur.data;
  file2 << ca.data;
  file3 << other.data;

  file1.close();
  file2.close();
  file3.close();

  string command = "bash -c \"merge -p .vcs/temp/merge1 .vcs/temp/merge2 .vcs/temp/merge3 > .vcs/temp/merge_op\"";
  cout << command << endl;
  //cout << command << endl;
  system (command.c_str ());

  ifstream op_file (".vcs/temp/merge_op");
  string str((istreambuf_iterator<char>(op_file)), istreambuf_iterator<char>());
  op_file.close ();

  new_blob.data = str;
}

void merge(string other_branch, string HOME)
{
  if ( !match_commit_and_cwd(HOME) )
  {
    cout << "Working tree and commit do not match. Commit changes first\n";
    exit (1);
  }

  ifstream head (".vcs/HEAD");
  string head_str;
  getline(head, head_str);
  head.close();

  string cur_latest_commit;
  ifstream cur_branch (head_str);
  getline(cur_branch, cur_latest_commit);
  cur_branch.close();

  string other_latest_commit = ".vcs/refs/" + other_branch;
  ifstream othr_branch (other_latest_commit);
  getline(othr_branch, other_latest_commit);
  othr_branch.close();

  Commit cm1;
  load_commit(cm1, cur_latest_commit, HOME);

  Commit cm2;
  load_commit(cm2, other_latest_commit, HOME);

  Commit common_ancestor;
  if (get_common_ancestor (common_ancestor, cm1, cm2, HOME)) //fast forward merge
  {
    if (common_ancestor.sha1 == cm1.sha1)
    {
      ofstream cur_write (head_str, ios::out | ios::trunc);
      cur_write << other_latest_commit;
      cur_write.close ();
    }
    else  // ca == cm2
    {
      string othr_path = ".vcs/refs/" + other_branch;
      ofstream othr_write (othr_path, ios::out | ios::trunc);
      othr_write << cur_latest_commit;
      othr_write.close ();
    }

    cout << "Fast Forward Merge" << endl;
    cout << common_ancestor.message<<endl;
    return;
  }
  cout << common_ancestor.message << endl;
  cur_latest_commit = cm1.tree_sha1;
  other_latest_commit = cm2.tree_sha1;

  Tree t1, t2;
  load_tree(t1, cur_latest_commit, HOME);
  load_tree(t2, other_latest_commit, HOME);

  Tree t3;
  load_tree(t3, common_ancestor.tree_sha1, HOME);

  int k = HOME.find_last_of ("/");
  string dir_name = HOME.substr (k+1, HOME.size()-1);
  //cout << dir_name;
  string command = "bash -c \"mkdir .vcs/temp/" + dir_name + "\nrsync -Rr ./ .vcs/temp/" + dir_name + "\"";
  system (command.c_str ());
  //cout << command << endl;
  //common_ancestor_reset(common_ancestor.sha1, cm1.sha1, HOME);

  Tree t4;  // tree for new commit

  //cout << "\n\n\n\n\n\nSIZE\n\n\n";
  //cout << t1.sha1_pointers.size() << endl;
  merge_files(t1, t2, t3, t4, HOME);

  add_files_not_in_t1(t1, t2, t4, HOME);

  merge_commit(cm1, cm2, common_ancestor, t4, HOME);

  command = "bash -c \"rm -R .vcs/temp/" + dir_name + "\"";
  system (command.c_str ());
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
    t4.is_binary_file.push_back (t1.is_binary_file[i]);

    auto itr2 = find(t2.pointer_paths.begin(), t2.pointer_paths.end(), t1.pointer_paths[i]);
    auto itr3 = find(t3.pointer_paths.begin(), t3.pointer_paths.end(), t1.pointer_paths[i]);
    if(itr2 != t2.pointer_paths.end() && itr3 != t3.pointer_paths.end())   // file present everywhere
      {
          int k = t1.pointer_paths[i].find_first_of ("/");
          string actual_path = t1.pointer_paths[i].substr (k+1, t1.pointer_paths[i].size()-k);
//cout << "ACTUAL: " << actual_path <<endl;
          int itr2_index = itr2 - t2.pointer_paths.begin();
          int itr3_index = itr3 - t3.pointer_paths.begin();
          //string parent_matched_path = *itr;
          //string parent_matched_sha = t2.sha1_pointers[itr_index];
          string matched2_sha = t2.sha1_pointers[itr2_index];
          string matched3_sha = t3.sha1_pointers[itr3_index];

          if (t1.type[i] == false)  //if its a blob
          {
                if (t1.mtime[i] != t3.mtime[itr3_index]
                    && t2.mtime[itr2_index] != t3.mtime[itr3_index]) /* If file
                                                      changed in both branches*/
                {
                  if (!t1.is_binary_file[i])
                  {
                    cout << t1.pointer_paths[i] << endl;
                    cout << t2.pointer_paths[itr2_index] << endl;
                    cout << t3.pointer_paths[itr3_index] << endl;

                    Blob b1, b2, b3, b4;
                    load_blob(b1 ,t1.sha1_pointers[i], HOME);
                    load_blob(b2 ,t2.sha1_pointers[itr2_index], HOME);
                    load_blob(b2 ,t2.sha1_pointers[itr3_index], HOME);

                    bash_merge(b1, b2, b3, b4);

                    get_blob_sha1(b4);
                    t4.sha1_pointers.push_back( b4.sha1 );
                    //cout << delta << endl;
                    save_blob(b4 ,HOME);

                    update_working_directory_file (actual_path, b4);

                    struct stat st;
                    string merged_blob_path = ".vcs/objects/" + b4.sha1;
                    stat(&merged_blob_path[0], &st);
                    t4.mtime.push_back ((unsigned long)st.st_mtime);
                  }
                  else
                  {
                    t4.sha1_pointers.push_back( t1.sha1_pointers[i] );
                    t4.mtime.push_back (t1.mtime[i]);
                  }
                }
                // if changed only in other branch
                else if(t2.mtime[itr2_index] != t3.mtime[itr3_index])
                {
                  t4.sha1_pointers.push_back( matched2_sha );
                  t4.mtime.push_back (t2.mtime[itr2_index]);

                  Blob b;
                  //cout << "load:" << matched2_sha;
                  load_blob (b, matched2_sha, HOME);
                  update_working_directory_file (actual_path, b);
                }
                // if not changed or only changed in current branch
                else
                {
                  t4.sha1_pointers.push_back( t1.sha1_pointers[i] );
                  t4.mtime.push_back (t1.mtime[i]);
                }
          }
          else  // if it's a tree
          {
              Tree t1_inner;
              Tree t2_inner;
              Tree t3_inner;
              Tree t4_inner;

              //cout << t1.sha1 << endl;
              //cout << t1.sha1_pointers[i] << endl;
              //cout << matched2_sha << endl;
              //cout << matched3_sha << endl;

              load_tree(t1_inner, t1.sha1_pointers[i], HOME);
              load_tree(t2_inner, matched2_sha, HOME);
              load_tree(t3_inner, matched3_sha, HOME);

              merge_files (t1_inner, t2_inner, t3_inner, t4_inner, HOME);

              t4.sha1_pointers.push_back( t4_inner.sha1 );
              t4.mtime.push_back (t1.mtime[i]);
          }
      }

    else   // file not present in CA or second branch (no merging)
      {
          t4.sha1_pointers.push_back( t1.sha1_pointers[i] );
          t4.mtime.push_back (t1.mtime[i]);
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
          int k = t2.pointer_paths[i].find_first_of ("/");
          string actual_path = t2.pointer_paths[i].substr (k+1, t1.pointer_paths[i].size()-1);

          t4.type.push_back(t2.type[i]);
          t4.pointer_paths.push_back(t2.pointer_paths[i]);
          t4.pointer_perm.push_back(t2.pointer_perm[i]);
          t4.mtime.push_back (t2.mtime[i]);
          t4.is_binary_file.push_back (t2.is_binary_file[i]);

          int itr_index = itr - t1.pointer_paths.begin();
          //string parent_matched_path = *itr;
          //string parent_matched_sha = t2.sha1_pointers[itr_index];
          //string matched_sha = t2.sha1_pointers[itr2_index];
          //string matched3_sha = t3.sha1_pointers[itr3_index];

          if (t1.type[i] == false)  //if its a blob
          {
              //cout << "IN OTHER BRANCH\n" << t2.pointer_paths[i] << endl;
              t4.sha1_pointers.push_back(t2.sha1_pointers[i]);

              Blob b;
              //cout << "load:" << t2.sha1_pointers[i] <<endl;
              load_blob (b, t2.sha1_pointers[i], HOME);
              update_working_directory_file (actual_path, b);
          }
          else
          {
              Tree t1_inner;
              Tree t2_inner;
              Tree t4_inner;

              load_tree(t1_inner, matched_sha, HOME);
              load_tree(t2_inner, t2.sha1_pointers[i], HOME);

              add_files_not_in_t1 (t1_inner, t2_inner, t4_inner, HOME);

              t4.sha1_pointers.push_back( t4_inner.sha1 );

              struct stat st;
              int status = 0;
              if (stat (&actual_path[0], &st) != 0)
              {
                  mkdir(&actual_path[0], 0777);
              }
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

void update_working_directory_file (string path, Blob& b)
{
  cout << "PATH: " << path << endl;
  ofstream file (path, ios::trunc | ios::out);
  file << b.data;
  file.close ();
}
