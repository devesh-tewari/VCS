#include <string>
#include <iostream>
#include "objects.h"
using namespace std;

string diff(string s1, string s2)
{
  string path1 = ".vcs/temp/diff1";
  string path2 = ".vcs/temp/diff2";
  // string path3 = ".vcs/temp/patch";
  ofstream file1(path1, std::ios::binary | std::ios::out | std::ios::trunc);
  ofstream file2(path2, std::ios::binary | std::ios::out | std::ios::trunc);
  file1 << s1;
  file2 << s2;
  file1.close();
  file2.close();
<<<<<<< HEAD
  string command = "bash -c \"diff .vcs/temp/diff2 .vcs/temp/diff1 >.vcs/temp/patch\"";
  cout << command << endl;
  system (command.c_str ());
  ifstream myfile (".vcs/temp/patch");
  string data="";
  string line;
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
      data+=line;
      data+="\n";
    }
    myfile.close();
  }

  else cout << "Unable to open file"; 

  cout<<data<<"diffresult\n";
  return data;
=======
  string command = "bash -c \"diff .vcs/temp/diff1 .vcs/temp/diff2\"";
  //cout << command << endl;
  string result;
  result = system (command.c_str ());
  cout<<result<<"diffresult\n";
  return result;
>>>>>>> 1792f787fa48125cef0d9bd11e07573663b43fbf
}

/*int main ()
{
  string s1, s2;
  s1 = "fsd'gd\n\nsfsd\n";
  s2 = "fdsfd\tms\nkmfskd\n";
  cout << diff(s1, s2) << endl;
  return 0;
}*/
