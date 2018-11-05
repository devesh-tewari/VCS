#include<string>
#include<bits/stdc++.h>
using namespace std;
#pragma once

// Forward declaration of class boost::serialization::access
namespace boost {
namespace serialization {
class access;
}
}

class Obj {
public:
  // Serialization expects the object to have a default constructor
  Obj() {}
  string name;
  int type_and_permissions;
  string sha1;
  string bin_data;


  // Allow serialization to access non-public data members.
  friend class boost::serialization::access;

  template<typename Archive>
  void serialize(Archive& ar, const unsigned version) {
    ar & name & type_and_permissions & sha1 & bin_data;  // Simply serialize the data members of Obj
  }
};
