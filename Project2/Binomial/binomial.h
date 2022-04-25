#ifndef H_BI
#define H_BI
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
struct BinoNode
{
    int position;
    int key;
    int degree;
    struct BinoNode *p;
    struct BinoNode *child;//left-child
    struct BinoNode *sibling;
};
typedef struct BinoNode * hNode;
typedef struct heap
{
    struct BinoNode * root;
}* binomialHeap;

//hNode create_hNode(int key);
hNode create_hNode(int key, int position);
binomialHeap createBH();
hNode bhMinimum(binomialHeap h);
void bhLink(hNode node1, hNode node2);//link node1 and node2 together, node2 becomes a root.
hNode bhMerge(binomialHeap h1, binomialHeap h2);//merge h1,h2
binomialHeap bhUnion(binomialHeap * h1, binomialHeap * h2);
//binomialHeap bhInsert(binomialHeap h1, int key);//insert node X into bh
binomialHeap bhInsert(binomialHeap h1, hNode node);
binomialHeap bhExtractMin(binomialHeap * h);//find the min
void bhDecreaseKey(binomialHeap h, hNode x, int key);
void bhDelete(binomialHeap h, hNode x);
void traverseBH(hNode t);//traverse the BH
#endif