#include <stdio.h>
#include <string>
#include "objects.h"
using namespace std;

bool is_binary (Blob& bl)
{
  bool flag = false;
  for (int i = 0; i < bl.data.size(); i *= 4)
  {
    if (!isascii (s[i]))
    {
      flag = true;
    }
  }
  return flag;
}
