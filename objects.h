#ifndef OBJECTS_H
#define OBJECTS_H

#include <string>
#include <ctime>
#include <set>
using namespace std;

class Blob
{
  string name;
  int type_and_permissions;
  string sha1;
  string bin_data;
}

class Tree
{
  string name;
  int type_and_permissions;
  string sha1;
  set<string> blobs_sha1;
  set<string> trees_sha1;
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

void save_blob(Blob);
void load_blob(Blob);

void save_tree(Tree);
void load_tree(Tree);

void save_commit(Commit);
void load_commit(Commit);

#endif OBJECTS_H
