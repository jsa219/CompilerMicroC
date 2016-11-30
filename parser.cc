/*
 * CSE 109
 * Jordan Alam
 * jsa219
 * parser.cc
 */
 
 #include "parser.h"

const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV",
                      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",
                      "AND", "OR",
                      "PUSHL", "PUSHV", "STORE",
                      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",
                      "PRINTF",
                      "LABEL", "SEQ",
                      "FUNC", "PARAM1", "PARAM2", "PARAM3", "PARAM4", "PARAM5" };
                      
Parser::Parser(lexer& Lexerx, ostream& outx): Lexer(Lexerx), out(outx), lindex(1), tindex(1) {
  Token = Lexer.nextToken();
  table = SymbolTable();
}

Parser::~Parser() { // DefaultDestructor
};

void Parser::error(string message) { // error(message)
  cerr << message << " Found " << Token.getLexme() << " at line " << Token.getLine() << " position " << Token.getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) { //check(tokenType, message)
  if (Token.getType() != tokenType)
    error(message);
}

Parser::TreeNode* Parser::factor() {
	TreeNode* node = new TreeNode(SEQ);
	
	std::string temp = Token.getLexme();


	switch(Token.getType()) {
		case token::LPAREN:
			Token = Lexer.nextToken();		// skip the LPAREN
			node = expression();
			check(token::RPAREN, "No matching ) for (");
			Token = Lexer.nextToken();
			
			break;
		
		case token::INTLIT:
			node = new TreeNode(Parser::PUSHL, Token.getLexme());	// node with PUSHL INTLIT
			Token = Lexer.nextToken();		//grab next token
			
			break;
			
		case token::IDENT:

			Token = Lexer.nextToken();
			if(Token.getType() == token::LPAREN) {
				Token = Lexer.nextToken();
				if(Token.getType() == token::RPAREN) {
					return new TreeNode(CALL, temp);
				}else{
					node = expression();
					while(Token.getType() == token::COMMA) {
						Token = Lexer.nextToken();
						node = new TreeNode(SEQ, node, expression());
					}
					check(token::RPAREN, "missing ) for (");
					Token = Lexer.nextToken();
				
					node = new TreeNode(SEQ, node, new TreeNode(CALL, temp));
				}
			}
			else {
				
				node = new TreeNode(PUSHV, table.getUniqueSymbol(temp));
			}
			break;
			
		default:
			error("Invalid factor");
				break;
		}
	return node;
	
}

  Parser::TreeNode* Parser::term() {
	  TreeNode* termNode = factor();
	  TreeNode* factorNode;
	  int type = Token.getType();
	  
	  while(type == token::TIMES || type == token::DIVIDE) {
		  Token = Lexer.nextToken();
		  factorNode = factor();
		  Operation op;
		  switch(type) {
			  case token::TIMES:
				op = MULT;
				break;
			  case token::DIVIDE:
				op = DIV;
				break;
			}
			termNode = new TreeNode(op, termNode, factorNode);
			type = Token.getType();
	  } 
  return termNode;
}

Parser::TreeNode* Parser::expression() {
	
	TreeNode* expressionNode = term();
	TreeNode* termNode;
	int tokenType = Token.getType();
	
  while(tokenType == token::PLUS || tokenType == token::MINUS) {
    Token = Lexer.nextToken();
    termNode = term();
    
    Operation op;
    
    if(tokenType == token::PLUS) {
		op = ADD;
	} else {
		op = SUB;
	}
	expressionNode = new TreeNode(op, expressionNode, termNode);
	tokenType = Token.getType();
  }
  return expressionNode;
}

Parser::TreeNode* Parser::relationalExpression() {
	
	TreeNode* relExp = expression();
	TreeNode* expressionNode;
	
	int type = Token.getType();
	Operation op;
	
	switch(type){
		case token::EQ:
			op = ISEQ;
			break;
		case token::LT:
			op = ISLT;
			break;
		case token::LE:
			op = ISLE;
			break;
		case token::GT:
			op = ISGT;
			break;
		case token::GE:
			op = ISGE;
			break;
		case token::NE:
			op = ISNE;
			break;
		default:
		return relExp;
	}
	Token = Lexer.nextToken();
	expressionNode = expression();
	
	relExp = new TreeNode(op, relExp, expressionNode);
	return relExp;
}

Parser::TreeNode* Parser::logicalExpression() {
	
	TreeNode* logExp = relationalExpression();
	TreeNode* relExp;
	int type = Token.getType();
	
	while(type == token::AND || type == token::OR) {
		Token = Lexer.nextToken();
		relExp = relationalExpression();
		Operation op;
		
		if(type == token::AND) {
			op = AND;
		} else {
			op = OR;
		}
		logExp = new TreeNode(op, logExp, relExp);
		type = Token.getType();
	}
	return logExp;
}

Parser::TreeNode* Parser::assignmentStatement() {
	
	check(token::IDENT, "missing IDENT");
	Operation op;
	op = STORE;
	
	string hashedString = table.getUniqueSymbol(Token.getLexme());
	if(hashedString == "") {
		error("undefined variable");
	}
	
	TreeNode* identifier = new TreeNode(op, hashedString);

	Token = Lexer.nextToken();
	
	check(token::ASSIGN, "missing =");
	
	Token = Lexer.nextToken();
	
	TreeNode* logExp = logicalExpression();
	
	op = SEQ;
	TreeNode* seq = new TreeNode(op, logExp, identifier);
	
	check(token::SEMICOLON, "missing a semicolon");
	Token = Lexer.nextToken();
	
	return seq;	
}

Parser::TreeNode* Parser::printfStatement() {

  TreeNode* paramList = NULL;
  int nparams = 0;
  check(token::PRINTF, "Expecting printf");
  Token = Lexer.nextToken();
  check(token::LPAREN, "Expecting (");
  Token = Lexer.nextToken();
  check(token::STRINGLIT, "Expecting string literal");
  string formatString = Token.getLexme();
  Token = Lexer.nextToken();
  if (Token.getType() == token::COMMA) {
    Token = Lexer.nextToken();
    paramList = expression();
    ++nparams;
    while (Token.getType() == token::COMMA) {
      Token = Lexer.nextToken();
      paramList = new TreeNode(SEQ, paramList, expression());
      ++nparams;
    }
  }

  check(token::RPAREN, "Expecting )");

  Token = Lexer.nextToken();
  check(token::SEMICOLON, "Expecting ;");
  Token = Lexer.nextToken();
  TreeNode* printStatement =
	new TreeNode(SEQ, paramList, new TreeNode(PRINTF, itos(nparams) + formatString));
    
  return printStatement;
}	

Parser::TreeNode* Parser::whileStatement() {
	
	check(token::WHILE, "no while statement");
	Token = Lexer.nextToken();
	
	check(token::LPAREN, "missing (");
	Token = Lexer.nextToken();
	
	string labOne = makeLabelNoColon();
	
	TreeNode* current = new TreeNode(LABEL, labOne + ":");
	
	current = new TreeNode(SEQ, current, logicalExpression());
	
	check(token::RPAREN, "Missing )");
	Token = Lexer.nextToken();
	
	
	string labTwo = makeLabelNoColon();
	
	current = new TreeNode(SEQ, current, new TreeNode(JUMPF, labTwo));
	
	table.enterScope();
	current = new TreeNode(SEQ, current, block());
	table.exitScope();
	current = new TreeNode(SEQ, current, new TreeNode(JUMP, labOne));
	
	current = new TreeNode(SEQ, current, new TreeNode(LABEL, labTwo + ":"));
	
	return current;
	
}

Parser::TreeNode* Parser::ifStatement() {

	check(token::IF, "no if statement");	// check if statement and skip token
	Token = Lexer.nextToken();
	
	check(token::LPAREN, "missing (");
	Token = Lexer.nextToken();
	
	TreeNode* lExp = logicalExpression();
	
	check(token::RPAREN, "missing )");
	Token = Lexer.nextToken();
	
	table.enterScope();
	TreeNode* thenBlock = block();
	table.exitScope();
	
	string labOne = makeLabelNoColon();
	lExp = new TreeNode(SEQ, lExp, new TreeNode(JUMPF, labOne));
	lExp = new TreeNode(SEQ, lExp, thenBlock); // SEQ node with the if block
	
	if(Token.getType() == token::ELSE) { // if the statement is an if...else
		
		string labTwo = makeLabelNoColon();
		lExp = new TreeNode(SEQ, lExp, new TreeNode(JUMP, labTwo));
		lExp = new TreeNode(SEQ, lExp, new TreeNode(LABEL, labOne + ":"));
		
		Token = Lexer.nextToken();	// get off of the else token
		
		table.enterScope();
		lExp = new TreeNode(SEQ, lExp, block());
		table.exitScope();
		
		lExp = new TreeNode(SEQ, lExp, new TreeNode(LABEL, labTwo + ":"));
	} else {
		lExp = new TreeNode(SEQ, lExp, new TreeNode(LABEL, labOne + ":")); // if the statement is if(){}
	}
	return lExp;
}

Parser::TreeNode* Parser::returnStatement() {
	
	check(token::RETURN, "missing return statement");
	Token = Lexer.nextToken();
	
	TreeNode* retNode = new TreeNode(SEQ, logicalExpression(), new TreeNode(RET));
	check(token::SEMICOLON, "missing ;");
	Token = Lexer.nextToken();
	
	return retNode;
}

Parser::TreeNode* Parser::statement() {
	
	TreeNode* statementNode;
	
	if(Token.getType() == token::VAR) {
		statementNode = vardefStatement();
	} else
	if(Token.getType() == token::RETURN) {
		statementNode = returnStatement();
	} else
	if(Token.getType() == token::IDENT) {
		statementNode = assignmentStatement();
	}else 
	if(Token.getType() == token::WHILE) {
		statementNode = whileStatement();
	}else 
	if(Token.getType() == token::IF) {
		statementNode = ifStatement();
	}else
	if(Token.getType() == token::PRINTF) {
		statementNode = printfStatement();
	}	else {
		error("Invalid statement");
	}
	return statementNode;
	
}

Parser::TreeNode* Parser::block() {
	
	check(token::LBRACE, "{ missing");
	
	Token = Lexer.nextToken();
	
	
	if(Token.getType() == token::RBRACE) {
		Token = Lexer.nextToken();
		
		return new TreeNode(SEQ);
	} else {
		TreeNode* blockN = statement();
		while(Token.getType() != token::RBRACE) {
			blockN = new TreeNode(SEQ, blockN, statement());
		}
		check(token::RBRACE, "missing }");
		Token = Lexer.nextToken();
		
		return blockN;
	}

}

Parser::TreeNode* Parser::function() {
	
	check(token::FUNCTION, "no function");
	Token = Lexer.nextToken();	
	
	check(token::IDENT, "no identifier"); // function name
	
	TreeNode* funName = new TreeNode(FUNC, Token.getLexme() + ":");
	Token = Lexer.nextToken();
	
	check(token::LPAREN, "missing (");
	Token = Lexer.nextToken();

	table.enterScope();
	if(Token.getType() == token::IDENT) { // if there's a parameter
		if(!table.addSymbol(Token.getLexme())){
			// first parameter name is already defined
			error("the function name is already defined"); // because the param shouldn't yet be defined
		}
		else {
			// parameter name is not yet defined

			int c = 0;

			string *params = new string[1024];
			params[c] = table.getUniqueSymbol(Token.getLexme()); // enter first param into the array
			if(params[c] == ""){
				error("name not defined");
			}
			c++;
						
			Token = Lexer.nextToken();	// go to token after first param name
			
			while(Token.getType() == token::COMMA) { // check that it's a comma
				Token = Lexer.nextToken();	// skip the comma
				if(Token.getType() != token::IDENT) {	// make sure next is an ident
					error("invalid paramater");	
				}else {
					if(!table.addSymbol(Token.getLexme())){
						// parameter name is already defined
						error("the function name is already defined"); // because the param shouldn't yet be defined
					} else {					
						params[c] = table.getUniqueSymbol(Token.getLexme());
						c++;
						Token = Lexer.nextToken();	// skip the next identifier
					}
				}					
			}
			c--;

			while(c >= 0) {
				funName = new TreeNode(SEQ, funName, new TreeNode(STORE, params[c]));
				c--;
			}
		}
	}
	
	check(token::RPAREN, "missing )");
	Token = Lexer.nextToken();
	
	funName = new TreeNode(SEQ, funName, block());

	table.exitScope();

	return funName;
}

Parser::TreeNode* Parser::compilationunit() {
	TreeNode* top = new TreeNode(SEQ);
	if(Token.getType() == token::FUNCTION) {
		top = function();
	}
	while(Token.getType() == token::FUNCTION) {
		TreeNode* funNode = function();
		top = new TreeNode(SEQ, top, funNode);
	}
	return top;
}

Parser::TreeNode* Parser::vardefStatement() {
	
	check(token::VAR, "no var identifier");
	Token = Lexer.nextToken();
	check(token::IDENT, "no identifier after var");
	if(!table.addSymbol(Token.getLexme())) {
		error("symbol already in use");
	}
	Token = Lexer.nextToken();
	while(Token.getType() == token::COMMA) {
		Token = Lexer.nextToken();
		check(token::IDENT, "no identifier after var");
		if(!table.addSymbol(Token.getLexme())) {
			error("symbol already in use");
		}
		Token = Lexer.nextToken();
	}
	check(token::SEMICOLON, "missing ;");
	Token = Lexer.nextToken();
	return new TreeNode(SEQ);
}

void emit(string str) {
	cout << str << "\n";
}

string currentFunc;
int nfmts = 0;
string fmts[100];
	
void Parser::geninst(TreeNode* node) {
	// Generates the instructions for a single node
	if(node != NULL) {
		geninst(node->leftChild);
		geninst(node->rightChild);
		string j1, j2;
		
		switch(node->op) {	
			case SEQ:
				break;

			case ADD:
				emit("  pop rbx");
				emit("  pop rax");
				emit("  add rax,rbx");
				emit("  push rax");
				break;

			case SUB:
				emit("  pop rbx");
				emit("  pop rax");
				emit("  sub rax,rbx");
				emit("  push rax");
				break;

			case AND:
				emit("  pop rbx");
				emit("  pop rax");
				emit("  and rax,rbx");
				emit("  push rax");
				break;

			case OR:
				emit("  pop rbx");
				emit("  pop rax");
				emit("  or rax,rbx");
				emit("  push rax");
				break;
	
			case MULT:
				emit("  pop rbx");
				emit("  pop rax");
				emit("  imul rbx");
				emit("  push rax");
				break;
				
			case DIV:
				emit("  mov rdx,0");
				emit("  pop rbx");
				emit("  pop rax");
				emit("  idiv rbx");
				emit("  push rax");
				break;
				
			case LABEL:
				emit(node->val);
				break;
				
			case ISEQ:
				ISEQvariations("je");
							
				break;
				
			case ISNE:

				ISEQvariations("jne");
	
				
				break;		
				
			case ISLT:
			
				ISEQvariations("jl");			
			
				break;		

				
			case ISLE:
			
				ISEQvariations("jle");
			
				break;		
				
				
			case ISGT:
			
				ISEQvariations("jg");
	

				break;		

								
			case ISGE:
				
				ISEQvariations("jge");
		

				break;		

			
			case JUMP:
				emit("  jmp " + node->val);
				
				break;
				
			case JUMPF:
				emit("  pop rax");
				emit("  cmp rax,0");
				emit("  je " + node->val);
				
				break;
				
			case JUMPT:
				emit("  pop rax");
				emit("  cmp rax,0");
				emit("  jne x");
				
				break;
				
			case PUSHL:
				emit("  mov rax," + node->val);
				emit("  push rax");
				
				break;
				
			case PUSHV:
			        emit("  push qword[" + node->val + "]");
				
				break;
				
			case CALL:
				emit("  call " + node->val);
				emit("  push rax");
				
				break;
				
			case FUNC:
				currentFunc = node->val;
				emit(currentFunc);
				if(currentFunc != "main:") {
					emit("  pop r15");
				}
				else {
				}
				break;
				
			case STORE:
				emit("  pop qword[" + node->val + "]");
				//cout << "STORE EMIT" << endl;
				
				break;	
				
			case RET:
			        emit("  pop rax");
				if(currentFunc != "main:") {
				  emit("  push r15");
				}
				emit("  ret");
				if(currentFunc == "main:") {
					emit("  mov rax,1");
					emit("  int 0x80");
				}
				
				
				break;	
			
			case PRINTF:
			{	string fmt = node->val;
				int nparams = fmt.at(0) - '0';	// gives an int with the number of params
				fmt = "`" + fmt.substr(1) + "`";	// changes value of fmt to remove nparams from the start of it
								// And wrap it in backticks
				
				fmts[nfmts++] = fmt;
				emit("  mov    rdi,fmt" + itos(nfmts));	// move into rdi
					// Generates pop instructions to pop values intp proper registers
				if (nparams == 5) {
					emit("  pop   r9");
					--nparams;
				}
				
				if (nparams == 4) {
					emit("  pop   r8");
					--nparams;
				}
				if (nparams == 3) {
					emit("  pop   rcx");
					--nparams;
				}
				if (nparams == 2) {
					emit("  pop   rdx");
					--nparams;
				}
				if (nparams == 1) {
					emit("  pop   rsi");
				}
				emit("  mov    rax,0");
				emit("  push   rbp");
				emit("  call   printf");
				emit("  pop    rbp");
						
			}
				break;
			
			
			default:
				cout << "miss " << ops[node->op] << "  " <<  node->val << "  " << endl;
//				error("really bad");
				
				
			}
		}
		
	
}

void Parser::ISEQvariations(string str) {
				
				string j1 = makeLabelNoColon();
				string j2 = makeLabelNoColon();
				
				emit("  pop rbx");
				emit("  pop rax");
				emit("  cmp rax,rbx");
				emit("  " + str + " " + j1);
				emit("  mov rax,0");
				emit("  jmp " + j2);
				emit(j1 + ":");
				emit("  mov rax,1");
				emit(j2 + ":");
				emit("  push rax");
}

void Parser::vardefs(TreeNode *node) {
	
	
	if(node != NULL) {
		vardefs(node->leftChild);
		vardefs(node->rightChild);
		
		if(node->op == PUSHV || node->op == STORE) {
			int dup = 0;	// if the variable is a duplicate
			for(int i = 0; i < count; i++) {	// Checks array for possible duplicates
				if(node->val == vars[i]) {
					dup = 1;	// if there is a dup anywhere in array, don't write it again, so don't add it to string
				}
			}		
			if(!dup) {	// as long as there are no dups, add to array
				
/*				if(count >= (int)(sizeof(vars)/sizeof(vars[0]))) {
					string* temp = new string[2 * (int)(sizeof(vars)/sizeof(vars[0]))];
					for(int j = 0; j < count; j++) {
						temp[j] = vars[j];
						vars = temp;
					}
				}
	*/
		//This commented out section would enable more efficient use of memory, however it segfaults currently.
		//It would allow the use of a smaller array which would double when necessary
		//Otherwise, the size of vars is set to 1024, allocating space enough for 1024 variables.
		
		
				vars[count] = node->val;
				count++;
			}
			dup = 0;	// reset boolean duplicates
		}
	}
	
}

void Parser::genasm(TreeNode *node) {
	
	emit("  global main");
	emit("  extern printf");
	
	cout << "  section .bss" << endl;
	
	count = 0;
	vars = new string[1024];	// 1024 is the space allocated for var names. If over 1024 var names in the program, there will be an error. Also, the programmer is insane.
	vardefs(node);	// sets the string array vars to hold all variables
	int i = 0;		
	while(vars[i] != "" ) {	// traverses the array and prints out variables
		cout << "  " << vars[i] << " resq 1" << endl;
		i++;
	}
		
	emit("  section .text");
	geninst(node);
	
	 cout << endl << "  section .data" << endl; // prints data section header
	 for (int i=0; i < nfmts; ++i) {	// prints data section
		 cout << "  fmt" << i+1 << ": db " << fmts[i] << ", 0" << endl;
  }
}
