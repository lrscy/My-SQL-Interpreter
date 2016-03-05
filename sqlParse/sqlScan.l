%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "syntaxTree.h"
#include "y.tab.h"

int createKeywordNode( char *pKeywordName );
int createNameNode( char *pName );
int createIntConstNode( int nVal );
int createDoubleConstNode( double dVal );
int createStringConstNode( char *pStrVal );
int createOperatorNode( char *pOperator );
int createBoundNode( char *pName );

%}

%%

HELP {
	if( !createKeywordNode( "HELP" ) ) return 0;
	return HELP;
}

QUIT {
	if( !createKeywordNode( "QUIT" ) ) return 0;
	return QUIT;
}

CREATE {
	if( !createKeywordNode( "CREATE" ) ) return 0;
	return CREATE;
}

TABLE {
	if( !createKeywordNode( "TABLE" ) ) return 0;
	return TABLE;
}

DROP {
	if( !createKeywordNode( "DROP" ) ) return 0;
	return DROP;
}

SELECT {
	if( !createKeywordNode( "SELECT" ) ) return 0;
	return SELECT;
}

FROM {
	if( !createKeywordNode( "FROM" ) ) return 0;
	return FROM;
}

WHERE {
	if( !createKeywordNode( "WHERE" ) ) return 0;
	return WHERE;
}

INSERT {
	if( !createKeywordNode( "INSERT" ) ) return 0;
	return INSERT;
}

INTO {
	if( !createKeywordNode( "INTO" ) ) return 0;
	return INTO;
}

VALUES {
	if( !createKeywordNode( "VALUES" ) ) return 0;
	return VALUES;
}

UPDATE {
	if( !createKeywordNode( "UPDATE" ) ) return 0;
	return UPDATE;
}

SET {
	if( !createKeywordNode( "SET" ) ) return 0;
	return SET;
}

DELETE {
	if( !createKeywordNode( "DELETE" ) ) return 0;
	return DELETE;
}

INT {
	if( !createKeywordNode( "INT" ) ) return 0;
	return INT;
}

DOUBLE {
	if( !createKeywordNode( "DOUBLE" ) ) return 0;
	return DOUBLE;
}

STRING {
	if( !createKeywordNode( "STRING" ) ) return 0;
	return STRING;
}

NULL {
	if( !createKeywordNode( "NULLX" ) ) return 0;
	return NULLX;
}

AND {
	if( !createKeywordNode( "AND" ) ) return 0;
	return AND;
}

OR {
	if( !createKeywordNode( "OR" ) ) return 0;
	return OR;
}

NOT {
	if( !createKeywordNode( "NOT" ) ) return 0;
	return NOT;
}

[a-zA-Z][a-zA-Z0-9_]* {
	if( !createNameNode( yytext ) ) return 0;
	return NAME;
}

[0-9]+ {
	int nVal;
	sscanf( yytext, "%d", &nVal );
	if( !createIntConstNode( nVal ) ) return 0;
	return INTVAL;
}

[0-9]+"."[0-9]* {
	double dVal;
	sscanf( yytext, "%lf", &dVal );
	if( !createDoubleConstNode( dVal ) ) return 0;
	return DOUBLEVAL;
}

'[^'\n]*' {
	if( !createStringConstNode( yytext ) ) return 0;
	return STRINGVAL;
}

[ \t\r\n]+ { }

[;] {
	createBoundNode( yytext );
	return 0;
}

[(),.] {
	if( !createBoundNode( yytext ) ) return 0;
	return yytext[0];
}

"+" |
"-" |
"*" |
"/" {
	if( !createOperatorNode( yytext ) ) return 0;
	return yytext[0];
}

"=" |
"<" |
">" |
"<>" |
"<=" |
">=" {
	if( !createOperatorNode( yytext ) ) return 0;
	return COMPARISON;
}

. {
	printf( "ILLEGLE STRING: %s\n", yytext );
	return 0;
}

%%

int createKeywordNode( char *pKeywordName ) {
	if( !pKeywordName ) {
		printf( "KEYWORD NULL\n" );
		return 0;
	}
	int len = strlen( pKeywordName );
	struct TNode *p = mallocNode();
	char *pStr = NULL;
	if( !p ) {
		printf( "Create TNode Fail!\n" );
		return 0;
	}
	pStr = ( char * )malloc( len + 1 );
	p->mType = KEYWORD;
	strcpy( pStr, pKeywordName );
	pStr[len] = 0;
	p->mStrName = pStr;
	yylval.pNode = p;
	return 1;
}

int createNameNode( char *pName ) {
	if( !pName ) {
		printf( "NAME NULL\n" );
		return 0;
	}
	int len = strlen( pName );
	struct TNode *p = mallocNode();
	char *pStr = NULL;
	if( !p ) {
		printf( "Create TNode Fail!\n" );
		return 0;
	}
	pStr = ( char * )malloc( len + 1 );
	p->mType = ID;
	strcpy( pStr, pName );
	pStr[len] = 0;
	p->mStrName = pStr;
	yylval.pNode = p;
	return 1;
}

int createBoundNode( char *pName ) {
	if( !pName ) {
		printf( "BOUND NULL\n" );
		return 0;
	}
	int len = strlen( pName );
	struct TNode *p = mallocNode();
	char *pStr = NULL;
	if( !p ) {
		printf( "Create TNode Fail!\n" );
		return 0;
	}
	pStr = ( char * )malloc( len + 1 );
	p->mType = BOUND;
	strcpy( pStr, pName );
	pStr[len] = 0;
	p->mStrName = pStr;
	yylval.pNode = p;
	return 1;
}

int createIntConstNode( int nVal ) {
	struct TNode *p = mallocNode();
	if( !p ) {
		printf( "Create TNode Fail!\n" );
		return 0;
	}
	p->mType = TYPE_INT;
	p->mVal.valInt = nVal;
	yylval.pNode = p;
	return 1;
}

int createDoubleConstNode( double dVal ) {
	struct TNode *p = mallocNode();
	if( !p ) {
		printf( "Create TNode Fail!\n" );
		return 0;
	}
	p->mType = TYPE_DOUBLE;
	p->mVal.valDouble = dVal;
	yylval.pNode = p;
	return 1;
}

int createStringConstNode( char *pStrVal ) {
	if( !pStrVal ) {
		printf( "STRING VALUE NULL!\n" );
		return 0;
	}
	int len = strlen( pStrVal );
	struct TNode *p = mallocNode();
	char *pStr = NULL;
	if( !p ) {
		printf( "Create TNode Fail!\n" );
		return 0;
	}
	pStr = ( char * )malloc( len + 1 );
	p->mType = TYPE_STRING;
	strcpy( pStr, pStrVal );
	pStr[len] = 0;
	p->mVal.valStr = pStr;
	yylval.pNode = p;
	return 1;
}

int createOperatorNode( char *pOperator ) {
	if( !pOperator ) {
		printf( "OPERATOR NULL\n" );
		return 0;
	}
	int len = strlen( pOperator );
	struct TNode *p = mallocNode();
	char *pStr = NULL;
	if( !p ) {
		printf( "Create TNode Fail!\n" );
		return 0;
	}
	pStr = ( char * )malloc( len + 1 );
	p->mType = OPERATOR;
	strcpy( pStr, pOperator );
	pStr[len] = 0;
	p->mStrName = pStr;
	yylval.pNode = p;
	return 1;
}

int yywrap() {
	return 1;
}
