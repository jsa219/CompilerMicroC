/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * stack.cc
 */
 #include <cstdlib>
#include <stdio.h>
#include "stack.h"


Stack::Stack(int n):size(n),tos(Stack::EMPTY) {
  stack = new HashTable*();
}

Stack::Stack():size(10),tos(Stack::EMPTY) {
  stack = new HashTable*[size];
}

Stack::Stack(const Stack& s)  { // Copy constructor
  size = s.size;
  tos = s.tos;
  stack = new HashTable*[size];
  for (int i = 0; i < size; ++i) {
    stack[i] = s.stack[i];
  }
}


Stack::~Stack() {
  /*if (stack != NULL) {
      delete [] stack;
  }*/
}

void Stack::push(HashTable* val) {
  if (tos >= size - 1) {
    cerr << "Stack overflow\n";
    exit(1);
  } else {
    stack[++tos] = val;
  }
}

HashTable* Stack::pop() {
  if (tos < 0) {
    cerr << "Stack underflow\n";
    exit(1);
  } else {
    return stack[tos--];
  }
}

HashTable* Stack::peek() const {
  if (tos < 0) {
    cerr << "Peek on empty stack!\n";
    exit(1);
  } else {
    return stack[tos];
  }
}

/*ostream& operator <<(ostream& out, const Stack& s){
  out << "[ ";
  //operator<<(out, "[ ");
  for (int i=0; i <= s.tos; i++) {
    out << s.stack[i] << " ";
  }
  out << "]";  
  return out;
}*/

void Stack::checkIndex(int i) const {
  if (i < 0 || i > tos) {
    cerr << "Index out of bounds\n";
    exit(1);
  }  
}

/*Stack s;
s[2] // === s.operator[](2)

s[2] = 4;
*/
HashTable* Stack::operator [](int i){
  //cout << "lvalue []\n";
  checkIndex(i);
  return stack[i];
}
/*
x = s[2];
const int& Stack::operator [](int i) const {
  cout << "rvalue []\n";  
  checkIndex(i);  
  return stack[i];
}
*/
