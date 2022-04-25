#ifndef _FIBONACCI_H_
#define _FIBONACCI_H_
#include <stdio.h>
#include <stdlib.h>

//typedef enum{false, true} bool;

struct FibNode; //the node of Fibnacci heap's node which stores the detailed value
struct FibHeap; //the node Fibonacci heap which stores the whole situation of heap
typedef struct FibNode* pFibNode;   //the pointer towards node
typedef struct FibHeap* pFibHeap;   //the pointer towards heap

pFibHeap makeFibHeap(); //build an empty heap
pFibNode getFibNode(int value, int position);   //get a node with single value
pFibHeap insertToFibHeap(pFibHeap heap, pFibNode node); //insert a node into heap
pFibHeap decreaseFibHeap(pFibHeap heap, pFibNode node, int value);  //decrease a certain node's value
pFibNode deleteMinFibHeap(pFibHeap heap);   //delete min node from the heap

typedef struct FibNode{
    int key;    //value of the node
    int degree; //the number of children of the node
    int position;   //the position of the vertice in the Dijkstra map
    bool mark;      //whether the node has lost its child after the last time it becomes a child 
    pFibNode p;     //pointer towards the node's parent
    pFibNode left;  //pointer towards the node's left sibling
    pFibNode right; //pointer towards the node's right sibling
    pFibNode child; //pointer towards one of the node's children
} FibNode;
typedef struct FibHeap{
    int totalNode;  //total number of the nodes
    pFibNode min;   //minimum node of the whole heap
    pFibNode *degreeArray;  //array stores trees of each degree
} FibHeap;

//************assistance functions************//
//****functions helping "deleteMinFibHeap"****//
void moveChildToRoots(pFibHeap heap, pFibNode parent);  //move the children of a (root)node into roots
void ConSolidDate(pFibHeap heap);   //merge all trees of the same degree
pFibNode removeFromRoot(pFibHeap heap, pFibNode node);  //remove node from root and make it as a single tree
//*****functions helping "decreaseFibHeap"*****//
pFibNode linkFibNode(pFibHeap heap, pFibNode x, pFibNode y);    //link node y as x's child
void Cut(pFibHeap heap, pFibNode pchild, pFibNode pparent);     //cut node pchild and link it to the root
void Cascading_Cut(pFibHeap heap, pFibNode node);       //continue to cut nodes until we get to false or root
#endif