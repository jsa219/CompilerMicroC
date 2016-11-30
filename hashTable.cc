/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * hashTable.cc
 */
 
 #include "hashTable.h"

HashTable::HashTable(int n):size(n){
  //check(n > 2 && prime(n),
	//"(HashTable()) Need positive prime number for table size");
  table = new Link*[n];
  check(table != NULL,"(HashTable(int)) Heap overflow");
  for(int j = 0; j < size; j++)
    table[j] = NULL;
}

HashTable::HashTable(const HashTable& t):size(t.size){
  table = new Link*[size];
  check(table != NULL, "(HashTable(HashTable)) Heap overflow");
  for(int j = 0; j < size; j++)
    table[j] = t.table[j];
}

HashTable::~HashTable(){
  for(int j = 0; j < size; j++)
    if(table[j] != NULL)
      delete table[j];
  delete [] table;
}

HashTable& HashTable::add(const string str, string d){
  int h;
  check(!inTable(str),"(add()) Duplicate key");
  h = Hash(str).hash() % size;
  table[h] = new Link(str, d, table[h]);
  check(table[h] != NULL, "(add()) Heap overflow");
  return *this;
}

bool HashTable::inTable(const string k) const{
  int h;
  Link *temp;
  h = Hash(k).hash() % size;
  temp = table[h];
  while(temp != NULL && temp->key != k)//strcmp(temp->key,k) != 0)
    temp = temp->next;
  return temp != NULL;
}

string HashTable::operator [](const string str){
	int h;
	Link *temp;
    h = Hash(str).hash() % size;
    temp = table[h];
    while(temp != NULL && temp->key != str){
		temp = temp->next;
	}
	return temp->data;
}

void HashTable::check(bool b, const string mess){
  if(!b){
    cerr << "ERROR[HashTable]: " << mess << endl;
    exit(1);
  }
}
