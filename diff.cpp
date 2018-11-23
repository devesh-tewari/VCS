#include <bits/stdc++.h>
using namespace std;
string diff(string s1, string s2)
{
  string command = "s1='" + s1 + "'\n";
  command += "s2='" + s2 + "'\n";
  command += "bash -c \"diff <(echo '$s1') <(echo '$s2')\"";
  cout << command << endl;
  string result;
  return result = system (command.c_str ());
}

int main ()
{
  string s1, s2;
  s1 = "fsdg\"d\n\nsfsd";
  s2 = "fdsfd\tms\nkmfskd";
  cout << diff(s1, s2) << endl;
  return 0;
}
