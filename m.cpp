#include "cereal/types/vector.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/binary.hpp"
#include <fstream>
#include <string>
using namespace std;

class Blob
{
  public:
    string x="a";
    int y=10;
    string z="b";
    string w="c";
    int l;
    vector<string> b{"a","b"};
    void insert(string temp)
    {
      b.push_back(temp);
    }
  template <class Archive>
  void serialize( Archive & ar )
  {
    ar( x,y,z,w,l,b);
  }

};



int main()
{
  std::ofstream os("out", std::ios::binary | std::ios::out);
  cereal::BinaryOutputArchive archive( os );

  Blob myData;
  myData.l=7;
  myData.insert("fdnfndjf");
    //cout << myData.l <<endl;
  // myData.ab.insert("43534dfffffffff");
  archive( myData );
  os.close();

  
  std::ifstream is("out", std::ios::binary| std::ios::in);
  cereal::BinaryInputArchive iarchive( is );

  Blob idata;
  iarchive(idata);
  cout<<idata.x<<endl;
    cout <<idata.b[2] <<endl;
  is.close();
  return 0;
}