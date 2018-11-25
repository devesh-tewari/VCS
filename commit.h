#ifndef COMMIT_H
#define COMMIT_H

#include <string>
using namespace std;

void commit(string,string);
void match_commit (string,string,string);
void merge_commit(Commit&, Commit&, Commit&, Tree&, string);

#endif
