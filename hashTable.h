// used form Professor Femmister

#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "hash.h"
#include "link.h"
#include <iostream>
#include <cstring>

using namespace std;

class HashTable{
public:
  HashTable(int n=97);
  //HashTable();
  HashTable(const HashTable &t);
  ~HashTable();

  HashTable& add(const string str, string d);

  bool inTable(const string)const;

  string operator [](const string);

  
private:
  int size;   //the size of the array table
  Link **table;  //dynamically allocated array of pointers, each
  static void check(bool b, const string mess);
};

#endif
