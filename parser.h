/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * parser.h
 */
#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "lexer.h"
#include <iostream>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <sstream>

#include "symbolTable.h"

using namespace std;

class Parser {
  
private:

  enum Operation {
    ADD, SUB, MULT, DIV, // Arithmetic Operators
    ISEQ, ISNE, ISLT, ISLE, ISGT, ISGE, // Relational Operators
    AND, OR, // Logical Operators
    PUSHL, PUSHV, STORE, // Value Transfer Instructions
    JUMP, JUMPF, JUMPT, CALL, RET, // Location Transfer Instructions
    PRINTF, // Misc
    LABEL, SEQ, // Pseudo Operations
	FUNC, PARAM1, PARAM2, PARAM3, PARAM4, PARAM5 //fun and param
  };
  
public:  
  class TreeNode {
    
  public:

    Operation op;
    string val; // Variable name or jump label
    TreeNode *leftChild;
    TreeNode *rightChild;

    // Utility function used by constructors
    void init(Operation opx, string valx, TreeNode *leftChildx, TreeNode *rightChildx) {
      op = opx;
      val = valx;
      leftChild = leftChildx;
      rightChild = rightChildx;
    }


    TreeNode(Operation op, string val) {
      init(op, val, NULL, NULL);
    }
    
    TreeNode(Operation op, string val, TreeNode *leftChild, TreeNode *rightChild) {
      init(op, val, leftChild, rightChild);
    }

    TreeNode(Operation op) {
      init(op, "", NULL, NULL);
    }

    TreeNode(Operation op, TreeNode *leftChild, TreeNode *rightChild) {
      init(op, "", leftChild, rightChild);
    }

    static string toString(TreeNode *node) {
      return toString0(node, 0);
    }
    
    static string toString0(TreeNode *node, int spaces) {
      static string blanks = "                                        ";
      string left = "";
      string right = "";
      bool isLeaf = true;
      if (node->leftChild != NULL) {
	left = toString0(node->leftChild, spaces+2);
	isLeaf = false;
      }
      if (node->rightChild != NULL) {
	right = toString0(node->rightChild, spaces+2);
	isLeaf = false;	
      }
      string ret;
      if (isLeaf) {
	ret = blanks.substr(0, spaces) + ops[node->op] + "[" + node->val + "]";
      } else {
	ret = blanks.substr(0, spaces) + ops[node->op] + "(\n" + left + ",\n" + right + "\n" + 
	      blanks.substr(0, spaces) + ")";
      }
      return ret;
    }
    
  };
  
private:
  lexer Lexer;
  token Token;
  ostream& out;
  int lindex;
  int tindex;

  SymbolTable table;
  
  string itos(int i) { stringstream ss; ss << i; string res = ss.str(); return res;}

  string makeLabel() { string tmp = "L"; stringstream ss; ss << ++lindex; string res = ss.str(); tmp = tmp + res; return tmp + ':';}	// returns a string called L followed by a number which is incremented after called


  string makeLabelNoColon() { string tmp = "L"; stringstream ss; ss << ++lindex; string res = ss.str(); tmp = tmp + res; return tmp;}	// returns a string called L followed by a number which is incremented after called

  string rmLastChar(string str) {
	string result = "";
	for(int i = 0; i < (int)str.length() - 1; i++) {
		result+=str[i];
	}
	return result;
  }

  static const string ops[];
  void error(string message);
  void check(int tokenType, string message); // check validity. 'if' must be followed by '(' If it's not, error
											 // error message would be "Expecting ("
  
 public:
  TreeNode* factor();
  TreeNode* term();
  TreeNode* expression();
  TreeNode* relationalExpression();
  TreeNode* logicalExpression();
  TreeNode* assignmentExpression();
  TreeNode* returnStatement();
  TreeNode* printfStatement();
  TreeNode* whileStatement();
  TreeNode* ifStatement();
  TreeNode* assignmentStatement();
  TreeNode* vardefStatement();  
  TreeNode* statement();
  TreeNode* block();
  TreeNode* parameterdef();
  TreeNode* parameterdefs();
  TreeNode* function();
  TreeNode* compilationunit();



  
  void geninst(TreeNode *node);
  



  void vardefs(TreeNode *node);
	string* vars;
	int count;
	//Vars needed for this function. vars is an array of strings, and count is for traversing the array.




  void genasm(TreeNode *node);

  void ISEQvariations(string str);
  
    
  Parser(lexer& Lexer, ostream& out);
  ~Parser(); 

};

#endif
