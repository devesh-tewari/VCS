#include "init.h"
#include "objects.h"

int main(int argc, char** argv)
{
  if(argc == 0)
    return 0;

  string HOME = ".";
//cout << argc << endl;
  if(strcmp(argv[1], "init") == 0)
  {
    if(argc > 2)
    {
      init(argv[2]);
      HOME = argv[2];
    }
    else
    {
      init(".");
      HOME = ".";
    }
    return 0;
  }

  int i;
  if(strcmp(argv[1], "add") == 0)
  {
    //if(HOME == "")
      //return 0;
    if(argc > 2)
    {
      vector<string> v;
      for(i = 2; i < argc; i++)
        v.push_back(argv[i]);
      add(v, HOME);
    }
    return 0;
  }
  return 0;
}
