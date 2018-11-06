#include "serialize.h"
#include "objects.h"
#include <string>
#include <ctime>
#include <set>
#include "cereal/types/vector.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/binary.hpp"
#include <fstream>
using namespace std;

void save_blob(Blob bl, string HOME)
{
    string path = HOME + "/.vcs/objects/" + bl.sha1;
    ofstream os(path, ios::binary | ios::out);
    cereal::BinaryOutputArchive archive( os );
    Blob myData;
    archive( myData );
    os.close();
}

Blob& load_blob(string sha, string HOME)
{
    string path = HOME + "/.vcs/objects/" + sha;
    ifstream is(path, ios::binary| ios::in);
    cereal::BinaryInputArchive iarchive( is );
    Blob idata;
    iarchive(idata);
    is.close();
    return idata;
}

/*void save_tree(Tree)
{
    std::ofstream os(path_with_file_name, std::ios::binary | std::ios::out);
    cereal::BinaryOutputArchive archive( os );
    Blob myData;
    archive( myData );
    os.close();
}

Tree& load_tree(Tree)
{
    std::ifstream is(path, std::ios::binary| std::ios::in);
    cereal::BinaryInputArchive iarchive( is );
    Blob idata;
    iarchive(idata);
    is.close();
    return idata;
}

void save_commit(Commit)
{
    std::ofstream os(path_with_file_name, std::ios::binary | std::ios::out);
    cereal::BinaryOutputArchive archive( os );
    Blob myData;
    archive( myData );
    os.close();
}

Commit& load_commit(Commit)
{
    std::ifstream is(path, std::ios::binary| std::ios::in);
    cereal::BinaryInputArchive iarchive( is );
    Blob idata;
    iarchive(idata);
    is.close();
    return idata;
}*/
