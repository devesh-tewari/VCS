#ifndef REVERT_H
#define REVERT_H

#include <string>
using namespace std;


void revert(string , string ,string);
void revertUtil(string ,string ,string );
string patch1(string , string );
void delete_tree_files_of_reverted_commit(string , string , string , string);

#endif
