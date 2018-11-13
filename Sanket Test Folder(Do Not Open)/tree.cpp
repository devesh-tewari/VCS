#include <bits/stdc++.h>
#include <unordered_map>
#include <openssl/sha.h>
#include <iostream>
#include <fstream>  

using namespace std;
string a[7];
string sha[7]; 
unordered_map<string, string> umap;
vector< pair <int,string> > depth; 
void add_to_map(string ,string ,string ,int );


bool sortinrev(const pair<int,string> &a,  
               const pair<int,string> &b) 
{ 
       return (a.first > b.first); 
}

string get_tree_sha1(string str)
{
    char *text;
    text=&str[0];
    unsigned char hash[SHA_DIGEST_LENGTH];

    SHA1((unsigned char *)text, str.size(), hash);

    char sha1string[SHA_DIGEST_LENGTH*2 +1];
    for(int i = 0; i < SHA_DIGEST_LENGTH; ++i)
    {
        sprintf(&sha1string[i*2], "%02x", (unsigned int)hash[i]);
    }
    return sha1string;
}


int main()
{
    a[0]="a/1.txt";
    a[1]="a/2.txt";
    a[2]="a/b/3.txt";
    a[3]="a/b/4.txt";
    a[4]="a/b/c/5.txt";
    a[5]="a/b/d/6.txt";
    a[6]="a/e/f/g/h/7.txt";

    sha[0]="a/1.txt";
    sha[1]="a/2.txt";
    sha[2]="a/b/3.txt";
    sha[3]="a/b/4.txt";
    sha[4]="a/b/c/5.txt";
    sha[5]="a/b/d/6.txt";
    sha[6]="a/e/f/g/h/7.txt";



    for(int i=0;i<7;i++)
    {
        string temp=a[i];
        int k = temp.find_first_of("/");
        string dir_name = temp.substr(0, k);
        string remaining_part=temp.substr(k+1,temp.size()-k);
        //cout<<remaining_part<<"\n";
        int temp_depth=0;
        if(umap[dir_name].compare(""))
        {
                
                add_to_map(dir_name,remaining_part,sha[i],temp_depth);
                
        }
	    else
        {
                umap[dir_name]="dummy";
                depth.push_back( make_pair(temp_depth,dir_name) ); 
                add_to_map(dir_name,remaining_part,sha[i],temp_depth);

        }
    }

     for (auto itr = umap.begin(); itr != umap.end(); itr++) 
    { 
        
        cout << itr->first << "  " << itr->second << endl; 
     } 

     sort(depth.begin(), depth.end(), sortinrev);

     while(depth.size())
     {
        cout << depth[0].first << " " << depth[0].second << endl; 
        string tree_obj_data=umap[depth[0].second];
        tree_obj_data=tree_obj_data.substr(5,tree_obj_data.size()-5);  //dummy removed
        cout <<tree_obj_data<<"\n";
        string tree_obj_data_sha=get_tree_sha1(tree_obj_data);

        //create file in object folder
        std::ofstream outfile ("objects/"+tree_obj_data_sha);
        outfile << tree_obj_data << std::endl;
        outfile.close();

        int k = depth[0].second.find_last_of("/");
        string parent = depth[0].second.substr(0, k);
        umap[parent]=umap[parent]+tree_obj_data_sha+"\n";


        depth.erase(depth.begin());
     }
   
    return 0;
}


void add_to_map(string prev_path,string current_path,string sha1,int cur_depth)
{
        string temp=current_path;
        int k = temp.find_first_of("/");
        if(k>0)
        {
            string dir_name = temp.substr(0, k);
            string remaining_part=temp.substr(k+1,temp.size()-k);
            //cout<<remaining_part<<"\n";
            string map_entry=prev_path+"/"+dir_name;
            if(umap[map_entry].compare(""))
            {
                    
                    add_to_map(map_entry,remaining_part,sha1,cur_depth+1);
            }
            else
            {
                    umap[map_entry]="dummy";
                    depth.push_back( make_pair(cur_depth+1,prev_path +"/"+dir_name) ); 
                    add_to_map(map_entry,remaining_part,sha1,cur_depth+1);

            }
        }
        else
        {
            umap[prev_path]=umap[prev_path]+sha1+"\n";
        }
}