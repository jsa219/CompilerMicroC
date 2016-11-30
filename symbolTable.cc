/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * symbolTable.cc
 */
 
#include <iostream>
#include <ctype.h>
#include "symbolTable.h"

SymbolTable::SymbolTable() {
	stack = Stack();
	counter = 0;	
}

SymbolTable::~SymbolTable() {
	//delete(&stack);
}

void SymbolTable::enterScope() {
	stack.push(new HashTable());
}


void SymbolTable::exitScope() {
	stack.pop();
}

int SymbolTable::addSymbol(string sym) {	// creates unique symbol based on symm
	
	HashTable* tab = stack.peek();

	if(tab->inTable(sym)) {
		return 0;
	}	
	else {		// var not yet in hashTable
		stringstream ss;
		ss << sym << "$" << counter;
		tab->add(sym, ss.str());
		counter++;
	}
	//return true or false
	return 1;
}

string SymbolTable::getUniqueSymbol(string sym) {
	for(int i = stack.getTos(); i >= 0; i--) {
		if(stack[i]->inTable(sym)) {
			return (*(stack[i]))[sym];
		}
	}
	return "";
}

