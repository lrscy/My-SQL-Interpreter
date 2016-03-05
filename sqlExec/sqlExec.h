#ifndef SQLEXEC_H
#define SQLEXEC_H

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include "..\common\table.h"
using namespace std;

// Create-table function to basic table functions interface
int sqlExecCreateTable( const char *tableName, vector<PST> vecColDefine );

// Select function to basic table functions interface
int sqlExecSelectFromWhere( const char *tableName, ExpNode *expScalarExtendList, ExpNode *expSearchCondition );

// Insert-into function to basic table functions interface
int sqlExecInsertIntoValues( const char *tableName, vector<string> vecColNames, vector<Val> vecColVals );

// Update function to basic table functions interface
int sqlExecUpdateSetWhere( const char *tableName, vector<PSE> vecAssignments, ExpNode *expSearchCondition );

// Delete-from function to basic table functions interface
int sqlExecDeleteFromWhere( const char *tableName, ExpNode *expSearchCondition );

#endif // SQLEXEC_H
