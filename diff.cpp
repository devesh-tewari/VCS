#include <string>
#include <iostream>
#include "objects.h"
using namespace std;

string diff(string s1, string s2)
{
  string path1 = ".vcs/temp/diff1";
  string path2 = ".vcs/temp/diff2";
  ofstream file1(path1, std::ios::binary | std::ios::out | std::ios::trunc);
  ofstream file2(path2, std::ios::binary | std::ios::out | std::ios::trunc);
  file1 << s1;
  file2 << s2;
  file1.close();
  file2.close();
  string command = "bash -c \"diff .vcs/temp/diff1 .vcs/temp/diff2\"";
  //cout << command << endl;
  string result;
  return result = system (command.c_str ());
}

/*int main ()
{
  string s1, s2;
  s1 = "fsd'gd\n\nsfsd\n";
  s2 = "fdsfd\tms\nkmfskd\n";
  cout << diff(s1, s2) << endl;
  return 0;
}*/
