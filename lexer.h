/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * lexer.h
 */
#ifndef LEXER_H
#define LEXER_H

#include <cctype>
#include <iostream>
#include "token.h"
#include <fstream>

using namespace std;

class lexer {

public:
  
  lexer(istream& stream);
  ~lexer();
  
  istream& stream;
  token nextToken();
  
private:
  char nextChar();
  char c; // Character that has been read in.
  int line;
  int pos;
};

#endif
