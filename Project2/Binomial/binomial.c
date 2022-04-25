#include "binomial.h"

//hNode create_hNode(int key)
hNode create_hNode(int key, int position)
{
    hNode x = (hNode) malloc(sizeof(struct BinoNode));
    if (x == NULL) {
        printf("ERROR! Memory allocation of node failed\n");
        exit(-1);
    }
    x->key = key;
    x->position = position;
    x->p = NULL;
    x->child = NULL;
    x->sibling = NULL;
    x->degree = 0;
    return x;
}

binomialHeap createBH()
{
    binomialHeap h = (binomialHeap)malloc(sizeof (struct heap));
    if(!h)   exit(-1);
    h->root = NULL;
    return h;
}
/*insert node X into bh.
X will be inserted in the head of the heap.*/
binomialHeap bhInsert(binomialHeap h1, hNode node)
{
    binomialHeap h = createBH();
    h->root = node;
    h1 = bhUnion(&h1,&h);
    return h1;
}


/*find the minimum key from the binomial heap.
Search them in the sibling lists.*/
hNode bhMinimum(binomialHeap h)
{
    int min;
    struct BinoNode * p1 = h->root;
    struct BinoNode * p2 = NULL;
    if(p1)
    {
        min = p1->key;
        p2 = p1;
        p1 = p1->sibling;
        while(p1 != NULL)
        {
            if(p1->key<min)
            {
                min = p1->key;
                p2 = p1;
            }
            p1 = p1->sibling;
        }
    }
    return p2;
}

/*link node1 and node2 together, node2 becomes a root.*/
void bhLink(hNode node1, hNode node2)
{
    node1->p = node2;
    node1->sibling = node2->child;
    node2->child = node1;
    node2->degree++;
}

/*merge h1,h2*/
hNode bhMerge(binomialHeap h1, binomialHeap h2)
{
    hNode p1 = h1->root;
    hNode p2 = h2->root;
    hNode p = NULL;//return 
    hNode RNode = NULL;//return
    if(p1 == NULL || p2 == NULL)
    {
        if(p1 == NULL)  RNode = p2;
        if(p2 == NULL)  RNode = p1;
        return RNode;
    }
    if(p1->degree < p2->degree)//p1 is smaller
    {
        RNode = p1;
        p = RNode;
        p1 = p1->sibling;
    }
    else//p2 be the root
    {
        RNode = p2;
        p = RNode;
        p2 = p2->sibling;
    }
    while(p1 && p2)
    {
        if(p1->degree < p2->degree)
        {
            p->sibling = p1;
            p = p1;
            p1 = p1->sibling;
        }
        else
        {
            p->sibling = p2;
            p = p2;
            p2 = p2->sibling;
        }
    }
    if(p1)
        p->sibling = p1;
    if(p2)
        p->sibling = p2;
    return RNode;
}

/*Union the subtrees in the same degree.*/
binomialHeap bhUnion(binomialHeap * h1, binomialHeap * h2)
{
    binomialHeap h = createBH();
    h->root =  bhMerge(* h1, * h2);
    free(* h1);
    free(* h2);
    * h1 = NULL;
    * h2 = NULL;
    if(!h->root)    return h;
    hNode prevNode = NULL;
    hNode x = h->root;
    hNode nextNode = x->sibling;
    while(nextNode)
    {
        if(x->degree != nextNode->degree || (nextNode->sibling != NULL && x->degree == nextNode->sibling->degree))
        {
            prevNode = x;
            x = nextNode;
        }
        else if(x->key <= nextNode->key)//Two trees' degree equal and x is smaller
        {
            x->sibling = nextNode->sibling;//union into one tree
            bhLink(nextNode, x);//x is root
        }
        else//equal and nextNode is smaller
        {
            if(!prevNode)//x is a root node.
                h->root = nextNode;
            else prevNode->sibling = nextNode;
            bhLink(x,nextNode); // nextNode is root
            x = nextNode;
        }
        nextNode = x->sibling;
    }
    return h;
}


/*find and delete the min key*/
binomialHeap bhExtractMin(binomialHeap * h)
{
    hNode p = (*h)->root;
    hNode x = NULL;
    hNode x_prev = NULL;
    hNode p_prev = NULL;
    int min;

    if (p == NULL) {
        return NULL;
    }

    // find the root x with minimum key in the root list of h
    x = p;
    min = p->key;
    p_prev = p;
    p = p->sibling;
    while (p != NULL) {
        if(p->key < min) {
            x_prev = p_prev;
            x = p;
            min = p->key;
        }
        p_prev = p;
        p = p->sibling;
    }
    if (x == (*h)->root) {
        (*h)->root = x->sibling;
    } else if (x->sibling == NULL) {
        x_prev->sibling = NULL;
    } else {
        x_prev->sibling = x->sibling;
    }

    hNode child_x = x->child;
    if (child_x != NULL) {

        // generate a new Binomial Heap
        binomialHeap h1 = createBH();

        // reverse the order of linked list of x's children
        // and set the children's p to NULL
        // link h1->head to the head of resulting list
        child_x->p = NULL;
        h1->root = child_x;
        p = child_x->sibling;
        child_x->sibling = NULL;
        while (p != NULL) {
            p_prev = p;
            p = p->sibling;
            p_prev->sibling = h1->root;
            h1->root = p_prev;
            p_prev->p = NULL;
        }
        *h = bhUnion(h, &h1);
    }
    return *h;
}
/*
binomialHeap bhDecreaseKey(binomialHeap  h, hNode x, int key)
{
    if(x->key < key)
        return NULL;
    x->key -= key;
    hNode p = x;
    hNode p1 = x->p;
    while(p != NULL && p->key < p1->key)
    {
        p->key = p1->key;
        p1->key = key;
        p = p1;
        p1 = p->p;
    }

    return h;
}*/

void bhDecreaseKey(binomialHeap h, hNode x, int key) {
    if (x->key < key) {
        return;
    }
    x->key -= key;

    hNode y = x;
    hNode p = x->p;
    while (p != NULL && y->key < p->key) {
        y->key = p->key;
        p->key = key;
        y = p;
        p = y->p;
    }
}


/*delete a node x's key*/
void bhDelete(binomialHeap h, hNode x)
{
    printf("in delete\n");
    bhDecreaseKey(h,x,-1000000);//decrease the x to the minimum
    bhExtractMin(&h);
    printf("before return delete\n");
    return;
}

void traverseBH(hNode t)
{
    while(t)
    {
        printf("%d#",t->key);
        if(t->child)
            traverseBH(t->child);
        t = t->sibling;
    }
    return ;
}
/*
int main()
{
    int n,i,x,a,b;
    int num[1000];
    printf("Enter the number of nodes you want to insert:");
    scanf("%d",&n);
    printf("Enter your n numbers:");
    binomialHeap h = createBH();
    for(i = 0; i < n; i++)
    {
        scanf("%d",&num[i]);
    }
    for(i = 0; i < n; i++)
    {
        h = bhInsert(h,num[i], 0);
    }
    traverseBH(h->root);
    printf("\n");
    printf("minimum = %d\n",h->root->key);
    bhExtractMin(&h);
    traverseBH(h->root);

}*/