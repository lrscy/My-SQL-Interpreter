#ifndef TABLE_H
#define TABLE_H

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <map>
using namespace std;
const int MAXLEN = 1000 + 10;
const int NAMELEN = 100 + 10;

typedef int Type;
typedef string Val;
typedef string Opt;
typedef vector<Val> RowVal;
typedef pair<string, Type> PST;
typedef pair<string, Val> PSV;

// Table struct
struct Table {
	char tableName[NAMELEN];
	int nCol, nRow;
	vector<string> colName;
	int *colType; // 0 int 1 double 2 string
	vector<RowVal> data;
	map<string, int> colNo, rowNo;
};

// Expression Tree Node struct
struct ExpNode {
	int expType; // -1 node 0 int 1 double 2 string 3 name 4 operatorVal 5 operatorBool 6 comparison
	Val expVal;
	struct ExpNode *leftChild, *rightChild;
};

typedef pair<string, ExpNode *> PSE;

// Create Table function
int createTable( const char *tableName );

// Get all column names in the table
int getColNames( const char *tableName, vector<string> &vecCol );

// Get all rows' number in the table
int getRows( const char *tablename, vector<int> &vecRow );

// Add a column to the table when it's empty
int addCol( const char *tableName, PST pss ); // pss <name, type>

// Add a row to the table by column names
int addRowByNames( const char *tableName, vector<PSV> vecRow );

// Get rows in the table by expression tree passed in
int conditionRow( const char *tableName, ExpNode *root, vector<int> &vecRet );

// Get temporary table by the rows' number and the column expression tree passed in
int conditionTable( const char *tableName, ExpNode *conditionCols, vector<int> vecRow, Table &tableRet );

// delete row by rows' number passed in
int delRowByNoVector( const char *tableName, vector<int> vecRow );

// update row by rows' number and string expression-tree pair
int updRowByNoVector( const char *tableName, vector<int> vecRow, vector<PSE> vecCol );

// print table to the console
int printTable( const char *tableName );

// print expression tree to the console, just for debug
int printExpTree( ExpNode *root, int level );

#endif // TABLE_H
