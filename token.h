/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * token.h
 */
#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
using namespace std;

class token {

public:


  token(int type, string lexme, int line, int pos);

  token() {
  this->type = 0;
  this->lexme = "";
  this->line = 0;
  this->pos = 0;
	};


  ~token();

  int getType();
  string getLexme();
  int getLine();
  int getPos();

  //token nextToken();
  



  //Token types
  static const int IDENT = 1; // identifier
  static const int INTLIT = 2; // int literal
  static const int STRINGLIT = 3; // string literal
  // Operators
  static const int PLUS = 4;
  static const int MINUS = 5;
  static const int TIMES = 6;
  static const int DIVIDE = 7;
  static const int ASSIGN = 8;
  static const int EQ = 9;
  static const int NE = 10;
  static const int LT = 11;
  static const int LE = 12;
  static const int GT = 13;
  static const int GE = 14;
  static const int AND = 15;
  static const int OR = 16;
  // Punctuation
  static const int LPAREN = 17;
  static const int RPAREN = 18;
  static const int LBRACE = 19;
  static const int RBRACE = 20;
  static const int COMMA = 21;
  static const int SEMICOLON = 22;
  // Keywords
  static const int IF = 23;
  static const int ELSE = 24;
  static const int WHILE = 25;
  static const int FUNCTION = 26;
  static const int VAR = 27;
  static const int PRINTF = 28;
  static const int RETURN = 29;
  // Special
  static const int ENDOFFILE = 30;
  static const int ERROR = 31;
  


  
private:
  
  int type; // Type of token
  string lexme; // actual string that was read
  int line; // line number of the string
  int pos; // position in the line of the first character in the string

  
  
};
#endif
