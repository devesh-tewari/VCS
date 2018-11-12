#include "serialize.h"
#include "objects.h"
#include <string>
#include <ctime>
#include <vector>
#include "cereal/types/vector.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/binary.hpp"
#include <fstream>
using namespace std;

void save_blob(Blob bl, string HOME)
{
    string path = HOME + "/.vcs/objects/" + bl.sha1;
    ofstream os(path, std::ios::binary | std::ios::out);
    cereal::BinaryOutputArchive archive( os );
    archive( bl );
    os.close();
}

void load_blob(Blob &bl, string sha, string HOME)
{
    string path = HOME + "/.vcs/objects/" + sha;
    ifstream is(path, std::ios::binary| std::ios::in);
    cereal::BinaryInputArchive iarchive( is );
    iarchive(bl);
    is.close();
}
/*
void save_tree(Tree tr, string HOME)
{
    std::ofstream os(path_with_file_name, std::ios::binary | std::ios::out);
    cereal::BinaryOutputArchive archive( os );
    archive( tr );
    os.close();
}

Tree& load_tree(Tree)
{
    std::ifstream is(path, std::ios::binary| std::ios::in);
    cereal::BinaryInputArchive iarchive( is );
    Tree idata;
    iarchive(idata);
    is.close();
    return idata;
}

void save_commit(Commit cm, string HOME)
{
    std::ofstream os(path_with_file_name, std::ios::binary | std::ios::out);
    cereal::BinaryOutputArchive archive( os );
    archive( cm );
    os.close();
}

Commit& load_commit(string HOME)
{
    string path="Commit file Path daalna"
    std::ifstream is(path, std::ios::binary| std::ios::in);
    cereal::BinaryInputArchive iarchive( is );
    Commit idata;
    iarchive( idata );
    is.close();
    return idata;
}*/

void save_index(Index INDEX, string HOME)
{
    string path = HOME + "/.vcs/INDEX";
    std::ofstream os(path, std::ios::binary | std::ios::out);
    cereal::BinaryOutputArchive archive( os );
    archive( INDEX );
    os.close();
}

void load_index(Index &INDEX, string HOME)
{
    string path = HOME + "/.vcs/INDEX";
    std::ifstream is(path, std::ios::binary| std::ios::in);
    cereal::BinaryInputArchive iarchive( is );
    iarchive( INDEX );
    is.close();
}
