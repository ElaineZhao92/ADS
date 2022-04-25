#include "Fibonacci.h"

//make a new Fibonacci heap
//return the pointer towards it
pFibHeap makeFibHeap()
{
    pFibHeap heap;
    heap = (pFibHeap) malloc (sizeof(FibHeap));
    heap->degreeArray = NULL;
    heap->min = NULL;
    heap->totalNode = 0;
    return heap;
}

//get a node with certain values
//return a pointer towards it
pFibNode getFibNode(int value, int position)
{
    pFibNode node;
    node = (pFibNode) malloc (sizeof(FibNode));
    node->degree = 0;
    node->key = value;
    node->position = position;
    node->child = node->p = NULL;   //it has no parent or child
    node->left = node->right = node;    //it has no siblings
    node->mark = false;
    return node;
}

//insert a node into a heap
//return the pointer towards the heap
pFibHeap insertToFibHeap(pFibHeap heap, pFibNode node)
{
    heap->totalNode++;  //add the number of nodes
    if(heap->min == NULL)   //insert NULL heap's first node
        heap->min = node;
    else    //now we link the node to the heap's roots
    {
        node->left = heap->min; //min node is sure to be one of the roots
        node->right = heap->min->right;
        heap->min->right->left = node;
        heap->min->right = node;
        if(heap->min->key > node->key)  //update min is necessary
            heap->min = node;
    }
    return heap;
}

//decrease the value of a certain node in a heap
//return the pointer towards the heap
//attention: We must make sure the new value is less than the node's value before calling it!
pFibHeap decreaseFibHeap(pFibHeap heap, pFibNode node, int value)
{
    node->key = value;  //update the node as smaller value
    pFibNode ptr = node->p; 
    //when the node is not root and its parent has larger key
    if(ptr && ptr->key > node->key)
    {
        Cut(heap, node, ptr);   //move the node to the root
        Cascading_Cut(heap, ptr);   //continue to cut nodes into roots
    }
    if(node->key < heap->min->key)  //update the min if necessary
        heap->min = node;
    return heap;
}

//delete the minimum node in the heap
//return the pointer towards the node
//it's advised to free the node after you use it and it becomes useless
pFibNode deleteMinFibHeap(pFibHeap heap)
{
    pFibNode ptr;
    pFibNode minnode = heap->min;
    moveChildToRoots(heap, heap->min);  //min hasn't been removed now
    ptr = heap->min;
    ptr->right->left = ptr->left;   //remove the min node from the heap
    ptr->left->right = ptr->right;
    heap->totalNode--;  //reduce the number of nodes
    if(ptr == ptr->right)   //the heap has a single node "min"
        heap->min = NULL;
    else 
    {
        heap->min = heap->min->right;   //we'll make it points at proper one soon
        ConSolidDate(heap); //merge the trees of the same degree
    }
    return minnode;
}

//************assistance functions************//
//****functions helping "deleteMinFibHeap"****//

//move the children of node parent to the root of heap
//In practice we only use it to remove a root's children
void moveChildToRoots(pFibHeap heap, pFibNode parent)
{
    pFibNode ptr;
    while(parent->child != NULL)    //until all children of parent is removed
    {
        ptr = parent->child;
        if(ptr->right == ptr)   //parent has a single child
            parent->child = NULL;   //now parent has no child
        else    //parent has multiple children
            parent->child = ptr->right; //prepare to move the next child

        //remove child ptr from children lists
        ptr->left->right = ptr->right;
        ptr->right->left = ptr->left;
        //link ptr to roots
        ptr->left = heap->min;
        ptr->right = heap->min->right;
        heap->min->right->left = ptr;
        heap->min->right = ptr;
        ptr->p = NULL;  //update its parent
    }
    parent->degree = 0; //now it has no child anymore
}

//merge the trees of the same degree
void ConSolidDate(pFibHeap heap)
{
    int logn = 0, i;
    double tot = (double) heap->totalNode;
    while(tot >= 1.5)
    {
        logn++;
        tot /= 1.5;
    }   //estimate the maxDegree after merging
    heap->degreeArray = (pFibNode *) calloc (logn+1, sizeof(pFibNode));
    pFibNode ptr = heap->min, ptr2;
    int d;  //degree of the tree
    while(heap->min != NULL)    //while we remove all the trees
    {
        ptr = removeFromRoot(heap, heap->min);  //remove a tree from the roots
        d = ptr->degree;
        while(heap->degreeArray[d] != NULL) //until we have merged all the trees to merge this time
        {
            ptr2 = heap->degreeArray[d];
            //link the larger one to the smaller one
            if(ptr->key < ptr2->key)
                ptr = linkFibNode(heap, ptr, ptr2);
            else
                ptr = linkFibNode(heap, ptr2, ptr);
            heap->degreeArray[d++] = NULL;  //now we have no tree of this degree, and get a tree of d+1
        }
        heap->degreeArray[d] = ptr; //now we get a tree of degree d
    }
    for(i = 0; i <= logn; i++)
    {
        if(!heap->degreeArray[i])
            continue;
        //link the roots again
        if(heap->min == NULL)
            heap->min = heap->degreeArray[i];
        else
        {
            heap->degreeArray[i]->left = heap->min;
            heap->degreeArray[i]->right = heap->min->right;
            heap->min->right->left = heap->degreeArray[i];
            heap->min->right = heap->degreeArray[i];
            if(heap->min->key > heap->degreeArray[i]->key)
                heap->min = heap->degreeArray[i];
        }
    }
    free(heap->degreeArray);    //free the memory
}
//remove a tree from the heap
//return the pointer towards the tree
pFibNode removeFromRoot(pFibHeap heap, pFibNode node)
{
    pFibNode ptr = node;
    if(ptr == heap->min)    //remove the heap's min
    {
        if(ptr->right == ptr)   //when it is the last tree
            heap->min = NULL;
        else heap->min = ptr->right;    //we don't care what it is pointing at in practice
    }
    //remove it from the roots list
    ptr->right->left = ptr->left;
    ptr->left->right = ptr->right;
    ptr->right = ptr->left = ptr;
    return ptr;
}
//link y as x's child
//return the pointer towards node x who is the parent
//we always call it when x and y are both roots
pFibNode linkFibNode(pFibHeap heap, pFibNode x, pFibNode y)
{
    if(heap->min == y)  heap->min = x;  //move min if necessary
    //remove y from the children list of its parent
    y->left->right = y->right;
    y->right->left = y->left;
    y->p = x;   //now y becomes x's child
    if(x->child)    //x have child
    {
        //link y to the children
        y->right = x->child->right;
        y->left = x->child;
        x->child->right->left = y;
        x->child->right = y;
    }
    else //when x has no child, add y directly
    {
        x->child = y;
        y->left = y->right = y;
    }
    x->degree++;
    y->mark = false;    //y is a child once more
    return x;
}

//*****functions helping "decreaseFibHeap"*****//
//remove pchild into the root list
void Cut(pFibHeap heap, pFibNode pchild, pFibNode pparent)
{
    pparent->degree--;
    if(pparent->child == pchild)
    {
        if(pchild->right == pchild) //one child
            pparent->child = NULL;
        else pparent->child = pchild->right;    //parent has other children
    }
    //remove child from children list
    pchild->right->left = pchild->left;
    pchild->left->right = pchild->right;
    //add children to roots list
    pchild->p = NULL;
    pchild->left = heap->min;
    pchild->right = heap->min->right;
    heap->min->right->left = pchild;
    heap->min->right = pchild;
    pchild->mark = false;
}
//continue to cut until we get to false or root
void Cascading_Cut(pFibHeap heap, pFibNode node)
{
    pFibNode ptr = node->p;
    if(ptr) //not root, so we continue
    {
        if(node->mark)   //true, continue cutting
        {
            Cut(heap, node, ptr);
            Cascading_Cut(heap, ptr);
        }
        else node->mark = true; //node has lost child just now
    }
}
