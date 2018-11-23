#include <bits/stdc++.h>
using namespace std;
string diff(string s1, string s2)
{
  string command = "diff <(echo \"" + s1 + "\") <(echo \"" + s2 + "\")";
  cout << command << endl;
  string result;
  return result = system (command.c_str());
}

int main ()
{
  string s1, s2;
  cin>>s1;
  cin>>s2;
  cout << diff(s1, s2);
  return 0;
}
