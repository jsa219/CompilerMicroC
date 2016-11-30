/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * hash.cc
 */
 #include "hash.h"
using namespace std;

Hash::Hash(const string str) {
  int tmp = 0;
  char letter = str.length() - 1;
  while(letter) {
	  tmp += str[letter--];
  }
  key = abs(tmp);
}

int Hash::hash() const{
  return key;
}

