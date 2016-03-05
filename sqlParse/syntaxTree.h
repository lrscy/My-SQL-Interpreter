#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <stdlib.h>
#include <stdio.h>

#ifdef __CPLUSPLUS
extern "C" {
#endif

enum sqlNodeType { BRANCH = 0, KEYWORD, OPERATOR, ID, TYPE_INT, TYPE_DOUBLE, TYPE_STRING, BOUND };
enum sqlActionType { sqlCREATE_TABLE = 0, sqlDROP_TABLE, sqlSELECT, sqlINSERT, sqlUPDATE, sqlDELETE, sqlQUIT, sqlHELP };

// syntax tree node, child brother representation
struct TNode {
	int mType;
	char *mStrName;
	union valType {
		int valInt;
		char *valStr;
		double valDouble;
	} mVal;
	struct TNode *mpChildNode, *mpBrotherNode;
};
// declare global variable
extern struct TNode *syntaxTree;
extern enum sqlActionType sqlAction;

// malloc space for new node
struct TNode *mallocNode();

// free space
void freeNode( struct TNode *p );

// append child to a node
int appendChild( struct TNode *pParent, struct TNode *pChild );

// get child by its serial number
struct TNode *getChild( struct TNode *pParent, int pos );

// get child number of a node
int getChildNum( struct TNode *pParent );

// destory syntax tree
void destorySyntaxTree( struct TNode *p );

// print syntax tree, just for debug
void printTree( struct TNode *root, int level );

#ifdef __CPLUSPLUS
}
#endif

#endif // SYNTAXTREE_H
