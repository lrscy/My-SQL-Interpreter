#include "sqlTest.h"

int main() {
	char sqlInput[MAXN];
	string sql; sql.clear();
	while( true ) {
		printf( "SQL>" );
		memset( sqlInput, 0, sizeof( sqlInput ) );
		fgets( sqlInput, MAXN, stdin );
		sql += sqlInput;
		if( sql.find( ';' ) == string::npos ) { sql += " "; continue; }
		// sql syntax parsing
		if( sqlTransParse( sql.c_str() ) != 0 ) puts( "Syntax Error!\n" );
		else {
			if( sqlAction == sqlQUIT ) {
				if( sqlTransQuit() ) puts( "QUIT ERROR!" );
				else break;
			}
			if( sqlAction == sqlHELP ) { 
				if( sqlTransHelp() ) puts( "HELP ERROR!" );
		   	}
			if( sqlAction == sqlCREATE_TABLE ) { 
				if( sqlTransCreateTable() ) puts( "CREATE TABLE ERROR!" );
		   	}
			if( sqlAction == sqlDROP_TABLE ) { 
				if( sqlTransDrop() ) puts( "DROP ERROR!" );
			}
			if( sqlAction == sqlSELECT ) {
				if( sqlTransSelectFromWhere() ) puts( "SELECT ERROR!" );
			}
			if( sqlAction == sqlINSERT ) {
				if( sqlTransInsertIntoValues() ) puts( "INSERT ERROR!" );
			}
			if( sqlAction == sqlUPDATE ) {
				if( sqlTransUpdateSetWhere() ) puts( "UPDATE ERROR" ); 
			}
			if( sqlAction == sqlDELETE ) {
				if( sqlTransDeleteFromWhere() ) puts( "DELETE ERROR" );
			}
		}
		sql.clear();
	}
	return 0;
}
