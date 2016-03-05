#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "syntaxTree.h"

struct TNode *syntaxTree;
enum sqlActionType sqlAction;

/*
 * Function: malloc space for new node
 * Passing in arguments: None
 * Return arguments: None
 * Return value: struct TNode * - struct TNode pointer
 */
struct TNode *mallocNode() {
	struct TNode *p = ( struct TNode * )malloc( sizeof( struct TNode ) );
	if( !p ) return NULL;
	p->mType = 0;
	p->mStrName = NULL;
	p->mVal.valInt = 0;
	p->mVal.valStr = NULL;
	p->mVal.valDouble = 0;
	p->mpChildNode = NULL;
	p->mpBrotherNode = NULL;
	return p;
}

/*
 * Function: free space
 * Passing in arguments: struct TNode *p - node pointer
 * Return arguments: None
 * Return value: None
 */
void freeNode( struct TNode *p ) {
	if( p ) free( p );
	p = NULL;
	return ;
}

/*
 * Function: append child to a node
 * Passing in arguments: struct TNode *pParent - parent node pointer,
 *                       struct TNode *pChild - child node to be appended
 * Return arguments: None
 * Return value: 1 - success, 0 - failure
 */
int appendChild( struct TNode *pParent, struct TNode *pChild ) {
	if( !pChild ) {
		printf( "Append Failed!\n" );
		return 0;
	}
	if( !pParent ) {
		printf( "No Parent!\n" );
		return 0;
	}
	// get child node
	struct TNode *p = pParent->mpChildNode;
	if( !p ) {
		pParent->mpChildNode = pChild;
		return 1;
	}
	// get last brother node
	while( p->mpBrotherNode ) p = p->mpBrotherNode;
	p->mpBrotherNode = pChild;
	return 1;
}

/*
 * Function: get child by its serial number
 * Passing in arguments: struct TNode *pParent - parent node pointer,
 *                       int pos - serial number of the child
 * Return arguments: None
 * Return value: struct TNode * - child node
 */
struct TNode *getChild( struct TNode *pParent, int pos ) {
	if( !pParent ) return NULL;
	struct TNode *p = pParent->mpChildNode;
	int ret = 0;
	while( p ) {
		++ret;
		if( ret == pos ) return p;
		p = p->mpBrotherNode;
	}
	return NULL;
}

/*
 * Function: get child number of a node
 * Passing in arguments: struct TNode *pParent - parent node pointer,
 * Return arguments: None
 * Return value: child number
 */
int getChildNum( struct TNode *pParent ) {
	if( !pParent ) return 0;
	struct TNode *p = pParent->mpChildNode;
	int ret = 0;
	while( p ) {
		++ret;
		p = p->mpBrotherNode;
	}
	return ret;
}

/*
 * Function: destory syntax tree
 * Passing in arguments: struct TNode *p - TNode pointer
 * Return arguments: None
 * Return value: None
 */
void destorySyntaxTree( struct TNode *p ) {
	if( !p ) return ;
	struct TNode *p1, *p2;
	p1 = p->mpChildNode;
	while( p1 ) {
		p2 = p1->mpBrotherNode;
		destorySyntaxTree( p1 );
		p1 = p2;
	}
	p->mpChildNode = NULL;
	p->mpBrotherNode = NULL;
	if( p->mType == TYPE_STRING ) {
		if( p->mVal.valStr ) {
			free( p->mVal.valStr );
			p->mVal.valStr = NULL;
		}
	}
	free( p );
	return ;
}

/*
 * Function: print syntax tree, just for debug
 * Passing in arguments: struct TNode *root - TNode pointer, int level - node level
 * Return arguments: None
 * Return value: None
 */
void printTree( struct TNode *root, int level ) {
	if( !root ) return ;
	int i;
	putchar( '|' );
	for( i = 0; i < level; ++i ) printf( "--" );
	putchar( ' ' );
	if( root->mType == TYPE_STRING ) printf( "%s\n", root->mVal.valStr );
	else if( root->mType == TYPE_INT ) printf( "%d\n", root->mVal.valInt );
	else if( root->mType == TYPE_DOUBLE ) printf( "%.2f\n", root->mVal.valDouble );
	else printf( "%s\n", root->mStrName );
	printTree( root->mpChildNode, level + 1 );
	printTree( root->mpBrotherNode, level );
	return ;
}
