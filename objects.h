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
  string name;
  int type_and_permissions;
  string sha1;
  vector<string> sha1_pointers;
  vector<string> pointer_paths; //including filename
  vector<int> pointer_perm;

  Tree(int Name, int Type_and_permissions)
  {
    name = Name;
    type_and_permissions = Type_and_permissions;
  }

  template <class Archive>
  void serialize( Archive & ar )
  {
    ar(name, type_and_permissions, sha1, sha1_pointers, pointer_paths, pointer_perm);
  }
};

class Commit
{
public:
  string sha1;
  string tree_hash;
  string parent_hash;
  string author;
  string committer;
  time_t timestamp;
  string message;

  template <class Archive>
  void serialize( Archive & ar )
  {
    ar(sha1, tree_hash, parent_hash, author, committer, timestamp, message);
  }
};

class IndexEntry
{
public:
  int type_and_permissions;
  string sha1;
  string path;
  unsigned long int mtime;

  IndexEntry() {}
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

int set_time_and_permissions(int, int);
void add(vector<string>, string);

#endif
