#define __CPLUSPLUS

#include "sqlTrans.h"

/*
 * Function: quit function execution
 * Passing in arguments: None
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlTransQuit() {
	if( sqlAction != sqlQUIT ) return -1;
	if( !syntaxTree ) return -1;
	destorySyntaxTree( syntaxTree );
	return 0;
}

/*
 * Function: help function execution
 * Passing in arguments: None
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlTransHelp() {
	if( sqlAction != sqlHELP ) return -1;
	if( !syntaxTree ) return -1;
	string strHelp = "";
	strHelp = "CREATE TABLE example (Col1 INT, Col2 DOUBLE, Col3 STRING);\n";
	puts( "CREATE TABLE" );
	printf( "e.g.\t%s", strHelp.c_str() );
	strHelp = "DROP TABLE example;\n";
	puts( "DROP TABLE" );
	printf( "e.g.\t%s", strHelp.c_str() );
	strHelp = "SELECT * FROM example;\n";
	puts( "SELECT" );
	printf( "e.g.\t%s", strHelp.c_str() );
	strHelp = "INSERT INTO examples VALUES( 10, 3.14, 'hello' );\n";
	puts( "INSERT INTO" );
	printf( "e.g.\t%s", strHelp.c_str() );
	strHelp = "UPDATE example SET Col = 'col2' WHERE Col = 'col1';\n";
	puts( "UPDATE" );
	printf( "e.g.\t%s", strHelp.c_str() );
	strHelp = "DELETE FROM example WHERE Col = 'col';\n";
	puts( "DELETE FROM" );
	printf( "e.g.\t%s", strHelp.c_str() );
	puts( "HELP" );
	puts( "QUIT" );
	destorySyntaxTree( syntaxTree );
	return 0;
}

/*
 * Function: create-table command translates and execution
 * Passing in arguments: TNode *nodeTable - "table" node
 * Return arguments: string &tableName - table name
 * Return value: 0 - success, -1 - failure
 */
int getTableName( TNode *nodeTable, string &tableName ) {
	if( !nodeTable || nodeTable->mType != BRANCH ) return -1;
	int nChild = getChildNum( nodeTable );
	if( nChild <= 0 ) return -1;
	for( int i = 1; i <= nChild; ++i ) {
		TNode *tmp = getChild( nodeTable, i );
		if( !tmp ) return -1;
		tableName += tmp->mStrName;
	}
	return 0;
}

/*
 * Function: get columns' defines
 * Passing in arguments: TNode *nodeColumnDefine - "column_define" node
 * Return arguments: PST &pst - name type pair
 * Return value: 0 - success, -1 - failure
 */
int getColDefine( TNode *nodeColumnDefine, PST &pst ) {
	if( !nodeColumnDefine ) return -1;
	TNode *nodeColumn = getChild( nodeColumnDefine, 1 );
	if( !nodeColumn ) return -1;
	TNode *nodeName = getChild( nodeColumn, 1 );
	if( !nodeName ) return -1;
	string strName = nodeName->mStrName;
	TNode *nodeDataType = getChild( nodeColumnDefine, 2 );
	if( !nodeDataType ) return -1;
	TNode *nodeTypes = getChild( nodeDataType, 1 );
	if( !nodeTypes ) return -1;
	string strType = nodeTypes->mStrName;
	pst.first = strName;
	if( strType == "INT" ) pst.second = 0;
	if( strType == "DOUBLE" ) pst.second = 1;
	if( strType == "STRING" ) pst.second = 2;
	return 0;
}

/*
 * Function: get column define list
 * Passing in arguments: TNode *nodeColumnDefine - "base_table_element_list" node
 * Return arguments: vector<PST> &vecColDefine - column define list
 * Return value: 0 - success, -1 - failure
 */
int getColDefineList( TNode *nodeBaseTableElementList, vector<PST> &vecColDefine ) {
	if( !nodeBaseTableElementList ) return -1;
	int nChild = getChildNum( nodeBaseTableElementList );
	TNode *nodeBaseTableElement = NULL;
	TNode *nodeBaseTableElementListTemp = NULL;
	TNode *nodeColumnDefine = NULL;
	PST pst;
	if( nChild == 1 ) {
		nodeBaseTableElement = getChild( nodeBaseTableElementList, 1 );
		if( !nodeBaseTableElement ) return -1;
		nodeColumnDefine = getChild( nodeBaseTableElement, 1 );
		if( getColDefine( nodeColumnDefine, pst ) ) return -1;
		vecColDefine.push_back( pst );
	} else {
		nodeBaseTableElementListTemp = getChild( nodeBaseTableElementList, 1 );
		if( !nodeBaseTableElementListTemp ) return -1;
		if( getColDefineList( nodeBaseTableElementListTemp, vecColDefine ) ) return -1;
		nodeBaseTableElement = getChild( nodeBaseTableElementList, 3 );
		if( !nodeBaseTableElement ) return -1;
		nodeColumnDefine = getChild( nodeBaseTableElement, 1 );
		if( getColDefine( nodeColumnDefine, pst ) ) return -1;
		vecColDefine.push_back( pst );
	}
	return 0;
}

/*
 * Function: create-table command translates and execution
 * Passing in arguments: None
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlTransCreateTable() {
	if( sqlAction != sqlCREATE_TABLE ) return -1; 
	if( !syntaxTree ) return -1;
	TNode *nodeBaseTableDefine = getChild( syntaxTree, 1 );	
	if( !nodeBaseTableDefine ) return -1;
	TNode *nodeTable = getChild( nodeBaseTableDefine, 3 );
	if( !nodeTable ) return -1;
	string tableName = "";
	if( getTableName( nodeTable, tableName ) ) return -1;
	TNode *nodeBaseTableElementList = getChild( nodeBaseTableDefine, 5 );
	if( !nodeBaseTableElementList ) return -1;
	vector<PST> vecColDefine; vecColDefine.clear();
	if( getColDefineList( nodeBaseTableElementList, vecColDefine ) ) return -1;
	if( sqlExecCreateTable( tableName.c_str(), vecColDefine ) ) return -1;
	destorySyntaxTree( syntaxTree );
	return 0;
}

/*
 * Function: drop command translates and execution
 * Passing in arguments: None
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlTransDrop() {
	if( sqlAction != sqlDROP_TABLE ) return -1;
	if( !syntaxTree ) return -1;
	TNode *nodeDropStatement = getChild( syntaxTree, 1 );
	if( !nodeDropStatement ) return -1;
	TNode *nodeTable = getChild( nodeDropStatement, 3 );
	if( !nodeTable ) return -1;
	string tableName = "";
	if( getTableName( nodeTable, tableName ) ) return -1;
	string strDrop = "del ..\\databases\\" + tableName + ".txt";
	system( strDrop.c_str() );
	destorySyntaxTree( syntaxTree );
	return 0;
}

/*
 * Function: convert column names to expression tree
 * Passing in arguments: vector<string> &vecColNames - column names, // use & to accelerate function call
 *                       int len - column's number
 * Return arguments: ExpNode *&expScalarExtendList - column expression tree
 * Return value: 0 - success, -1 - failure
 */
int colNamesToBinaryExpNodeTree( vector<string> &vecColNames, int len, ExpNode *&expScalarExtendList ) {
	if( len <= 0 ) return 0;
	ExpNode *expScalarExtendListTemp = NULL, *expScalarExtend = NULL;
	if( colNamesToBinaryExpNodeTree( vecColNames, len - 1, expScalarExtendListTemp ) ) return -1;
	expScalarExtend = new ExpNode();
	if( !expScalarExtend ) return -1;
	expScalarExtend->expType = 3;
	expScalarExtend->expVal = vecColNames[len - 1];
	expScalarExtendList = new ExpNode();
	if( !expScalarExtendList ) return -1;
	expScalarExtendList->expType = -1;
	expScalarExtendList->leftChild = expScalarExtendListTemp;
	expScalarExtendList->rightChild = expScalarExtend;
	return 0;
}

/*
 * Function: get atom expression node
 * Passing in arguments: TNode *nodeAtom - "atom" node
 * Return arguments: ExpNode *&expAtom - atom expression node
 * Return value: 0 - success, -1 - failure
 */
int getAtomExpNode( TNode *nodeAtom, ExpNode *&expAtom ) {
	if( !nodeAtom ) return -1;
	TNode *nodeLiteral = getChild( nodeAtom, 1 );
	if( !nodeLiteral ) return -1;
	TNode *nodeVal = getChild( nodeLiteral, 1 );
	if( !nodeVal ) return -1;
	expAtom = new ExpNode();
	stringstream ss;
	if( nodeVal->mType == TYPE_INT ) {
		expAtom->expType = 0;
		ss.clear(); ss << nodeVal->mVal.valInt; ss >> expAtom->expVal;
	} else if( nodeVal->mType == TYPE_DOUBLE ) {
		expAtom->expType = 1;
		ss.clear(); ss << nodeVal->mVal.valDouble; ss >> expAtom->expVal;
	} else if( nodeVal->mType == TYPE_STRING ) {
		expAtom->expType = 2;
		ss.clear(); ss << nodeVal->mVal.valStr; ss >> expAtom->expVal;
	}
	expAtom->leftChild = NULL;
	expAtom->rightChild = NULL;
	return 0;
}

/*
 * Function: get column var expression node
 * Passing in arguments: TNode *nodeAtom - "column_var" node
 * Return arguments: ExpNode *&expColumnVar - column_var expression node
 * Return value: 0 - success, -1 - failure
 */
int getColumnVarExpNode( TNode *nodeColumnVar, ExpNode *&expColumnVar ) {
	if( !nodeColumnVar ) return -1;
	int nChild = getChildNum( nodeColumnVar );
	if( nChild <= 0 ) return -1;
	string strTemp = "";
	for( int i = 1; i <= nChild; ++i ) {
		TNode *tmp = getChild( nodeColumnVar, i );
		if( !tmp ) return -1;
		strTemp += tmp->mStrName;
	}
	expColumnVar = new ExpNode();
	expColumnVar->expType = 3;
	expColumnVar->expVal = strTemp;
	expColumnVar->leftChild = NULL;
	expColumnVar->rightChild = NULL;
	return 0;
}

/*
 * Function: convert scalar syntax tree to scalar expression tree
 * Passing in arguments: TNode *nodeScalarExtend - "scalar_extend" node
 * Return arguments: ExpNode *&expScalarExtend - scalar expression node
 * Return value: 0 - success, -1 - failure
 */
int scalarToBinaryExpNodeTree( TNode *nodeScalarExtend, ExpNode *&expScalarExtend ) {
	if( !nodeScalarExtend ) return -1;
	int nChild = getChildNum( nodeScalarExtend );
	if( nChild == 1 ) {
		TNode *nodeAtomOrColumnVar = getChild( nodeScalarExtend, 1 );
		if( !nodeAtomOrColumnVar ) return -1;
		string strTemp = nodeAtomOrColumnVar->mStrName;
		if( strTemp == "atom" ) {
			if( getAtomExpNode( nodeAtomOrColumnVar, expScalarExtend ) ) return -1;
		} else {
			if( getColumnVarExpNode( nodeAtomOrColumnVar, expScalarExtend ) ) return -1;
		}
	} else {
		TNode *nodeTemp = getChild( nodeScalarExtend, 1 );
		if( !nodeTemp ) return -1;
		if( nodeTemp->mType == BRANCH ) {
			expScalarExtend = new ExpNode();
			TNode *nodeOperator = getChild( nodeScalarExtend, 2 );
			if( !nodeOperator ) return -1;
			TNode *nodeScalarFirst = NULL, *nodeScalarSecond = NULL;
			ExpNode *expScalarFirst = NULL, *expScalarSecond = NULL;
			nodeScalarFirst = getChild( nodeScalarExtend, 1 );
			if( !nodeScalarFirst ) return -1;
			if( nodeOperator->mType != OPERATOR ) return -1;
			nodeScalarSecond = getChild( nodeScalarExtend, 3 );
			if( !nodeScalarSecond ) return -1;
			if( scalarToBinaryExpNodeTree( nodeScalarFirst, expScalarFirst ) ) return -1;
			if( !expScalarFirst ) return -1;
			if( scalarToBinaryExpNodeTree( nodeScalarSecond, expScalarSecond ) ) return -1;
			if( !expScalarSecond ) return -1;
			expScalarExtend->expType = 4;
			expScalarExtend->expVal = nodeOperator->mStrName;
			expScalarExtend->leftChild = expScalarFirst;
			expScalarExtend->rightChild = expScalarSecond;
		} else {
			if( scalarToBinaryExpNodeTree( getChild( nodeScalarExtend, 2 ), expScalarExtend ) ) return -1;
		}
	}
	return 0;
}

/*
 * Function: convert scalar_list syntax tree to scalar expression tree
 * Passing in arguments: TNode *nodeScalarExtendList - "scalar_extend_list" node
 * Return arguments: ExpNode *&expScalarExtendList - scalar expression subtree
 * Return value: 0 - success, -1 - failure
 */
int scalarListToBinaryExpNodeTree( TNode *nodeScalarExtendList, ExpNode *&expScalarExtendList ) {
	if( !nodeScalarExtendList ) return -1;
	int nChild = getChildNum( nodeScalarExtendList );
	if( nChild == 1 ) {
		TNode *nodeScalarExtend = getChild( nodeScalarExtendList, 1 );
		expScalarExtendList = new ExpNode();
		expScalarExtendList->expType = -1;
		expScalarExtendList->leftChild = NULL;
		if( scalarToBinaryExpNodeTree( nodeScalarExtend, expScalarExtendList->rightChild ) ) return -1;
	} else {
		TNode *nodeScalarExtendListTemp = NULL, *nodeScalarExtend = NULL;
		ExpNode *expScalarExtendListTemp = NULL, *expScalarExtend = NULL;
		nodeScalarExtendListTemp = getChild( nodeScalarExtendList, 1 );
		nodeScalarExtend = getChild( nodeScalarExtendList, 3 );
		if( scalarListToBinaryExpNodeTree( nodeScalarExtendListTemp, expScalarExtendListTemp ) ) return -1; 
		if( scalarToBinaryExpNodeTree( nodeScalarExtend, expScalarExtend ) ) return -1;
		expScalarExtendList = new ExpNode();
		if( !expScalarExtendList ) return -1;
		expScalarExtendList->expType = -1;
		expScalarExtendList->leftChild = expScalarExtendListTemp;
		expScalarExtendList->rightChild = expScalarExtend;
	}
	return 0;
}

/*
 * Function: get table reference
 * Passing in arguments: TNode *nodeTableReference - "table_reference" node
 * Return arguments: string &tableName - table name
 * Return value: 0 - success, -1 - failure
 */
int getTableReference( TNode *nodeTableReference, string &tableName ) {
	if( !nodeTableReference ) return -1;
	TNode *nodeTable = getChild( nodeTableReference, 1 );
	if( !nodeTable ) return -1;
	if( getTableName( nodeTable, tableName ) ) return -1;
	return 0;
}

/*
 * Function: get table reference list
 * Passing in arguments: TNode *nodeTableReferenceList - "table_reference_list" node
 * Return arguments: vector<string> &vecTableReferenceList - table reference list
 * Return value: 0 - success, -1 - failure
 */
int getTableReferenceList( TNode *nodeTableReferenceList, vector<string> &vecTableReferenceList ) {
	if( !nodeTableReferenceList ) return -1;
	int nChild = getChildNum( nodeTableReferenceList );
	TNode *nodeTableReferenceListTemp = NULL, *nodeTableReference = NULL;
	string tableName = "";
	if( nChild == 1 ) {
		nodeTableReference = getChild( nodeTableReferenceList, 1 );
		if( !nodeTableReference ) return -1;
		if( getTableReference( nodeTableReference, tableName ) ) return -1;
		vecTableReferenceList.push_back( tableName );
	} else {
		nodeTableReferenceListTemp = getChild( nodeTableReferenceList, 1 );
		if( !nodeTableReferenceListTemp ) return -1;
		if( getTableReferenceList( nodeTableReferenceListTemp, vecTableReferenceList ) ) return -1;
		nodeTableReference = getChild( nodeTableReferenceList, 3 );
		if( !nodeTableReference ) return -1;
		if( getTableReference( nodeTableReference, tableName ) ) return -1;
		vecTableReferenceList.push_back( tableName );
	}
	return 0;
}

map<string, string> oppositeOperator;

/*
 * Function: set opposite operator for all COMPARISION, "AND" and "OR"
 * Passing in arguments: None
 * Return arguments: None
 * Return value: None
 */
void initOppositeOperator() {
	oppositeOperator.clear();
	oppositeOperator["="] = "<>";
	oppositeOperator["<"] = ">=";
	oppositeOperator[">"] = "<=";
	oppositeOperator["<>"] = "=";
	oppositeOperator[">="] = "<";
	oppositeOperator["<="] = ">";
	oppositeOperator["OR"] = "AND";
	oppositeOperator["AND"] = "OR";
	return ;
}

/*
 * Function: get table reference list
 * Passing in arguments: TNode *nodePredicate - "predicate" node, int flag - flag for NOT
 * Return arguments: ExpNode *&expPredicate - predicate expression node
 * Return value: 0 - success, -1 - failure
 */
int predicateToBinaryExpNodeTree( TNode *nodePredicate, ExpNode *&expPredicate, int flag ) {
	if( !nodePredicate ) return -1;
	TNode *nodeComparisonPredicate = getChild( nodePredicate, 1 );
	if( !nodeComparisonPredicate ) return -1;
	expPredicate = new ExpNode();
	if( !expPredicate ) return -1;
	TNode *nodeComparison = getChild( nodeComparisonPredicate, 2 );
	if( !nodeComparison ) return -1;
	if( nodeComparison->mType != OPERATOR ) return -1;
	TNode *nodeScalarFirst = NULL, *nodeScalarSecond = NULL;
	ExpNode *expScalarFirst = NULL, *expScalarSecond = NULL;
	nodeScalarFirst = getChild( nodeComparisonPredicate, 1 );
	if( !nodeScalarFirst ) return -1;
	nodeScalarSecond = getChild( nodeComparisonPredicate, 3 );
	if( !nodeScalarSecond ) return -1;
	if( scalarToBinaryExpNodeTree( nodeScalarFirst, expScalarFirst ) ) return -1;
	if( !expScalarFirst ) return -1;
	if( scalarToBinaryExpNodeTree( nodeScalarSecond, expScalarSecond ) ) return -1;
	if( !expScalarSecond ) return -1;
	expPredicate->expType = 5;
	if( !flag ) expPredicate->expVal = nodeComparison->mStrName;
	else expPredicate->expVal = oppositeOperator[nodeComparison->mStrName];
	expPredicate->leftChild = expScalarFirst;
	expPredicate->rightChild = expScalarSecond;
	return 0;
}

/*
 * Function: get search condition expression tree
 * Passing in arguments: TNode *nodeSearchConditionList - "searchConditionList" node, int flag - flag for NOT
 * Return arguments: ExpNode *&expSearchConditionList - search condition expression subtree
 * Return value: 0 - success, -1 - failure
 */
int searchConditionToBinaryExpNodeTree( TNode *nodeSearchConditionList, ExpNode *&expSearchConditionList, int flag ) {
	if( !nodeSearchConditionList ) return -1;
	int nChild = getChildNum( nodeSearchConditionList );
	if( nChild == 1 ) {
		TNode *nodePredicate = getChild( nodeSearchConditionList, 1 );
		if( !nodePredicate ) return -1;
		if( predicateToBinaryExpNodeTree( nodePredicate, expSearchConditionList, flag ) ) return -1;
	} else if( nChild == 2 ) {
		flag ^= 1;
		if( searchConditionToBinaryExpNodeTree( getChild( nodeSearchConditionList, 2 ), expSearchConditionList, flag ) ) return -1;
	} else {
		TNode *tmp = getChild( nodeSearchConditionList, 1 );
		if( tmp->mType == BOUND ) {
			TNode *nodeSearchConditionListTemp = getChild( nodeSearchConditionList, 2 );
			if( searchConditionToBinaryExpNodeTree( nodeSearchConditionListTemp, expSearchConditionList, flag ) ) return -1;
		} else {
			TNode *nodeSearchConditionFirst = NULL, *nodeSearchConditionSecond = NULL;
			ExpNode *expSearchConditionFirst = NULL, *expSearchConditionSecond = NULL;
			expSearchConditionList = new ExpNode();
			if( !expSearchConditionList ) return -1;
			nodeSearchConditionFirst = getChild( nodeSearchConditionList, 1 );
			if( !nodeSearchConditionFirst ) return -1;
			nodeSearchConditionSecond = getChild( nodeSearchConditionList, 3 );
			if( !nodeSearchConditionSecond ) return -1;
			if( searchConditionToBinaryExpNodeTree( nodeSearchConditionFirst, expSearchConditionFirst, flag ) ) return -1;
			if( !expSearchConditionFirst ) return -1;
			if( searchConditionToBinaryExpNodeTree( nodeSearchConditionSecond, expSearchConditionSecond, flag ) ) return -1;
			if( !expSearchConditionSecond ) return -1;
			expSearchConditionList->expType = 6;
			TNode *nodeOperator = getChild( nodeSearchConditionList, 2 );
			if( !nodeOperator ) return -1;
			if( !flag ) expSearchConditionList->expVal = nodeOperator->mStrName;
			else expSearchConditionList->expVal = oppositeOperator[nodeOperator->mStrName];
			expSearchConditionList->leftChild = expSearchConditionFirst;
			expSearchConditionList->rightChild = expSearchConditionSecond;
		}
	}
	return 0;
}

/*
 * Function: select command translates and execution
 * Passing in arguments: None
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlTransSelectFromWhere() {
	if( sqlAction != sqlSELECT ) return -1;
	if( !syntaxTree ) return -1;
	TNode *nodeSelectStatement = getChild( syntaxTree, 1 );
	if( !nodeSelectStatement ) return -1;
	TNode *nodeTableExtend = getChild( nodeSelectStatement, 3 );
	if( !nodeTableExtend ) return -1;
	TNode *nodeFromClause = getChild( nodeTableExtend, 1 );
	if( !nodeFromClause ) return -1;
	TNode *nodeTableReferenceList = getChild( nodeFromClause, 2 );
	if( !nodeTableReferenceList ) return -1;
	vector<string> vecTableReferenceList; vecTableReferenceList.clear();
	if( getTableReferenceList( nodeTableReferenceList, vecTableReferenceList ) ) return -1;
	// single table, so only get one table
	TNode *nodeSelection = getChild( nodeSelectStatement, 2 );
	if( !nodeSelection ) return -1;
	TNode *nodeScalarExtendList = getChild( nodeSelection, 1 );
	if( !nodeScalarExtendList ) return -1;
	TNode *nodeScalarExtend = NULL;
	string strTemp = "";
	if( nodeScalarExtendList->mType == OPERATOR ) strTemp = nodeScalarExtendList->mStrName;
	ExpNode *expScalarExtendList = NULL;
	string tableName = vecTableReferenceList[0];
	if( strTemp == "*" ) {
		vector<string> vecColNames; vecColNames.clear();
		if( getColNames( tableName.c_str(), vecColNames ) ) return -1;
		if( colNamesToBinaryExpNodeTree( vecColNames, vecColNames.size(), expScalarExtendList ) ) return -1;
	} else {
		if( scalarListToBinaryExpNodeTree( nodeScalarExtendList, expScalarExtendList ) ) return -1;
	}
	TNode *nodeOptWhereClause = getChild( nodeTableExtend, 2 );
	if( !nodeOptWhereClause ) return -1;
	ExpNode *expSearchCondition = NULL;
	if( nodeOptWhereClause->mpChildNode ) {
		TNode *nodeWhereClause = getChild( nodeOptWhereClause, 1 );
		if( !nodeWhereClause ) return -1;
		TNode *nodeSearchCondition = getChild( nodeWhereClause, 2 );
		if( !nodeSearchCondition ) return -1;
		initOppositeOperator();
		if( searchConditionToBinaryExpNodeTree( nodeSearchCondition, expSearchCondition, 0 ) ) return -1;
	} else {
		expSearchCondition = new ExpNode();
		expSearchCondition->expType = -1;
		expSearchCondition->expVal = "TRUE";
		expSearchCondition->leftChild = NULL;
		expSearchCondition->rightChild = NULL;
	}
	if( sqlExecSelectFromWhere( tableName.c_str(), expScalarExtendList, expSearchCondition ) ) return -1;
	destorySyntaxTree( syntaxTree );
	return 0;
}

/*
 * Function: get column name
 * Passing in arguments: TNode *nodeColumn - "column" node
 * Return arguments: string &strTemp - column name
 * Return value: 0 - success, -1 - failure
 */
int getColumnName( TNode *nodeColumn, string &strTemp ) {
	if( !nodeColumn ) return -1;
	TNode *nodeName = nodeColumn->mpChildNode;
	if( !nodeName ) return -1;
	strTemp = nodeName->mStrName;
	return 0;
}

/*
 * Function: get column name list
 * Passing in arguments: TNode *nodeColumnList - "column_list" node
 * Return arguments: vector<string> &vecColNames - column name list
 * Return value: 0 - success, -1 - failure
 */
int getColumnNameList( TNode *nodeColumnList, vector<string> &vecColNames ) {
	if( !nodeColumnList ) return -1;
	int nChild = getChildNum( nodeColumnList );
	TNode *nodeColumn = NULL;
	TNode *nodeColumnListTemp = NULL;
	string strTemp;
	if( nChild == 1 ) {
		nodeColumn = getChild( nodeColumnList, 1 );
		if( !nodeColumn ) return -1;
		if( getColumnName( nodeColumn, strTemp ) ) return -1;
		vecColNames.push_back( strTemp );
	} else {
		nodeColumnListTemp = getChild( nodeColumnList, 1 );
		if( !nodeColumnListTemp ) return -1;
		if( getColumnNameList( nodeColumnListTemp, vecColNames ) ) return -1;
		nodeColumn = getChild( nodeColumnList, 3 );
		if( !nodeColumn ) return -1;
		if( getColumnName( nodeColumn, strTemp ) ) return -1;
		vecColNames.push_back( strTemp );
	}
	return 0;
}

/*
 * Function: get column value
 * Passing in arguments: TNode *nodeInsertAtom - "insert_atom" node
 * Return arguments: string &strTemp - column value
 * Return value: 0 - success, -1 - failure
 */
int getColumnVal( TNode *nodeInsertAtom, string &strTemp ) {
	if( !nodeInsertAtom ) return -1;
	TNode *nodeAtom = getChild( nodeInsertAtom, 1 );
	if( !nodeAtom ) return -1;
	TNode *nodeLiteral = getChild( nodeAtom, 1 );
	if( !nodeLiteral ) return -1;
	TNode *nodeVal = getChild( nodeLiteral, 1 );
	if( !nodeVal ) return -1;
	stringstream ss; ss.clear();
	if( nodeVal->mType == TYPE_INT ) ss << nodeVal->mVal.valInt; ss >> strTemp;
	if( nodeVal->mType == TYPE_DOUBLE ) ss << nodeVal->mVal.valDouble; ss >> strTemp;
	if( nodeVal->mType == TYPE_STRING ) strTemp = nodeVal->mVal.valStr;
	return 0;
}

/*
 * Function: get column value list
 * Passing in arguments: TNode *nodeInsertAtomList - "insert_atom_list" node
 * Return arguments: vector<string> &vecColVals - column value list
 * Return value: 0 - success, -1 - failure
 */
int getColumnValList( TNode *nodeInsertAtomList, vector<string> &vecColVals ) {
	if( !nodeInsertAtomList ) return -1;
	int nChild = getChildNum( nodeInsertAtomList );
	TNode *nodeInsertAtom = NULL;
	TNode *nodeInsertAtomListTemp = NULL;
	string strTemp = "";
	if( nChild == 1 ) {
		nodeInsertAtom = getChild( nodeInsertAtomList, 1 );
		if( !nodeInsertAtom ) return -1;
		if( getColumnVal( nodeInsertAtom, strTemp ) ) return -1;
		vecColVals.push_back( strTemp );
	} else {
		nodeInsertAtomListTemp = getChild( nodeInsertAtomList, 1 );
		if( !nodeInsertAtomListTemp ) return -1;
		if( getColumnValList( nodeInsertAtomListTemp, vecColVals ) ) return -1;
		nodeInsertAtom = getChild( nodeInsertAtomList, 3 );
		if( !nodeInsertAtom ) return -1;
		if( getColumnVal( nodeInsertAtom, strTemp ) ) return -1;
		vecColVals.push_back( strTemp );
	}
	return 0;
}

/*
 * Function: insert-into command translates and execution
 * Passing in arguments: None
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlTransInsertIntoValues() {
	if( sqlAction != sqlINSERT ) return -1;
	if( !syntaxTree ) return -1;
	TNode *nodeInsertStatement = getChild( syntaxTree, 1 );
	if( !nodeInsertStatement ) return -1;
	TNode *nodeTable = getChild( nodeInsertStatement, 3 );
	if( !nodeTable ) return -1;
	string tableName = "";
	if( getTableName( nodeTable, tableName ) ) return -1;
	TNode *nodeOptColumnList = getChild( nodeInsertStatement, 4 );
	if( !nodeOptColumnList ) return -1;
	vector<string> vecColNames; vecColNames.clear();
	if( nodeOptColumnList->mpChildNode ) {
		TNode *nodeColumnList = getChild( nodeOptColumnList, 2 );
		if( getColumnNameList( nodeColumnList, vecColNames ) ) return -1;
	} else {
		if( getColNames( tableName.c_str(), vecColNames ) ) return -1;
	}
	TNode *nodeValuesOrQuery = getChild( nodeInsertStatement, 5 );
	if( !nodeValuesOrQuery ) return -1;
	TNode *nodeInsertAtomList = getChild( nodeValuesOrQuery, 3 );
	if( !nodeInsertAtomList ) return -1;
	vector<Val> vecColVals; vecColVals.clear();
	if( getColumnValList( nodeInsertAtomList, vecColVals ) ) return -1;
	if( sqlExecInsertIntoValues( tableName.c_str(), vecColNames, vecColVals ) ) return -1;
	destorySyntaxTree( syntaxTree );
	return 0;
}

/*
 * Function: get assignment
 * Passing in arguments: TNode *nodeAssignment - "assignment" node
 * Return arguments: PSE &pse - string expression-tree pair
 * Return value: 0 - success, -1 - failure
 */
int getAssignment( TNode *nodeAssignment, PSE &pse ) {
	if( !nodeAssignment ) return -1;
	int nChild = getChildNum( nodeAssignment );
	if( nChild != 3 ) return -1;
	TNode *nodeColumn = getChild( nodeAssignment, 1 );
	if( !nodeColumn ) return -1;
	TNode *nodeOperator = getChild( nodeAssignment, 2 );
	if( !nodeOperator ) return -1;
	TNode *nodeVal = getChild( nodeAssignment, 3 );
	if( !nodeVal ) return -1;
	if( getColumnName( nodeColumn, pse.first ) ) return -1;
	if( scalarToBinaryExpNodeTree( nodeVal, pse.second ) ) return -1;
	return 0;
}

/*
 * Function: get assignment list
 * Passing in arguments: TNode *nodeAssignmentList - "assignment_list" node
 * Return arguments: vector<PSE> &vecAssignments - string expression-tree pair list
 * Return value: 0 - success, -1 - failure
 */
int getAssignmentList( TNode *nodeAssignmentList, vector<PSE> &vecAssignments ) {
	if( !nodeAssignmentList ) return -1;
	int nChild = getChildNum( nodeAssignmentList );
	TNode *nodeAssignment = NULL;
	TNode *nodeAssignmentListTemp = NULL;
	PSE pse;
	if( nChild == 1 ) {
		nodeAssignment = getChild( nodeAssignmentList, 1 );
		if( !nodeAssignment ) return -1;
		if( getAssignment( nodeAssignment, pse ) ) return -1;
		vecAssignments.push_back( pse );
	} else {
		nodeAssignmentListTemp = getChild( nodeAssignmentList, 1 );
		if( !nodeAssignmentListTemp ) return -1;
		if( getAssignmentList( nodeAssignmentListTemp, vecAssignments ) ) return -1;
		nodeAssignment = getChild( nodeAssignmentList, 3 );
		if( !nodeAssignment ) return -1;
		if( getAssignment( nodeAssignment, pse ) ) return -1;
		vecAssignments.push_back( pse );
	}
	return 0;
}

/*
 * Function: update command translates and execution
 * Passing in arguments: None
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlTransUpdateSetWhere() {
	if( sqlAction != sqlUPDATE ) return -1;
	if( !syntaxTree ) return -1;
	TNode *nodeUpdateStatement = getChild( syntaxTree, 1 );
	if( !nodeUpdateStatement ) return -1;
	TNode *nodeTable = getChild( nodeUpdateStatement, 2 );
	if( !nodeTable ) return -1;
	string tableName = "";
	if( getTableName( nodeTable, tableName ) ) return -1;
	TNode *nodeAssignmentList = getChild( nodeUpdateStatement, 4 );
	if( !nodeAssignmentList ) return -1;
	vector<PSE> vecAssignments; vecAssignments.clear();
	if( getAssignmentList( nodeAssignmentList, vecAssignments ) ) return -1;
	TNode *nodeOptWhereClause = getChild( nodeUpdateStatement, 5 );
	if( !nodeOptWhereClause ) return -1;
	ExpNode *expSearchCondition = NULL;
	if( nodeOptWhereClause->mpChildNode ) {
		TNode *nodeWhereClause = getChild( nodeOptWhereClause, 1 );
		if( !nodeWhereClause ) return -1;
		TNode *nodeSearchCondition = getChild( nodeWhereClause, 2 );
		if( !nodeSearchCondition ) return -1;
		initOppositeOperator();
		if( searchConditionToBinaryExpNodeTree( nodeSearchCondition, expSearchCondition, 0 ) ) return -1;
	} else {
		expSearchCondition = new ExpNode();
		expSearchCondition->expType = -1;
		expSearchCondition->expVal = "TRUE";
		expSearchCondition->leftChild = NULL;
		expSearchCondition->rightChild = NULL;
	}
	if( sqlExecUpdateSetWhere( tableName.c_str(), vecAssignments, expSearchCondition ) ) return -1;
	destorySyntaxTree( syntaxTree );
	return 0;
}

/*
 * Function: update command translates and execution
 * Passing in arguments: None
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlTransDeleteFromWhere() {
	if( sqlAction != sqlDELETE ) return -1;
	if( !syntaxTree ) return -1;
	TNode *nodeDeleteStatement = getChild( syntaxTree, 1 );
	if( !nodeDeleteStatement ) return -1;
	TNode *nodeTable = getChild( nodeDeleteStatement, 3 );
	if( !nodeTable ) return -1;
	string tableName;
	if( getTableName( nodeTable, tableName ) ) return -1;
	TNode *nodeOptWhereClause = getChild( nodeDeleteStatement, 4 );
	if( !nodeOptWhereClause ) return -1;
	ExpNode *expSearchCondition = NULL;
	if( nodeOptWhereClause->mpChildNode ) {
		TNode *nodeWhereClause = getChild( nodeOptWhereClause, 1 );
		if( !nodeWhereClause ) return -1;
		TNode *nodeSearchCondition = getChild( nodeWhereClause, 2 );
		if( !nodeSearchCondition ) return -1;
		initOppositeOperator();
		if( searchConditionToBinaryExpNodeTree( nodeSearchCondition, expSearchCondition, 0 ) ) return -1;
	} else {
		expSearchCondition = new ExpNode();
		expSearchCondition->expType = -1;
		expSearchCondition->expVal = "TRUE";
		expSearchCondition->leftChild = NULL;
		expSearchCondition->rightChild = NULL;
	}
	if( sqlExecDeleteFromWhere( tableName.c_str(), expSearchCondition ) ) return -1;
	destorySyntaxTree( syntaxTree );
	return 0;
}

/*
 * Function: sql syntax parse interface
 * Passing in arguments: const char *sql - sql command
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlTransParse( const char *sql ) {
	return sqlParse( sql );
}

/*
 * Function: print syntax tree interface
 * Passing in arguments: TNode *root - syntax tree root, int level - node level
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int sqlTransPrintTree( TNode *root, int level ) {
	printTree( root, level );
	return 0;
}
