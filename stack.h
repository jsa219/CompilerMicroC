/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * stack.h
 */
#ifndef STACK_H
#define STACK_H
#include "hashTable.h"
#include <iostream>
using namespace std;

/* 
   A simple implementation of a stack for ints.
*/

// This is a comment

class Stack {
public:
  // Constructors
  Stack(int n);
  Stack(); // Default constructor
  Stack(const Stack& s); // Copy Constructor - deep copy

  // Destructor
  ~Stack();

  // Regular Member Functions
  void push(HashTable* val);
  HashTable* pop();
  HashTable* peek() const;

  int getTos() { return tos; };

  // Inline Member Function
  bool isEmpty() const { return tos == EMPTY; } // inline member function

  // Overloaded Operator Friend Functions
  //friend ostream& operator<<(ostream& out, const Stack& s);

  // Overloaded Operator Member Functions
  //const HashTable& operator[](int i) const; // rvalue

  HashTable* operator[](int i); // lvalue  s1[3]  =>  s1.operator[](3);
  
private:
  HashTable** stack;
  int size;
  int tos;
  static const int EMPTY = -1;
  void checkIndex(int i) const;
};

#endif

