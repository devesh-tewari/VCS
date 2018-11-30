#include <bits/stdc++.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include "objects.h"
#include "serialize.h"
#include "diff.h"
#include <string>
#include <fstream>
using namespace std;

void vcsdiff(string);
void vcsdiff(string, string, string);
void vcsdiff_cached(string);
void match_commit_stagging(string treesha, unordered_map<string, string> &paths, string HOME);

bool file_exists(const std::string &name)
{
    ifstream f(name.c_str());
    return f.good();
}

void vcsdiff(string HOME)
{
    Index INDEX;
    struct stat st;
    string index_path = HOME + "/.vcs/INDEX";
    if (stat(&index_path[0], &st) != 0) // nothing added in index
        return;

    load_index(INDEX, HOME);
    for (int i = 0; i < INDEX.entries.size(); i++)
    {
        string path = INDEX.entries[i].path;
        //cout << path <<"\n";
        if (!file_exists(path))
        {
            cout <<"------------------------------\n"<< path << " Deleted from Working Directory\n";
            continue;
        }
        string path_sha1 = INDEX.entries[i].sha1;

        struct stat st;
        stat(&path[0], &st);
        if ((unsigned long)st.st_mtime == INDEX.entries[i].mtime)
        {
            continue;
        }

        Blob curr_blob;
        load_blob(curr_blob, path_sha1, HOME);

        string path2 = ".vcs/temp/diff2";
        ofstream file2(path2, std::ios::binary | std::ios::out | std::ios::trunc);
        file2 << curr_blob.data;
        file2.close();
        string command = "bash -c \"diff -u " + path + " .vcs/temp/diff2\"";
        //cout << command << endl;
        system(command.c_str());
    }
}

void vcsdiff(string HOME, string path1, string path2)
{
    if (!file_exists(path1))
    {
        cout << path1 << " doesn't exist";
        return;
    }
    if (!file_exists(path2))
    {
        cout << path2 << " doesn't exist";
        return;
    }

    string command = "bash -c \"diff -u " + path1 + " " + path2 + " \"";
    cout << command << endl;
    string result;
    result = system(command.c_str());
    cout << result << "\n";
    return;
}

void vcsdiff_cached(string HOME)
{
    Index INDEX;
    struct stat st;
    string index_path = HOME + "/.vcs/INDEX";

    if (stat(&index_path[0], &st) == 0)
    {
        load_index(INDEX, HOME);
    }
    else
    {
        cout << "nothing added in staging" << endl;
        return;
    }

    ifstream head(".vcs/HEAD");
    string head_str;
    getline(head, head_str);
    head.close();

    ifstream branch_read(head_str);
    string commit_sha;
    getline(branch_read, commit_sha);
    branch_read.close();

    cout << commit_sha << "\n";

    Commit cm;
    load_commit(cm, commit_sha, HOME);

    string file_path, file_sha;
    unordered_map<string, string> paths;
    for (int i = 0; i < INDEX.entries.size(); i++)
    {
        //file_path = HOME + "/" + INDEX.entries[i].path;
        file_path = INDEX.entries[i].path;
        file_sha = INDEX.entries[i].sha1;
        //cout << file_path << "\t" << file_path << "\n";
        paths.insert(make_pair(file_path, file_sha));
    }
    
    //cout << paths.size() << "\n";
    match_commit_stagging(cm.tree_sha1, paths, HOME);
    //cout << paths.size() << "\n";
    for (auto itr = paths.begin(); itr != paths.end(); ++itr)
    {
        if(itr->second.compare("")!=0)
        cout << itr->first << " new file added in staging\n";
    }
}

void match_commit_stagging(string treesha, unordered_map<string, string> &paths, string HOME)
{
    Tree curr_tree;
    load_tree(curr_tree, treesha, HOME);

    for (int i = 0; i < curr_tree.pointer_paths.size(); i++)
    {
        if (curr_tree.type[i] == false) //if its a blob
        {
            //cout << curr_tree.pointer_paths[i] <<"\t"<<paths[curr_tree.pointer_paths[i]] <<"\n";
            int k = curr_tree.pointer_paths[i].find_first_of("/");
            string temp_path = curr_tree.pointer_paths[i].substr(k + 1, curr_tree.pointer_paths[i].size() - k - 1);
            //cout << temp_path<<"\n";
            if (paths[temp_path].compare("") != 0) // last commit data present in stagging
            {

                Blob commit_tree_blob, index_blob;
                load_blob(commit_tree_blob, curr_tree.sha1_pointers[i], HOME);
                load_blob(index_blob, paths[temp_path], HOME);

                string path1 = ".vcs/temp/diff1";
                string path2 = ".vcs/temp/diff2";
                ofstream file1(path1, std::ios::binary | std::ios::out | std::ios::trunc);
                ofstream file2(path2, std::ios::binary | std::ios::out | std::ios::trunc);
                file1 << commit_tree_blob.data;
                file2 << index_blob.data;
                file1.close();
                file2.close();
                string command = "bash -c \"diff -u .vcs/temp/diff1 .vcs/temp/diff2\"";
                system(command.c_str());
                //paths[curr_tree.pointer_paths[i]]="";
                paths.erase(temp_path);
            }
            else
            {
                cout << curr_tree.pointer_paths[i] << " absent in staging\n";
            }
        }
        else
        {
            match_commit_stagging(curr_tree.sha1_pointers[i], paths, HOME);
        }
    }
}

// int main()
// {
//     // string p1,p2;
//     // cin>>p1;
//     // cin>>p2;
//     // string h="sdfg";
//     // vcsdiff(h,p1,p2);
//     string home = "/home/sanket/IIITH/Sem1/OS/project";
//     vcsdiff_cached(home);
//     return 0;
// }
