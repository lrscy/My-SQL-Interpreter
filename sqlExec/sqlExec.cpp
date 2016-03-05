#include "sqlExec.h"

/*
 * Function: create-table function to basic table functions interface
 * Passing in arguments: const char *tableName - table name, vector<PST> vecColDefine - column defines
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlExecCreateTable( const char *tableName, vector<PST> vecColDefine ) {
	if( !tableName ) return -1;
	if( createTable( tableName ) ) return -1;
	int nCol = vecColDefine.size();
	for( int i = 0; i < nCol; ++i ) {
		if( addCol( tableName, vecColDefine[i] ) ) return -1;
	}
	return 0;
}

/*
 * Function: select function to basic table functions interface
 * Passing in arguments: const char *tableName - table name,
 *                       ExpNode *expScalarExtendList - column expression tree
 *                       ExpNode *expSearchCondition - where clause expression tree
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlExecSelectFromWhere( const char *tableName, ExpNode *expScalarExtendList, ExpNode *expSearchCondition ) {
	if( !tableName ) return -1;
	vector<int> vecColNos; vecColNos.clear();
	vector<int> vecRowNos; vecRowNos.clear();
	// handle '*' of column project
	if( ( expSearchCondition->expType == -1 ) && ( expSearchCondition->expVal == "TRUE" ) ) {
		if( getRows( tableName, vecRowNos ) ) return -1;
	} else {
		if( conditionRow( tableName, expSearchCondition, vecRowNos ) ) return -1;
	}
	Table table;
	if( conditionTable( tableName, expScalarExtendList, vecRowNos, table ) ) return -1;
	if( printTable( table.tableName ) ) return -1;
	return 0;
}

/*
 * Function: insert-into function to basic table functions interface
 * Passing in arguments: const char *tableName - table name,
 *                       vector<string> vecColNames - column names,
 *                       vector<Val> vecColVals - column values
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlExecInsertIntoValues( const char *tableName, vector<string> vecColNames, vector<Val> vecColVals ) {
	if( !tableName ) return -1;
	int nCol = vecColNames.size();
	if( nCol != vecColVals.size() ) return -1;
	vector<PSV> vecColNameVal;
	for( int i = 0; i < nCol; ++i ) vecColNameVal.push_back( make_pair( vecColNames[i], vecColVals[i] ) );
	if( addRowByNames( tableName, vecColNameVal ) ) return -1;
	return 0;
}

/*
 * Function: update function to basic table functions interface
 * Passing in arguments: const char *tableName - table name,
 *                       vector<PSE> vecAssignments - column name expression-tree pairs,
 *                       ExpNode *expSearchCondition - where clause expression tree
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlExecUpdateSetWhere( const char *tableName, vector<PSE> vecAssignments, ExpNode *expSearchCondition ) {
	if( !tableName ) return -1;
	vector<int> vecRowNos; vecRowNos.clear();
	if( conditionRow( tableName, expSearchCondition, vecRowNos ) ) return -1;
	if( updRowByNoVector( tableName, vecRowNos, vecAssignments ) ) return -1;
	return 0;
}

/*
 * Function: delete-from function to basic table functions interface
 * Passing in arguments: const char *tableName - table name, ExpNode *expSearchCondition - where clause expression tree
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlExecDeleteFromWhere( const char *tableName, ExpNode *expSearchCondition ) {
	if( !tableName ) return -1;
	vector<int> vecRowNos; vecRowNos.clear();
	if( ( expSearchCondition->expType == -1 ) && ( expSearchCondition->expVal == "TRUE" ) ) {
		if( getRows( tableName, vecRowNos ) ) return -1;
	} else {
		if( conditionRow( tableName, expSearchCondition, vecRowNos ) ) return -1;
	}
	if( delRowByNoVector( tableName, vecRowNos ) ) return -1;
	return 0;
}
