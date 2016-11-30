/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * lexerTest.cc
 */

#include "token.h"
#include "lexer.h"
#include <iostream>
#include <fstream>

using namespace std;

void processFile(istream& in) {
  lexer lexer(in);
  token Token = token(1, "1", 1, 1);
  
  Token = lexer.nextToken();
  printf(" Type     Lexeme    Line #    Pos\n");
  while (Token.getType() != token::ENDOFFILE) {
    printf("%5d     %-10s %5d  %5d\n", Token.getType(), Token.getLexme().c_str(), Token.getLine(), Token.getPos());
    Token = lexer.nextToken();
  }
}


int main(int argc, char **argv) {
  ifstream in;
  
  if (argc > 1) {
    in.open(argv[1]);
    processFile(in);
    in.close();
  } else {
    processFile(cin);
  }
  
}
