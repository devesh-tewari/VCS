#ifndef CREATE_FILES_AFTER_RESET_H
#define CREATE_FILES_AFTER_RESET_H


#include <bits/stdc++.h>
using namespace std;


#define DEFAULT_MODE      S_IRWXU | S_IRGRP |  S_IXGRP | S_IROTH | S_IXOTH

void create_tree_files_into_wd(string current_sha,string HOME);
#endif
