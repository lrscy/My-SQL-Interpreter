#include "table.h"

/*
 * Function: read table from the databases folder
 * Passing in arguments: const char *tableName - table name, Table *table - table pointer
 * Return arguments: None
 * Return value: 0 - success, -1 - failure
 */
int read( const char *tableName, Table *table ) {
	char str1[MAXLEN] = "..\\databases\\", str2[NAMELEN];
	string str3;
	int row, col, len, cnt, tmp;
	strcat( str1, tableName );
	strcat( str1, ".txt" );
	FILE *fp = fopen( str1, "r+" );
	if( fp == NULL ) return -1;
	// Title tableName nRow nCol
	memset( str1, 0, sizeof( str1 ) );
	if( NULL == fgets( str1, MAXLEN, fp ) ) return -1;
	stringstream ss( str1 );
	ss >> str2 >> row >> col;
	// init table
	strcpy( table->tableName, str2 );
	table->nRow = row; table->nCol = col;
	// if table is empty, stop read
	if( table->nRow == 0 && table->nCol == 0 ) return 0;
	// colName
	if( NULL == fgets( str1, MAXLEN, fp ) ) return -1;
	ss.clear(); ss.str( str1 );
	cnt = 0;
	for( int i = 0; i < table->nCol; ++i ) {
		ss >> str3;
		table->colName.push_back( str3 );
		table->colNo[table->colName[i]] = i;
	}
	// colType
	table->colType = ( int * )malloc( table->nCol * sizeof( int ) );
	if( NULL == fgets( str1, MAXLEN, fp ) ) return -1;
	ss.clear(); ss.str( str1 );
	for( int i = 0; i < table->nCol; ++i ) ss >> table->colType[i];
	// datas
	for( int i = 0; i < table->nRow; ++i ) {
		if( NULL == fgets( str1, MAXLEN, fp ) ) return -1;
		RowVal tVec;
		ss.clear(); ss.str( str1 );
		for( int j = 0; j < table->nCol; ++j ) {
			ss >> str3;
			tVec.push_back( str3 );
			if( j == 0 ) table->rowNo[str3] = i + 1;
		}
		table->data.push_back( tVec );
	}
	fclose( fp );
	return 0;
}

/*
 * Function: write table to the databases folder
 * Passing in arguments: const char *tableName - table name, Table *table - table pointer
 * Return arguments: None
 * Return values: 0 - success, -1 - failure
 */
int write( const char *tableName, Table *table ) {
	char str1[MAXLEN] = "..\\databases\\";
	strcat( str1, tableName );
	strcat( str1, ".txt" );
	FILE *fp = fopen( str1, "w+" );
	if( !fp ) return -1;
	// write table name, row number and column number
	fprintf( fp, "%s %d %d\n", table->tableName, table->nRow, table->nCol );
	// write column names
	for( int i = 0; i < table->nCol; ++i ) fprintf( fp, "%s ", table->colName[i].c_str() ); fprintf( fp, "\n" );
	// write column types
	for( int i = 0; i < table->nCol; ++i ) fprintf( fp, "%d ", table->colType[i] ); fprintf( fp, "\n" );
	// write datas
	for( int i = 0; i < table->nRow; ++i ) {
		for( int j = 0; j < table->nCol; ++j )
			fprintf( fp, "%s ", table->data[i][j].c_str() );
		fprintf( fp, "\n" );
	}
	fclose( fp );
	return 0;
}

/*
 * Function: create an empty table which only have table name, row number and column number
 * Passing in arguments: const char *tableName - table name
 * Return arguments: None
 * Return values: 0 - success, -1 - failure
 */
int createTable( const char *tableName ) {
	char str1[MAXLEN] = "..\\databases\\";
	strcat( str1, tableName );
	strcat( str1, ".txt" );
	FILE *fp = fopen( str1, "r+" );
	if( fp ) { fclose( fp ); return -1; }
	fclose( fp );
	fp = fopen( str1, "w+" );
	// table name, row number and column number
	fprintf( fp, "%s 0 0\n", tableName );
	fclose( fp );
	return 0;
}

/*
 * Function: get all column names of the table
 * Passing in arguments: const char *tableName - table name
 * Return arguments: vector<string> &vecCol - all the column names in the table
 * Return values: 0 - success, -1 - failure
 */
int getColNames( const char *tableName, vector<string> &vecCol ) {
	if( !tableName ) return -1;
	Table table;
	if( read( tableName, &table ) ) return -1;
	vecCol.clear();
	for( int i = 0; i < table.nCol; ++i ) vecCol.push_back( table.colName[i] );
	if( write( tableName, &table ) ) return -1;
	return 0;
}

/*
 * Function: get all column names of the table
 * Passing in arguments: const char *tableName - table name
 * Return arguments: vector<int> &vecRow - all the row number in the table
 * Return values: 0 - success, -1 - failure
 */
int getRows( const char *tableName, vector<int> &vecRow ) {
	if( !tableName ) return -1;
	Table table;
	if( read( tableName, &table ) ) return -1;
	vecRow.clear();
	for( int i = 0; i < table.nRow; ++i ) {
		vecRow.push_back( i + 1 );
	}
	if( write( tableName, &table ) ) return -1;
	return 0;
}

/*
 * Function: get all column names of the table
 * Passing in arguments: const char *tableName - table name, PST pss - name type pair
 * Return arguments: None
 * Return values: 0 - success, -1 - failure
 */
int addCol( const char *tableName, PST pss ) {
	if( !tableName ) return -1;
	Table table;
	if( read( tableName, &table ) ) return -1;
	++table.nCol;
	table.colName.push_back( pss.first );
	table.colType = ( int * )realloc( table.colType, table.nCol * sizeof( int ) );
	table.colType[table.nCol - 1] = pss.second;
	table.colNo[pss.first] = table.nCol - 1;
	if( write( tableName, &table ) ) return -1;
	return 0;
}

/*
 * Function: add a row to the table by column names
 * Passing in arguments: const char *tableName - table name, vector<PSV> vecRow - name value pair
 * Return arguments: None
 * Return values: 0 - success, -1 - failure
 */
int addRowByNames( const char *tableName, vector<PSV> vecRow ) {
	if( !tableName ) return -1;
	int tmp;
	Table table;
	if( read( tableName, &table ) ) return -1;
	if( vecRow.size() != table.nCol ) return -1;
	++table.nRow;
	// get a new line
	RowVal tVec( 10 );
	for( int i = 0; i < table.nCol; ++i ) {
		// get column number by column name
		tmp = table.colNo[vecRow[i].first];
		tVec[tmp] = vecRow[i].second;
		if( tmp == 0 ) {
		   	if( table.rowNo[vecRow[i].second] != 0 ) return -2;
			else table.rowNo[vecRow[i].second] = table.nRow;
		}
	}
	table.data.push_back( tVec );
	if( write( tableName, &table ) ) return -1;
	return 0;
}

/*
 * Function: int comparison
 * Passing in arguments: Val nVal - value in table, Opt opt - operator, Val tVal - value in expression tree
 * Return arguments: None
 * Return values: true - compare success, false - compare failure
 */
bool nCmp( Val nVal, Opt opt, Val tVal ) {
	int n1, n2;
	// convert string to int
	stringstream ss( nVal + " " + tVal );
	ss >> n1 >> n2;
	if( opt == "=" ) return n1 == n2;
	else if( opt == "<" ) return n1 < n2;
	else if( opt == ">" ) return n1 > n2;
	else if( opt == "<>" ) return n1 != n2;
	else if( opt == ">=" ) return n1 >= n2;
	else if( opt == "<=" ) return n1 <= n2;
	return false;
}

/*
 * Function: double comparison
 * Passing in arguments: Val dVal - value in table, Opt opt - operator, Val tVal - value in expression tree
 * Return arguments: None
 * Return values: true - compare success, false - compare failure
 */
bool dCmp( Val dVal, Opt opt, Val tVal ) {
	// to avoid accuracy error
	const int eps = 1e-8;
	double d1, d2;
	// convert string to double
	stringstream ss( dVal + " " + tVal );
	ss >> d1 >> d2;
	if( opt == "=" ) return fabs( d1 - d2 ) <= eps;
	else if( opt == "<" ) return d1 - d2 < -eps;
	else if( opt == ">" ) return d1 - d2 > eps;
	else if( opt == "<>" ) return fabs( d1 - d2 ) > eps;
	else if( opt == ">=" ) return d1 - d2 >= -eps;
	else if( opt == "<=" ) return d1 - d2 < eps;
	return false;
}

/*
 * Function: string comparison
 * Passing in arguments: Val sVal - value in table, Opt opt - operator, Val tVal - value in expression tree
 * Return arguments: None
 * Return values: true - compare success, false - compare failure
 */
bool sCmp( Val sVal, Opt opt, Val tVal ) {
	if( opt == "=" ) return sVal == tVal;
	else if( opt == "<" ) return sVal < tVal;
	else if( opt == ">" ) return sVal > tVal;
	else if( opt == "<>" ) return sVal != tVal;
	else if( opt == ">=" ) return sVal >= tVal;
	else if( opt == "<=" ) return sVal <= tVal;
	return false;
}

/*
 * Function: value comparison
 * Passing in arguments: Val Val - value in table, Opt opt - operator, Val tVal - value in expression tree
 * Return arguments: None
 * Return values: true - compare success, false - compare failure
 */
bool tCmp( Val val, Opt opt, Val tVal, int type ) {
	bool flag = false;
	switch( type ) {
		case 0: flag = nCmp( val, opt, tVal ); break;    // 0 int
		case 1: flag = dCmp( val, opt, tVal ); break;    // 1 double
		case 2: flag = sCmp( val, opt, tVal ); break;    // 2 string
	}
	return flag;
}

/*
 * Function: calculate int variable
 * Passing in arguments: Val val1 - first value, Opt opt - operator, Val val2 - second value
 * Return arguments: Val &val - calculate result value, Type &type - result value's type 
 * Return values: 0 - success, -1 - failure
 * Note: result value's type may become double when 9 / 2 = 4.5
 */
int calInt( Val val1, Opt opt, Val val2, Val &val, Type &type ) {
	// convert string to int
	stringstream ss;
	ss << ( val1 + " " + val2 );
	char str[100];
	int n1, n2, n;
	type = 0;
	ss >> n1 >> n2; ss.clear();
	if( opt == "+" ) {
		n = n1 + n2;
		ss << n; ss >> val;
		return 0;
	}
	if( opt == "-" ) {
		n = n1 - n2;
		ss << n; ss >> val;
		return 0;
	}
	if( opt == "*" ) {
		n = n1 * n2;
		ss << n; ss >> val;
		return 0;
	}
	if( opt == "/" ) {
		// divisor cannot be 0
		if( n2 == 0 ) return -1;
		// if n1 mod n2 not equals 0, convert result values' type from int to double
		if( n1 % n2 ) {
			double d = 1.0 * n1 / n2;
			ss << d; ss >> val;
			type = 1;
		} else {
			n = n1 / n2;
			ss << n; ss >> val;
		}
		return 0;
	}
	return 0;
}

/*
 * Function: calculate double variable
 * Passing in arguments: Val val1 - first value, Opt opt - operator, Val val2 - second value
 * Return arguments: Val &val - calculate result value
 * Return values: 0 - success, -1 - failure
 */
int calDouble( Val val1, Opt opt, Val val2, Val &val ) {
	// to avoid accuracy error
	const double eps = 1e-8;
	// convert string to double
	stringstream ss;
	ss << ( val1 + " " + val2 );
	double d1, d2, d;
	ss >> d1 >> d2; ss.clear();
	if( opt == "+" ) {
		d = d1 + d2;
		ss << d; ss >> val;
		return 0;
	}
	if( opt == "-" ) {
		d = d1 - d2;
		ss << d; ss >> val;
		return 0;
	}
	if( opt == "*" ) {
		d = d1 * d2;
		ss << d; ss >> val;
		return 0;
	}
	if( opt == "/" ) {
		// divisor cannot be 0
		if( fabs( d2 ) <= eps ) return -1;
		d = d1 / d2;
		ss << d; ss >> val;
		return 0;
	}
	return 0;
}

/*
 * Function: calculate result of the expression tree
 * Passing in arguments: Table *table - table pointer, int row - row number, ExpNode *root - expression tree node
 * Return arguments: Val &val - calculate result value, Type &type - result value's type
 * Return values: 0 - success, -1 - failure
 */
int cal( Table *table, int row, ExpNode *root, Val &val, Type &type ) {
	if( !root ) return -1;
	// string cannot be calculated
	if( root->expType == 2 ) return -1;
	// get value which type is int or double
	if( root->expType < 2 ) { val = root->expVal; type = root->expType; return 0; }
	// if node type is column name, get its value
	if( root->expType == 3 ) {
		int col = table->colNo[root->expVal];
		val = table->data[row][col];
		type = table->colType[col];
		return 0;
	}
	// if node type is operatorVal, calculate its left child and right child, then return a value
	if( root->expType == 4 ) {
		Val val1, val2;
		Type type1, type2;
		if( cal( table, row, root->leftChild, val1, type1 ) ) return -1;
		if( cal( table, row, root->rightChild, val2, type2 ) ) return -1;
		type = max( type1, type2 );
		if( type == 0 && calInt( val1, root->expVal, val2, val, type ) ) return -1;
		if( type == 1 && calDouble( val1, root->expVal, val2, val ) ) return -1;
	}
	return 0;
}

/*
 * Function: check row whether meets conditions in the expression tree
 * Passing in arguments: Table *table - table name, ExpNode *root - expression tree node
 * Return arguments: int &errmsg - error message
 * Return values: true - meet the conditions, false - fail meeting the conditions
 */
bool check( Table *table, int row, ExpNode *root, int &errmsg ) {
	if( !root ) return false;
	errmsg = 0;
	// handle operator "AND" and "OR"
	if( root->expType == 6 ) {
		bool flag1 = check( table, row, root->leftChild, errmsg );
		if( errmsg ) return false;
		bool flag2 = check( table, row, root->rightChild, errmsg );
		if( errmsg ) return false;
		if( root->expVal == "AND" ) return flag1 && flag2;
		else return flag1 || flag2;
	}
	// handle comparisons
	if( root->expType == 5 ) {
		Val val1, val2;
		Type type1, type2;
		type1 = root->leftChild->expType;
		type2 = root->rightChild->expType;
		if( type1 ==  4 || type1 == 5 ) {
			errmsg = cal( table, row, root->leftChild, val1, type1 );
			if( errmsg ) return false;
		} else if( type1 < 4 ) {
			val1 = root->leftChild->expVal;
			if( type1 == 3 ) {
				int col = table->colNo[val1];
				val1 = table->data[row][col];
				type1 = table->colType[col];
			}
		}
		if( type2 == 4 || type2 == 5 ) {
			errmsg = cal( table, row, root->rightChild, val2, type2 );
			if( errmsg ) return false;
		} else if( type2 < 4 ) {
			val2 = root->rightChild->expVal;
			if( type2 == 3 ) {
				int col = table->colNo[val2];
				val2 = table->data[row][col];
				type2 = table->colType[col];
			}
		}
		// make two values' type same
		if( type1 < 2 && type2 < 2 ) type1 = max( type1, type2 );
		else if( type1 == 2 && type2 == 2 ) type1 = 2;
		else { errmsg = -1; return false; }
		return tCmp( val1, root->expVal, val2, type1 );
	}
	return false;
}

/*
 * Function: get rows in the table which meets conditions in the expression tree
 * Passing in arguments: Table *table - table name, ExpNode *root - expression tree node
 * Return arguments: vector<int> &vecRet - rows which meet condions in the expression tree
 * Return values: 0 - success, -1 - failure
 */
int conditionRow( const char *tableName, ExpNode *root, vector<int> &vecRet ) {
	if( !tableName ) return -1;
	Table table;
	if( read( tableName, &table ) ) return -1;
	for( int i = 0; i < table.nRow; ++i ) {
		int errmsg;
		// check the row whether meet the conditions in the expression tree
		if( check( &table, i, root, errmsg ) ) {
			vecRet.push_back( i + 1 );
			continue;
		}
		if( errmsg ) return errmsg;
	}
	if( write( tableName, &table ) ) return -1;
	return 0;
}

/*
 * Function: get column names by scanning the expression tree
 * Passing in arguments: ExpNode *scalarExtend - expression tree node
 * Return arguments: vector<string> &vecCol - column names in the expression tree
 * Return values: 0 - success, -1 - failure
 */
int scanCols( ExpNode *scalarExtend, vector<string> &vecCol ) {
	if( !scalarExtend ) return -1;
	if( scalarExtend->expType == 3 ) {
		vecCol.push_back( scalarExtend->expVal );
		return 0;
	}
	if( scalarExtend->leftChild ) scanCols( scalarExtend->leftChild, vecCol );
	if( scalarExtend->rightChild ) scanCols( scalarExtend->rightChild, vecCol );
	return 0;
}

/*
 * Function: calculate result of the expression tree and write in the temporary table
 * Passing in arguments: Table *table - original table pointer, Table *tableRet - temporary table pointer,
 *                       int rowNo - row number in original table, int rowNoRet - row number in temporary table,
 *                       int colNo - column number in the temporary table,
 *                       ExpNode *conditionCols - column expression tree
 * Return arguments: None
 * Return values: 0 - success, -1 - failure
 */
int calRow( Table *table, Table *tableRet, int rowNo, int rowNoRet, int colNo, ExpNode *conditionCols ) {
	if( !conditionCols ) return -1;
	// if node type is -1, it separate two columns, it's right child represent a column and it's left child is also a column list
	if( conditionCols->expType == -1 ) {
		if( conditionCols->leftChild && calRow( table, tableRet, rowNo, rowNoRet, colNo - 1, conditionCols->leftChild ) ) return -1;
		if( calRow( table, tableRet, rowNo, rowNoRet, colNo - 1, conditionCols->rightChild ) ) return -1;
		return 0;
	} else {
		Val val;
		Type type;
		if( cal( table, rowNo, conditionCols, val, type ) ) return -1;
		tableRet->colType[colNo] = max( type, tableRet->colType[colNo] );
		tableRet->data[rowNoRet][colNo] = val;
	}
	return 0;
}

/*
 * Function: get temporary table by the rows' number and the column expression tree
 * Passing in arguments: const char *tableName - table name, ExpNode *conditionCols - columns' expression tree,
 *                       vector<int> vecRow - selected rows' number
 * Return arguments: Table &tableRet - temporary table
 * Return values: 0 - success, -1 - failure
 */
int conditionTable( const char *tableName, ExpNode *conditionCols, vector<int> vecRow, Table &tableRet ) {
	if( !tableName ) return -1;
	Table table;
	if( read( tableName, &table ) ) return -1;
	// give temporary table a name
	strcpy( tableRet.tableName, "temp" );
	vector<string> vecCol; vecCol.clear();
	if( scanCols( conditionCols, vecCol ) ) return -1;
	// get temporary table's size
	tableRet.nRow = vecRow.size(); tableRet.nCol = vecCol.size();
	// get temporary table's column names and types
	tableRet.colType = ( int * )malloc( tableRet.nCol * sizeof( int ) );
	for( int i = 0; i < vecCol.size(); ++i ) {
		int colNo = table.colNo[vecCol[i]];
		tableRet.colName.push_back( table.colName[colNo] );
		tableRet.colType[i] = table.colType[colNo];
	}
	// write datas to the temporary table
	for( int i = 0; i < vecRow.size(); ++i ) {
		int row = vecRow[i] - 1;
		RowVal tVec( tableRet.nCol );
		tableRet.data.push_back( tVec );
		if( calRow( &table, &tableRet, row, i, tableRet.nCol, conditionCols ) ) return -1;
	}
	if( write( tableRet.tableName, &tableRet ) ) return -1;
	if( write( tableName, &table ) ) return -1;
	return 0;
}

/*
 * Function: delete rows by rows' number
 * Passing in arguments: const char *tableName - table name, vector<int> vecRow - selected rows' number
 * Return arguments: None
 * Return values: 0 - success, -1 - failure
 */
int delRowByNoVector( const char *tableName, vector<int> vecRow ) {
	if( !tableName ) return -1;
	Table table;
	if( read( tableName, &table ) ) return -1;
	sort( vecRow.begin(), vecRow.end(), greater<int>() );
	for( int i = 0; i < vecRow.size(); ++i ) {
		--table.nRow;
		// move all rows behind the row
		for( int j = vecRow[i] - 1; j < table.nRow; ++j ) {
			for( int k = 0; k < table.nCol; ++k ) {
				table.data[j][k] = table.data[j + 1][k];
				if( k == 0 ) table.rowNo[table.data[j][k]] = 0;
			}
		}
	}
	if( write( tableName, &table ) ) return -1;
	return 0;
}

/*
 * Function: update rows by rows' number and string expression-tree pair
 * Passing in arguments: const char *tableName - table name, vector<int> vecRow - selected rows' number
 *                       vector<PSE> vecCol - name expression-tree pair
 * Return arguments: None
 * Return values: 0 - success, -1 - failure
 */
int updRowByNoVector( const char *tableName, vector<int> vecRow, vector<PSE> vecCol ) {
	if( !tableName ) return -1;
	int tmp1, tmp2;
	Table table;
	if( read( tableName, &table ) ) return -1;
	for( int i = 0; i < vecRow.size(); ++i ) {
		// get row number
		tmp1 = vecRow[i] - 1;
		for( int j = 0; j < vecCol.size(); ++j ) {
			// get column number
			tmp2 = table.colNo[vecCol[j].first];
			Val val;
			Type type;
			// calculate column values
			if( cal( &table, tmp1, vecCol[j].second, val, type ) ) return -1;
			table.colType[tmp2] = max( type, table.colType[tmp2] );
			table.data[tmp1][tmp2] = val;
			// change primary key
			if( tmp2 == 0 ) {
				table.rowNo[table.data[tmp1][tmp2]] = 0;
				if( table.rowNo[table.data[tmp1][tmp2]] ) return -2;
				table.rowNo[table.data[tmp1][tmp2]] = i + 1;
			}
		}
	}
	if( write( tableName, &table ) ) return -1;
	return 0;
}

/*
 * Function: print table element
 * Passing in arguments: Val val - value to print, int allWidth - the width of the element
 * Return arguments: None
 * Return values: 0 - success, -1 - failure
 * Note: leave a space on value's both side
 */
int printTableElement( Val val, int allWidth ) {
	int leftSpace, rightSpace, valLen = val.length();
	leftSpace = ( allWidth - 2 - valLen ) / 2;
	rightSpace = allWidth - 2 - valLen - leftSpace;
	putchar( '|' );
	for( int i = 0; i < leftSpace; ++i ) putchar( ' ' );
	printf( "%s", val.c_str() );
	for( int i = 0; i < rightSpace; ++i ) putchar( ' ' );
	return 0;
}

/*
 * Function: print table to the console
 * Passing in arguments: const char *tableName - table name
 * Return arguments: None
 * Return values: 0 - success, -1 - failure
 */
int printTable( const char *tableName ) {
	if( !tableName ) return -1;
	Table table;
	if( read( tableName, &table ) ) return -1;
	int col = table.nCol, row = table.nRow;
	int maxColWidth[col], allColWidth = table.nCol + 1, valLen;
	// get all the columns' max width
	for( int j = 0; j < col; ++j ) {
		maxColWidth[j] = table.colName[j].length() + 2;
		for( int i = 0; i < row; ++i ) {
			valLen = table.data[i][j].length();
			maxColWidth[j] = max( maxColWidth[j], valLen + 2 );
		}
		// get total width of the table
		allColWidth += maxColWidth[j];
	}
	// get table name's length
	valLen = strlen( table.tableName );
	// print table
	putchar( '+' ); for( int i = 1; i < allColWidth - 1; ++i ) putchar( '-' ); puts( "+" );
	printTableElement( table.tableName, allColWidth ); puts( "|" );
	putchar( '+' ); for( int i = 1; i < allColWidth - 1; ++i ) putchar( '-' ); puts( "+" );
	for( int i = 0; i < col; ++i ) printTableElement( table.colName[i], maxColWidth[i] + 2 ); puts( "|" );
	putchar( '+' ); for( int i = 1; i < allColWidth - 1; ++i ) putchar( '-' ); puts( "+" );
	for( int i = 0; i < row; ++i ) {
		for( int j = 0; j < col; ++j ) printTableElement( table.data[i][j], maxColWidth[j] + 2 ); puts( "|" ); 
		putchar( '+' ); for( int i = 1; i < allColWidth - 1; ++i ) putchar( '-' ); puts( "+" );
	}
	if( write( tableName, &table ) ) return -1;
	return 0;
}

/*
 * Function: print expression to the console, just for debug
 * Passing in arguments: ExpNode *root - expression tree, int level - expression tree node level
 * Return arguments: None
 * Return values: 0 - success, -1 - failure
 */
int printExpTree( ExpNode *root, int level ) {
	if( !root ) return -1;
	putchar( '|' ); for( int i = 0; i < level; ++i ) printf( "--" ); putchar( ' ' );
	if( root->leftChild ) printExpTree( root->leftChild, level + 1 );
	if( root->rightChild ) printExpTree( root->rightChild, level + 1 );
	return 0;
}
