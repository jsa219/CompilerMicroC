/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * microc.cc
 */
 
 #include "token.h"
#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <fstream>

using namespace std;

void processFile(istream& in) {
	lexer Lexer(in);
	token Token;
	Parser parser(Lexer, cout);
	Parser::TreeNode* program = parser.compilationunit();
	//cout << Parser::TreeNode::toString(program) << endl << endl; // prints tree
	parser.genasm(program);
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
	return 0;
}
