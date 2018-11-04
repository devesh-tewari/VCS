#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <ctime>
#include <set>
using namespace std;

/* Type and permissions format:
   16 low bits: 4-bit object type, 3-bit unused, 9-bit unix permissions.
   For blob, 1st type bit is set, for tree, second one and for commit,
   third one */

class Blob
{
  string name;
  int type_and_permissions;
  string sha1;
  string bin_data;

  Tree(int Name, int Type_and_permissions)
  {
    name = Name;
    type_and_permissions = Type_and_permissions;
  }
}

class Tree
{
  string name;
  int type_and_permissions;
  string sha1;
  set<string> sha1_pointers;

  Tree(int Name, int Type_and_permissions)
  {
    name = Name;
    type_and_permissions = Type_and_permissions;
  }
}

class Commit
{
  string tree_hash;
  string parent_hash;
  string author;
  string committer;
  time_c date;
  string message;
}

void set_time_and_permissions();

/*void save_blob(Blob);
void load_blob(Blob);

void save_tree(Tree);
void load_tree(Tree);

void save_commit(Commit);
void load_commit(Commit);*/

#endif OBJECTS_H
