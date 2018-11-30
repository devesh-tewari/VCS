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

void save_blob(Blob& bl, string HOME)
{
    string path = HOME + "/.vcs/objects/" + bl.sha1;
    ofstream os(path, std::ios::binary | std::ios::out | std::ios::trunc);
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

void save_tree(Tree& tr, string HOME)
{
    string path = HOME + "/.vcs/objects/" + tr.sha1;
    ofstream os(path, std::ios::binary | std::ios::out | std::ios::trunc);
    cereal::BinaryOutputArchive archive( os );
    archive( tr );
    os.close();
}

void load_tree(Tree &tr, string sha, string HOME)
{
    string path = HOME + "/.vcs/objects/" + sha;
    ifstream is(path, std::ios::binary| std::ios::in);
    cereal::BinaryInputArchive iarchive( is );
    iarchive(tr);
    is.close();
}

void save_commit(Commit& cm, string HOME)
{
    string path = HOME + "/.vcs/objects/" + cm.sha1;
    ofstream os(path, std::ios::binary | std::ios::out | std::ios::trunc);
    cereal::BinaryOutputArchive archive( os );
    archive( cm );
    os.close();
}

void load_commit(Commit &cm, string sha, string HOME)
{
    string path = HOME + "/.vcs/objects/" + sha;
    ifstream is(path, std::ios::binary| std::ios::in);
    cereal::BinaryInputArchive iarchive( is );
    iarchive(cm);
    is.close();
}

void save_index(Index &INDEX, string HOME)
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
