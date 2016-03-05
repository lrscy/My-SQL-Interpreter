all: table sqlExec sqlTrans sqlTest
	g++ -std=c++11 -O2 -o sqlTest ..\common\table.o ..\sqlExec\sqlExec.o ..\sqlTrans\sqlTrans.o sqlTest.o ..\lib\libSqlParse.a ..\lib\libSyntaxTree.a
table:
	g++ -std=c++11 -O2 -c ..\common\table.cpp -o ..\common\table.o
sqlExec:
	g++ -std=c++11 -O2 -c ..\sqlExec\sqlExec.cpp -o ..\sqlExec\sqlExec.o
sqlTrans:
	g++ -std=c++11 -O2 -c ..\sqlTrans\sqlTrans.cpp -o ..\sqlTrans\sqlTrans.o
sqlTest:
	g++ -std=c++11 -O2 -c sqlTest.cpp
