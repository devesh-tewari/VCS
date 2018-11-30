#include <string>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include "objects.h"
#include "commit.h"
#include "serialize.h"
#include "reset.h"
using namespace std;

void log(string HOME)
{
    string current_sha;
    ifstream head(".vcs/HEAD");
    string head_str;
    getline(head, head_str);
    head.close();

    ifstream branch_read(head_str);
    getline(branch_read, current_sha);
    branch_read.close();
    while(current_sha!="")
    {
        Commit curr;
        load_commit(curr,current_sha,HOME);
        printf("\033[0;33m");
        cout<<"commit "<<current_sha<<endl;
        printf("\033[0m");
        cout<<"Author: "<<curr.author<<endl;
        tm* cm_time = localtime(&curr.timestamp);
        cout<<"Date: "<<asctime(cm_time);
        cout<<endl;
        cout<<"\t"<<curr.message<<endl<<endl;
        current_sha=curr.parent_sha1;
    }


}