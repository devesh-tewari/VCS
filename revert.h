#ifndef REVERT_H
#define REVERT_H

#include <string>
using namespace std;


void reset( string destination_sha , string current_sha , string option ,string HOME);
void resetUtil (string curr_sha,string parent_sha,string HOME);
string patch( string curr_blob_data, string parent_file_patch);

#endif
