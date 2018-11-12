#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "objects.h"
#include <string>
using namespace std;

void save_blob(Blob, string);
void load_blob(Blob&, string, string);

void save_index(Index, string);
void load_index(Index&, string);

#endif
