all: sqlParse.a syntaxTree.a clean
	copy libSqlParse.a ..\lib
	copy libSyntaxTree.a ..\lib
	del libSqlParse.a libSyntaxTree.a
sqlParse.a: lex.yy.o y.tab.o
	ar -crv libSqlParse.a lex.yy.o y.tab.o
lex.yy.o: lex.yy.c y.tab.h
	gcc -c lex.yy.c
y.tab.o: y.tab.c
	gcc -c y.tab.c
syntaxTree.a: syntaxTree.o
	ar -crv libSyntaxTree.a syntaxTree.o
syntaxTree.o: syntaxTree.c
	gcc -c syntaxTree.c
lex.yy.c: sqlScan.l
	flex sqlScan.l
y.tab.c y.tab.h: sqlSyntaxTree.y
	bison -d -y -v -t sqlSyntaxTree.y
clean:
	del lex.yy.c lex.yy.o y.tab.h y.tab.c y.tab.o syntaxTree.o
