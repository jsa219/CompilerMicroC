/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * symbolTable.h
 */
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "hashTable.h"
#include "stack.h"
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sstream>
#include "hash.h"
#include "link.h"


using namespace std;

class SymbolTable {

	public:


		SymbolTable();
		~SymbolTable();
		
		void enterScope(); // pushes new hash table to the stack
		void exitScope(); // pops top hashtable off the stack

		int addSymbol(string sym); // creates a unique symbol based on sym (x ==> x$1) and stores it with original symbol in hash table
			// returns true or false (1 or 0) if successful or not
			// if another var is defined in the same scope with the same name, 0 is returned and an error has occured

		string getUniqueSymbol(string sym);	// returns first symbol in scope if it can be found
								// Lookup x, get x$1
			//empty symbol for new strng
private:
	Stack stack;
	int counter;
};


// take Scack code and use it or create code from scratch

#endif
