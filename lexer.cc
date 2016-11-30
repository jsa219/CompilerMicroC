/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * lexer.cc
 */

#include <iostream>
#include <ctype.h>
#include "lexer.h"
#include "token.h"

using namespace std;

lexer::lexer(istream& stream):stream(stream) {

  pos = 0;
  line = 0;
  c = nextChar();
}

lexer::~lexer() {
}

char lexer::nextChar() {
	pos++;
	char c = stream.get();
	if(c == ' ' || c == '\t') {
		pos++;
		return ' ';
	}
	if(c == '\n') {
		line++;
		pos = 0;
		return ' ';
	}
	if(c == EOF) {
		return '#';
	}
	return c;
}

token lexer::nextToken() {
	string lex = "";
	int startPos = pos;
	int startLine = line;  
	
	while(c == ' ') {
		c = nextChar();
	}
	
	if(isdigit(c)) {	//INTLIT
		while(isdigit(c)) {
			lex+=c;
			c = nextChar();
		}
		return token(token::INTLIT, lex, startLine, startPos);
	}
	
	if(c == '"') {	//STRINGLIT
		c = nextChar();	 

		while(c != '"') {
			if(c == EOF) {
				lex+=c;
				return token(token::ERROR, lex, startLine, startPos);
			}
			lex+=c;
			c = nextChar();
		}//Ends when c == "   ( a double quotation mark
		c = nextChar();

		if(c == '"') { // if there are 3 """ in a row, error
			lex+=c;
			return token(token::ERROR, lex, startLine, startPos);
		} else {
			return token(token::STRINGLIT, lex, startLine, startPos);
			}

	}
	
	if(c == '#') {	// EOF
		lex+=c;
		return token(token::ENDOFFILE, lex, startLine, startPos);
	}

	if(isalpha(c)) { //Identifiers and KEYWORDS
		lex+=c;
		c=nextChar();
		while(isalnum(c)) {
			lex+=c;
			c=nextChar();
		}
		if(lex == "if") {
			return token(token::IF, lex, startLine, startPos);
		}
		if(lex == "else") {
			return token(token::ELSE, lex, startLine, startPos);
		}
		if(lex == "while") {
			return token(token::WHILE, lex, startLine, startPos);
		}
		if(lex == "function") {
			return token(token::FUNCTION, lex, startLine, startPos);
		}
		if(lex == "var") {
			return token(token::VAR, lex, startLine, startPos);
		}
		if(lex == "printf") {
			return token(token::PRINTF, lex, startLine, startPos);
		}
		if(lex == "return") {
			return token(token::RETURN, lex, startLine, startPos);
		} else {
			return token(token::IDENT, lex, startLine, startPos); // variable name
		}
	}

	if(!isalnum(c)) { // Operator or punctuation
		if(c == '=') {
			lex+=c;
			c = nextChar();
			if(c == '=') {
				lex+=c;
				c = nextChar();
				if(isalnum(c) || c == '(' || ' ') {
					return token(token::EQ, lex, startLine, startPos);
				} else {
					lex+=c;
					return token(token::ERROR, lex, startLine, startPos);
				}
			}
			if(isalnum(c) || c == '(' || c == ' ') {
				return token(token::ASSIGN, lex, startLine, startPos);
			} else {
				lex+=c;
				return token(token::ERROR, lex, startLine, startPos);
			}
			
		}

		if(c == '<' || c == '>') {
			lex+=c;
			c = nextChar();
			if(c == '=') {
			  lex+=c;
			  if(lex == "<=") {
				  c = nextChar();
				  if(isalnum(c) || c == ' '){  
					return token(token::LE, lex, startLine, startPos);
				} else {
					lex+=nextChar();
					return token(token::ERROR, lex, startLine, startPos);
				}
			} 
			  if(lex == ">=") {
				  c = nextChar();
				  if(isalnum(c) || c == ' '){  
					return token(token::GE, lex, startLine, startPos);
				} else {
					lex+=nextChar();
					return token(token::ERROR, lex, startLine, startPos);
				}
			}
		}
		 if(isalnum(c) || c == ' ') {
			 if(lex == "<") {
				 return token(token::LT, lex, startLine, startPos);
			 } else {
			    return token(token::GT, lex, startLine, startPos);
			  }
			}else {
			  lex+=c;
			  return token(token::ERROR, lex, startLine, startPos);
			}
		}
		
		if(c == '!') {
		  lex+=c;
		  c = nextChar();
		  if(c == '=') {
		    lex+=c;
		    c = nextChar();
		    return token(token::NE, lex, startLine, startPos);
		  }else {
		    lex+=c;
		    c = nextChar();
		    return token(token::ERROR, lex, startLine, startPos);
		  }
		}
		
		if(c == '&') {
		  lex+=c;
		  c = nextChar();
		  if(c == '&') {
		    lex+=c;
		    c = nextChar();
		    return token(token::AND, lex, startLine, startPos);
		  }else {
		    lex+=c;
		    c = nextChar();
		    return token(token::ERROR, lex, startLine, startPos);
		  }
		}
		
		if(c == '|') {
		  lex+=c;
		  c = nextChar();
		  if(c == '|') {
		    lex+=c;
		    c = nextChar();
		    return token(token::OR, lex, startLine, startPos);
		  }else {
		    lex+=c;
		    c = nextChar();
		    return token(token::ERROR, lex, startLine, startPos);
		  }
		}

		switch(c) {
		case('+'): //PLUS
		  lex+=c;
		  c = nextChar();
		  if(!isalnum(c) && !(c == '(') && !(c == ' ')){
		    lex+=c;
		    c = nextChar();
		    return token(token::ERROR, lex, startLine, startPos);
		  }else {
		    return token(token::PLUS, lex, startLine, startPos);
		  }
		    
		case('-'): //MiNUS
		  lex+=c;
		  c = nextChar();
		  if(!isalnum(c) && !(c == '(') && !(c == ' ')){
		    lex+=c;
		    c = nextChar();
		    return token(token::ERROR, lex, startLine, startPos);
		  }else {
		    return token(token::MINUS, lex, startLine, startPos);
		  }
		  
		case('*'): //TIMES
		  lex+=c;
		  c = nextChar();
		  if(!isalnum(c) && !(c == '(')  && !(c == ' ')){
		    lex+=c;
		    c = nextChar();
		    return token(token::ERROR, lex, startLine, startPos);
		  }else {
		    return token(token::TIMES, lex, startLine, startPos);
			}
		  
		case('/'): //DIVIDE
		  lex+=c;
		  c = nextChar();
		  if(!isalnum(c) && !(c == '(')  && !(c == ' ')){
		    lex+=c;
		    c = nextChar();
		    return token(token::ERROR, lex, startLine, startPos);
		  }else {
		    return token(token::DIVIDE, lex, startLine, startPos);
		  }
		  
		case('('): // LPAREN
		  lex+=c;
		  c = nextChar();
		  if(!isalnum(c) && !(c == '(') && !(c == ')')  && !(c == ' ') && !(c == '"')){
		    lex+=c;
		    return token(token::ERROR, lex, startLine, startPos);
		  }else {
		    return token(token::LPAREN, lex, startLine, startPos);
		  }
		  
		case(')'): // RPAREN
		  lex+=c;
		  c = nextChar();
		  if(!isalnum(c) && !(c == ';') && !(c == ')')  && !(c == ' ') && !(c == '{')){
		    lex+=c;
		    return token(token::ERROR, lex, startLine, startPos);
		  }else {
		    return token(token::RPAREN, lex, startLine, startPos);
		  }
		  
		case('{'): // LBRACE
		  lex+=c;
		  c = nextChar();
		  if(isalnum(c) || (c == ' ') || (c == '}')) {
		    return token(token::LBRACE, lex, startLine, startPos);
		  }else {
		    lex+=c;
		    return token(token::ERROR, lex, startLine, startPos);
		  }
		  
		case('}'): // RBRACE
		  lex+=c;
		  c = nextChar();
		    if(isalnum(c) || (c == ' ' || c == '{')) {
		    return token(token::RBRACE, lex, startLine, startPos);
		  }else {
		    lex+=c;
		    return token(token::ERROR, lex, startLine, startPos);
		  }
		  
		case(','): // COMMA
		  lex+=c;
		  c = nextChar();
		  if(isalnum(c) || c == ' ') {
		    return token(token::COMMA, lex, startLine, startPos);
		  }else {
		    return token(token::ERROR, lex, startLine, startPos);
		  }
		  
		case(';'): // SEMICOLON
		  lex+=c;
		  c = nextChar();
		  if(isalnum(c) || c == ' ' || c == '}') {
		    return token(token::SEMICOLON, lex, startLine, startPos);
		  }else {
		    return token(token::ERROR, lex, startLine, startPos);
		  }
		  default:
			lex+=c;
			c = nextChar();
			return token(token::ERROR, lex, startLine, startPos);
		} // end switch
	}//end when(!isalnum)			
	lex+=c;
	return token(token::ERROR, lex, startLine, startPos);
} // end nextToken()


  
    
