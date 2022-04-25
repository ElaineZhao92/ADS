#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "Fibonacci.h"

#define INF 0xEFEFEF
#define MAX_NODE 14081900        //max No. of vertice

struct LinkNode                 //linklist of terminal points
{
    int t;                      //No. of terminal point
    int dis;                    //distance between source and terminal points
    struct LinkNode* next;      //point to next adjacent point of source point
};
typedef struct LinkNode linknode;

struct Node                     //array of source points
{
    linknode *linklist;         //point to adjacent points of current node

    int min_dis;                //shortest path length between given point and current point
    int visited;                //has current point been visited

    pFibNode FNode;             //point to according node in Fibonacci Heap
}sourcelist[MAX_NODE];          //index is the no. of source point
typedef struct Node node;

//build adjacency list
void BuildLinkList();
//return the address of the last node in linklist
linknode* GetLastLinknode(linknode* root);
//using Dijkstra Algorithm to find the shortest path between source and terminal
void Dijkstra(int source, int terminal);
//find whether node X is adjacent with given point
linknode *FindXinLink(int x, linknode* linkroot);

pFibHeap FHeap = NULL;


int main()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_GREEN); //output software reminder in blue
    FHeap = makeFibHeap();
    //Build Adjacency List
    BuildLinkList();

    printf("build linklist fin\n");
    int source, terminal;
    printf("input '-1' to quit\n");
    printf("Please input the query: ");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);//output user's input in white
    scanf("%d", &source);
    //query
    while(source != -1)
    {
        scanf("%d", &terminal);
        if(source < MAX_NODE && terminal < MAX_NODE)
        {
            Dijkstra(source, terminal);
        }
        else
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED);//output error in red
            printf("#ERROR# input is too big\n");
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_GREEN); //output software reminder in blue
        printf("Please input the query: ");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);//output user's input in white
        scanf("%d", &source);
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_GREEN); //output software reminder in blue
    printf("Bye!\n");
    system("pause");
    return 0;
}

//build adjacency list
void BuildLinkList()
{
    //initialization
    for(int i = 0; i < MAX_NODE; i++) sourcelist[i].visited = -1;
    int maxx = -1;

    printf("Building linklist...\n");
    FILE *fp = fopen("..\\documents//B_connect.txt", "r");
    if(!fp)
    {
        printf("#ERROR# open B_connect.txt failed!\n");
        return;
    }
    
    int s;    //source
    int t;    //terminal
    int dis;  //distance between source & terminal

    //scan the Map File
    while(!feof(fp))
    {
        fscanf(fp, "%d %d %d\n", &s, &t, &dis);
        if(s > maxx) maxx = s;
        if(t > maxx) maxx = t;      
        if(sourcelist[s].visited && sourcelist[t].visited)
        //s & t are not in adjacency list
        {
            //s->t
            sourcelist[s].visited = 0;
            sourcelist[s].min_dis = INF;
            //first point in linklist
            linknode *tmp = (linknode*)malloc(sizeof(linknode));
            tmp->t = t;
            tmp->dis = dis;
            tmp->next = NULL;
            sourcelist[s].linklist = tmp;

            //t->s
            sourcelist[t].visited = 0;
            sourcelist[t].min_dis = INF;
            //first point in linklist
            tmp = (linknode*)malloc(sizeof(linknode));
            tmp->t = s;
            tmp->dis = dis;
            tmp->next = NULL;
            
            sourcelist[t].linklist = tmp;
        }
        else if(!sourcelist[s].visited && !sourcelist[t].visited)
        //s & t are both in the adjacency list
        {
            //s->t
            linknode *linklast = GetLastLinknode(sourcelist[s].linklist);
            linknode *subtmp = (linknode*)malloc(sizeof(linknode));
            subtmp->t = t;
            subtmp->dis = dis;
            subtmp->next = NULL;
            linklast->next = subtmp;

            //t->s
            linklast = GetLastLinknode(sourcelist[t].linklist);
            subtmp = (linknode*)malloc(sizeof(linknode));
            subtmp->t = s;
            subtmp->dis = dis;
            subtmp->next = NULL;
            linklast->next = subtmp;
        }
        else if(sourcelist[s].visited && !sourcelist[t].visited)
        //s not in, t in
        {
            //t->s
            linknode *linklast = GetLastLinknode(sourcelist[t].linklist);
            linknode *subtmp = (linknode*)malloc(sizeof(linknode));
            subtmp->t = s;
            subtmp->dis = dis;
            subtmp->next = NULL;
            linklast->next = subtmp;

            //s->t
            sourcelist[s].visited = 0;
            sourcelist[s].min_dis = INF;
            //first point in linklist
            subtmp = (linknode*)malloc(sizeof(linknode));
            subtmp->t = t;
            subtmp->dis = dis;
            subtmp->next = NULL;
            sourcelist[s].linklist = subtmp;
        }
        else if(!sourcelist[s].visited&& sourcelist[t].visited)
        //s in, t not in
        {
            //s->t
            linknode *linklast = GetLastLinknode(sourcelist[s].linklist);
            linknode *subtmp = (linknode*)malloc(sizeof(linknode));
            subtmp->t = t;
            subtmp->dis = dis;
            subtmp->next = NULL;
            linklast->next = subtmp;

            //t->s
            sourcelist[t].visited = 0;
            sourcelist[t].min_dis = INF;
            //first point in linklist
            subtmp = (linknode*)malloc(sizeof(linknode));
            subtmp->t = s;
            subtmp->dis = dis;
            subtmp->next = NULL;
            sourcelist[t].linklist = subtmp;
        }        
    }
    return ;
}

//return the address of the last node in linklist
linknode* GetLastLinknode(linknode* root)
{
    if(root->next == NULL) return root;
    else                   return GetLastLinknode(root->next);
}

//using Dijkstra Algorithm to find the shortest path between source and terminal
void Dijkstra(int source, int terminal)
{
    time_t start = clock();//timer start

    pFibNode curnode;
    
    //initialization
    int i = 1;
    while(i < MAX_NODE)
    {
        sourcelist[i].visited = 0;
        
        if(i == source)//the source point itself
        {
            sourcelist[i].min_dis = 0;
            sourcelist[i].FNode = getFibNode(0, source);
        }
        else
        {
            sourcelist[i].min_dis = INF;
        }
        i++;
    }

    //insert source into the heap
    FHeap = insertToFibHeap(FHeap, sourcelist[source].FNode);

    while(FHeap->totalNode)
    //when the heap is not empty
    {
        curnode = deleteMinFibHeap(FHeap);                                            
        int index = curnode->position;         //curnode->position - index of xurnode
        
        if(sourcelist[index].visited)
        //curnode has been visited
        {
            free(curnode);
            continue; 
        }

        sourcelist[index].visited = 1;          //mark curnode as visited
        free(curnode);                          //delete curnode from heap
        
        //relax
        linknode* cur_update = sourcelist[index].linklist;
        while(cur_update!=0)
        //there are still point adjacent to curnode
        {
            int t = cur_update->t;     //index of adjacent node
            if(!sourcelist[t].visited && cur_update->dis+sourcelist[index].min_dis < sourcelist[t].min_dis)
            {
                if(sourcelist[t].min_dis == INF)
                //new unvisited point - insert into heap
                {
                    sourcelist[t].FNode = getFibNode(cur_update->dis+sourcelist[index].min_dis, t);
                    FHeap = insertToFibHeap(FHeap, sourcelist[t].FNode);
                }  
                else
                //old unvisited point - update its key value in heap
                {
                    FHeap = decreaseFibHeap(FHeap, sourcelist[t].FNode, cur_update->dis+sourcelist[index].min_dis);
                }
                //update the shortest path length
                sourcelist[t].min_dis = cur_update->dis+sourcelist[index].min_dis;
            }
            //visit next adjacent node
            cur_update = cur_update->next;
        }
    }

    time_t stop = clock();//timer stop
    //output query result
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);//output in green color
    if(sourcelist[terminal].min_dis == INF) printf("%d and %d are unconnected!\n", source, terminal);
    else printf("The shortest path between %d and %d is %d.\n", source, terminal, sourcelist[terminal].min_dis);
    printf("cost time %.0fms\n", difftime(stop, start));
    return;
}

//find whether node X is adjacent with given point
linknode *FindXinLink(int x, linknode* linkroot)
{
    if(!linkroot) return NULL;                                   //no point in the linklist

    if(linkroot->t == x) return linkroot;                        //find the given point
    else                 return FindXinLink(x, linkroot->next);  //visit next point in linklist
}