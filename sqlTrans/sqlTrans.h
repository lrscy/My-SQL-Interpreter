#ifndef SQLTRANS_H
#define SQLTRANS_H

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include "..\common\table.h"
#include "..\sqlExec\sqlExec.h"
#include "..\sqlParse\sqlParse.h"
#include "..\sqlParse\syntaxTree.h"
using namespace std;

// Quit function execution
int sqlTransQuit();

// Help function execution
int sqlTransHelp();

// Create-table command translates and execution
int sqlTransCreateTable();

// Drop command translates and execution
int sqlTransDrop();

// Select command translates and execution
int sqlTransSelectFromWhere();

// Insert-into command translates and execution
int sqlTransInsertIntoValues();

// Update command translates and execution
int sqlTransUpdateSetWhere();

// Delete-from command translates and execution
int sqlTransDeleteFromWhere();

// sql syntax parse interface
int sqlTransParse( const char *sql );
// print syntax tree interface
int sqlTransPrintTree( TNode *root, int level );

#endif // SQLTRANS_H
