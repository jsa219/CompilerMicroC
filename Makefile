OPTS = -g -c -Wall -Werror

microc: microc.o lexer.o token.o parser.o symbolTable.o hashTable.o stack.o link.o hash.o
	g++ -o microc microc.o lexer.o token.o parser.o symbolTable.o hashTable.o stack.o link.o hash.o

lextest: lextest.o lexer.o token.o
	g++ -o lextest lextest.o lexer.o token.o

token.o: token.cc token.h
	g++ $(OPTS) token.cc

parser.o: parser.cc parser.h token.h symbolTable.h
	g++ $(OPTS) parser.cc

microc.o: microc.cc lexer.h token.h parser.h
	g++ $(OPTS) microc.cc

lexer.o: lexer.cc lexer.h token.h
	g++ $(OPTS) lexer.cc


lextest.o: lextest.cc token.h lexer.h
	g++ $(OPTS) lextest.cc

hashTable.o: hashTable.h hashTable.cc link.h hash.h
	g++ $(OPTS) hashTable.cc

symbolTable.o: symbolTable.h symbolTable.cc stack.h hashTable.h link.h
	g++ $(OPTS) symbolTable.cc

stack.o: stack.h stack.cc hashTable.h
	g++ $(OPTS) stack.cc

link.o: link.h link.cc
	g++ $(OPTS) link.cc

hash.o: hash.h hash.cc
	g++ $(OPTS) hash.cc


clean:
	rm -f lextest microc *.o *~ 
