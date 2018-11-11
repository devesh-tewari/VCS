#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "objects.h"
#include <string>
using namespace std;

void save_blob(Blob, string);
Blob& load_blob(string);

void save_index(Index, string);
Index& load_index(string);

#endif
