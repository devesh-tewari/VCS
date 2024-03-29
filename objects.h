#ifndef OBJECTS_H
#define OBJECTS_H

#include <ctime>
#include <bits/stdc++.h>
using namespace std;

/* Type and permissions format:
   16 low bits: 4-bit object type, 3-bit unused, 9-bit unix permissions.
   For blob, 1st type bit is set, for tree, second one and for commit,
   third one */

class Blob
{
public:
  string name;
  int type_and_permissions;
  string sha1;
  string data;

  Blob() {}

  Blob(string Name, int Type_and_permissions)
  {
    name = Name;
    type_and_permissions = Type_and_permissions;
  }

  template <class Archive>
  void serialize( Archive & ar )
  {
    ar(name, type_and_permissions, sha1, data);
  }
};

class Tree
{
public:
  string name; // used path instead including dirname
  int type_and_permissions;
  string sha1;
  vector<string> sha1_pointers;
  vector<string> pointer_paths; //including filename
  vector<string> pointer_perm;
  vector<long int> mtime;
  vector<bool> type; // 0 for blob and 1 for tree
  vector<bool> is_binary_file;

  Tree() {}

  Tree(int Name, int Type_and_permissions)
  {
    name = Name;
    type_and_permissions = Type_and_permissions;
  }

  template <class Archive>
  void serialize( Archive & ar )
  {
    ar (name, type_and_permissions, sha1, sha1_pointers, pointer_paths,
        pointer_perm, mtime, type, is_binary_file);
  }
};

class Commit
{
public:
  string sha1;
  string tree_sha1;
  string parent_sha1;
  string second_parent_sha1;
  string author;
  string committer;
  time_t timestamp;
  string message;
  bool is_new_branch;

  Commit()
  {
    this->timestamp = time(0);   // get time now
    this->parent_sha1 = "";
    this->second_parent_sha1 = "";
    this->is_new_branch = false;
  }

  template <class Archive>
  void serialize( Archive & ar )
  {
    ar (sha1, tree_sha1, parent_sha1, author, committer, timestamp, message,
        is_new_branch);
  }
};

class IndexEntry
{
public:
  int type_and_permissions;
  string sha1;
  string path;
  unsigned long int mtime;
  bool is_binary_file;

  IndexEntry()
  {
    this->is_binary_file = false;
  }

template <class Archive>
  void serialize( Archive & ar )
  {
    ar(type_and_permissions, sha1, path, mtime);
  }

};

class Index
{
public:
  vector< IndexEntry > entries;

  Index()
  {
    entries.clear();
  }

  template <class Archive>
  void serialize( Archive & ar )
  {
    ar(entries);
  }

};

void get_blob_sha1(Blob&);
string get_string_sha1(string);

int set_time_and_permissions(int, int);

bool is_binary (Blob&);

#endif
