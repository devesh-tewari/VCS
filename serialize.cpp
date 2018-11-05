#include"objects.h"
#include <string>
#include <ctime>
#include <set>
#include "cereal/types/set.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/binary.hpp"
#include <fstream>
using namespace std;

void save_blob(Blob,path_with_file_name)
{
    std::ofstream os(path_with_file_name, std::ios::binary | std::ios::out);
    cereal::BinaryOutputArchive archive( os );
    Blob myData;
    archive( myData );
    os.close();
}

Blob& load_blob(Blob,path)
{
    std::ifstream is(path, std::ios::binary| std::ios::in);
    cereal::BinaryInputArchive iarchive( is );
    Blob idata;
    iarchive(idata);
    is.close();
    return idata;
}

void save_tree(Tree)
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
}