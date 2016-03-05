%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "syntaxTree.h"

struct TNode *createNonTerminalNode( char *pName );

%}

%union {
	struct TNode *pNode;
}
%token <pNode> HELP
%token <pNode> QUIT
%token <pNode> CREATE
%token <pNode> TABLE
%token <pNode> DROP
%token <pNode> SELECT
%token <pNode> FROM
%token <pNode> WHERE
%token <pNode> INSERT
%token <pNode> INTO
%token <pNode> VALUES
%token <pNode> UPDATE
%token <pNode> SET
%token <pNode> DELETE
%token <pNode> NAME
%token <pNode> INTVAL
%token <pNode> DOUBLEVAL
%token <pNode> STRINGVAL
%token <pNode> INT
%token <pNode> DOUBLE
%token <pNode> STRING
%token <pNode> NULLX
%token <pNode> COMPARISON
%token <pNode> ','
%token <pNode> '.'
%token <pNode> '('
%token <pNode> ')'

%left <pNode> OR
%left <pNode> AND
%right <pNode> NOT
%left <pNode> '+' '-'
%left <pNode> '*' '/'

%type <pNode> base_table_define
%type <pNode> drop_statement
%type <pNode> select_statement
%type <pNode> insert_statement
%type <pNode> update_statement
%type <pNode> delete_statement
%type <pNode> help_statement
%type <pNode> table
%type <pNode> base_table_element_list
%type <pNode> base_table_element
%type <pNode> column_define
%type <pNode> column
%type <pNode> data_type
%type <pNode> selection
%type <pNode> scalar_extend_list
%type <pNode> scalar_extend
%type <pNode> atom
%type <pNode> literal
%type <pNode> column_var
%type <pNode> table_extend
%type <pNode> from_clause
%type <pNode> table_reference_list
%type <pNode> table_reference
%type <pNode> opt_where_clause 
%type <pNode> where_clause
%type <pNode> search_condition
%type <pNode> predicate 
%type <pNode> comparison_predicate
%type <pNode> opt_column_list
%type <pNode> column_list
%type <pNode> values_or_query
%type <pNode> insert_atom_list
%type <pNode> insert_atom
%type <pNode> assignment_list
%type <pNode> assignment

%start sql

%%

sql: 
	base_table_define {
		struct TNode *p = createNonTerminalNode( "sql" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		syntaxTree = p;
		sqlAction = sqlCREATE_TABLE;
	}
	| drop_statement {
		struct TNode *p = createNonTerminalNode( "sql" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		syntaxTree = p;
		sqlAction = sqlDROP_TABLE;
	}
	| select_statement {
		struct TNode *p = createNonTerminalNode( "sql" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		syntaxTree = p;
		sqlAction = sqlSELECT;
	}
	| insert_statement {
		struct TNode *p = createNonTerminalNode( "sql" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		syntaxTree = p;
		sqlAction = sqlINSERT;
	}
	| update_statement {
		struct TNode *p = createNonTerminalNode( "sql" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		syntaxTree = p;
		sqlAction = sqlUPDATE;
	}
	| delete_statement {
		struct TNode *p = createNonTerminalNode( "sql" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		syntaxTree = p;
		sqlAction = sqlDELETE;
	}
	| QUIT {
		struct TNode *p = createNonTerminalNode( "sql" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		syntaxTree = p;
		sqlAction = sqlQUIT;
	}
	| help_statement {
		struct TNode *p = createNonTerminalNode( "sql" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		syntaxTree = p;
		sqlAction = sqlHELP;
	}
	;

help_statement:
	HELP {
		struct TNode *p = createNonTerminalNode( "help_statement" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

base_table_define:
	CREATE TABLE table '(' base_table_element_list ')' {
		struct TNode *p = createNonTerminalNode( "base_table_define" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $4 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $5 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $6 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

table:
	NAME {
		struct TNode *p = createNonTerminalNode( "table" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| NAME '.' NAME {
		struct TNode *p = createNonTerminalNode( "table" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

base_table_element_list:
	base_table_element {
		struct TNode *p = createNonTerminalNode( "base_table_element_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| base_table_element_list ',' base_table_element {
		struct TNode *p = createNonTerminalNode( "base_table_element_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

base_table_element:
	column_define {
		struct TNode *p = createNonTerminalNode( "base_table_element" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

column_define:
	column data_type {
		struct TNode *p = createNonTerminalNode( "column_define" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

column:
	NAME {
		struct TNode *p = createNonTerminalNode( "column" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

data_type:
	INT {
		struct TNode *p = createNonTerminalNode( "data_type" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| DOUBLE {
		struct TNode *p = createNonTerminalNode( "data_type" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| STRING {
		struct TNode *p = createNonTerminalNode( "data_type" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

drop_statement:
	DROP TABLE table {
		struct TNode *p = createNonTerminalNode( "drop_statement" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

select_statement:
	SELECT selection table_extend {
		struct TNode *p = createNonTerminalNode( "select_statement" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

selection:
	'*' {
		struct TNode *p = createNonTerminalNode( "selection" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| scalar_extend_list {
		struct TNode *p = createNonTerminalNode( "selection" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

scalar_extend_list:
	scalar_extend {
		struct TNode *p = createNonTerminalNode( "scalar_extend_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| scalar_extend_list','scalar_extend {
		struct TNode *p = createNonTerminalNode( "scalar_extend_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

scalar_extend:
	scalar_extend'+'scalar_extend {
		struct TNode *p = createNonTerminalNode( "scalar_extend" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| scalar_extend'-'scalar_extend {
		struct TNode *p = createNonTerminalNode( "scalar_extend" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| scalar_extend'*'scalar_extend {
		struct TNode *p = createNonTerminalNode( "scalar_extend" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| scalar_extend'/'scalar_extend {
		struct TNode *p = createNonTerminalNode( "scalar_extend" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| '('scalar_extend')' {
		struct TNode *p = createNonTerminalNode( "scalar_extend" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| atom {
		struct TNode *p = createNonTerminalNode( "scalar_extend" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| column_var {
		struct TNode *p = createNonTerminalNode( "scalar_extend" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

atom:
	literal {
		struct TNode *p = createNonTerminalNode( "atom" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

literal:
	INTVAL {
		struct TNode *p = createNonTerminalNode( "literal" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| DOUBLEVAL {
		struct TNode *p = createNonTerminalNode( "literal" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| STRINGVAL {
		struct TNode *p = createNonTerminalNode( "literal" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

column_var:
	NAME {
		struct TNode *p = createNonTerminalNode( "column_var" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| NAME'.'NAME {
		struct TNode *p = createNonTerminalNode( "column_var" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

table_extend:
	from_clause opt_where_clause {
		struct TNode *p = createNonTerminalNode( "table_extend" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

from_clause:
	FROM table_reference_list {
		struct TNode *p = createNonTerminalNode( "from_clause" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

table_reference_list:
	table_reference {
		struct TNode *p = createNonTerminalNode( "table_reference_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| table_reference_list','table_reference {
		struct TNode *p = createNonTerminalNode( "table_reference_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

table_reference:
	table {
		struct TNode *p = createNonTerminalNode( "table_reference" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

opt_where_clause:
	{
		struct TNode *p = createNonTerminalNode( "opt_where_clause" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		$$ = p;
	}
	| where_clause {
		struct TNode *p = createNonTerminalNode( "opt_where_clause" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

where_clause:
	WHERE search_condition {
		struct TNode *p = createNonTerminalNode( "where_clause" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

search_condition:
	search_condition OR search_condition {
		struct TNode *p = createNonTerminalNode( "search_condition" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| search_condition AND search_condition {
		 struct TNode *p = createNonTerminalNode( "search_condition" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| NOT search_condition {
		struct TNode *p = createNonTerminalNode( "search_condition" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| '(' search_condition ')' {
		struct TNode *p = createNonTerminalNode( "search_condition" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| predicate {
		struct TNode *p = createNonTerminalNode( "search_condition" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

predicate:
	comparison_predicate {
		struct TNode *p = createNonTerminalNode( "predicate" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

comparison_predicate:
	scalar_extend COMPARISON scalar_extend {
		struct TNode *p = createNonTerminalNode( "comparison_predicate" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

insert_statement:
	INSERT INTO table opt_column_list values_or_query {
		struct TNode *p = createNonTerminalNode( "insert_statement" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $4 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $5 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

opt_column_list:
	{
		struct TNode *p = createNonTerminalNode( "opt_column_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		$$ = p;
	}
	| '(' column_list ')' {
		struct TNode *p = createNonTerminalNode( "opt_column_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

column_list:
	column {
		struct TNode *p = createNonTerminalNode( "column_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| column_list','column {
		struct TNode *p = createNonTerminalNode( "column_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

values_or_query:
	VALUES'('insert_atom_list')' {
		struct TNode *p = createNonTerminalNode( "values_or_query" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $4 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

insert_atom_list:
	insert_atom {
		struct TNode *p = createNonTerminalNode( "insert_atom_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| insert_atom_list','insert_atom {
		struct TNode *p = createNonTerminalNode( "insert_atom_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

insert_atom:
	atom {
		struct TNode *p = createNonTerminalNode( "insert_atom" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| NULLX {
		struct TNode *p = createNonTerminalNode( "insert_atom" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

update_statement:
	UPDATE table SET assignment_list opt_where_clause {
		struct TNode *p = createNonTerminalNode( "update_statement" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $4 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $5 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

assignment_list:
	assignment {
		struct TNode *p = createNonTerminalNode( "assignment_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| assignment_list','assignment {
		struct TNode *p = createNonTerminalNode( "assignment_list" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

assignment:
	column COMPARISON scalar_extend {
		struct TNode *p = createNonTerminalNode( "assignment" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( $2 == NULL ) return 1;
		if( strcmp( $2->mStrName, "=" ) ) return 1;
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	| column COMPARISON NULLX {
		struct TNode *p = createNonTerminalNode( "assignment" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( $2 == NULL ) return 1;
		if( strcmp( $2->mStrName, "=" ) ) return 1;
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

delete_statement:
	DELETE FROM table opt_where_clause {
		struct TNode *p = createNonTerminalNode( "delete_statement" );
		if( !p ) {
			printf( "error:createNonTerminalNode\n" );
			return 1;
		}
		if( !appendChild( p, $1 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $2 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $3 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		if( !appendChild( p, $4 ) ) {
			printf( "error:appendChild\n" );
			return 1;
		}
		$$ = p;
	}
	;

%%

struct TNode *createNonTerminalNode( char *pName ) {
	if( !pName ) return NULL;
	int len = strlen( pName );
	struct TNode *p = mallocNode();
	char *pStr = NULL;
	if( !p ) {
		printf( "Create TNode Fail\n" );
		return NULL;
	}
	pStr = ( char * )malloc( len + 1 );
	p->mType = BRANCH;
	strcpy( pStr, pName );
	pStr[len] = 0;
	p->mStrName = pStr;
	return p;
}

int yyerror( char *str ) {
	printf( "%s\n", str );
	exit( 1 );
}

typedef YY_BUFFER_STATE;

int sqlParse( const char *sql ) {
	if( !sql ) printf( "No input SQL query\n" );
	int len = strlen( sql );
	YY_BUFFER_STATE state = yy_scan_bytes( sql, len );
	yy_switch_to_buffer( state );
	int n = yyparse();
	yy_delete_buffer( state );
	return n;
}
