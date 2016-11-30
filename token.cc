/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * token.cc
 */
#include <iostream>
#include "token.h"

using namespace std;

token::token(int type, string lexme, int line, int pos) {
  this->type = type;
  this->lexme = lexme;
  this->line = line;
  this->pos = pos;
}


token::~token() {
}



  //skip spaces
  //save pos and line values
  //determine type of token based on first character

  //if first char is a letter, IDENT or Keyword
    //Read until non-letter or digit

  //if first char is a digit, INTLIT

  //if first char is a special character(+, -, ....) operator or punctuation

  //if first char is a double quote(") STRINGLIT

  //if first char is a #, ENDOFFILE

  //all other cases, ERROR
  







//Getters

int token::getType() {
  return this->type;
}

string token::getLexme() {
  return this->lexme;
}

int token::getLine() {
  return this->line;
}

int token::getPos() {
  return this->pos;
}
