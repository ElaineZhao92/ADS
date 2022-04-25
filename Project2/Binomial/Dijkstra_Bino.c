#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "binomial.h"
#include "binomial.c"
#define INF 0xEFEFEF
#define MAX_NODE 14082000       //5k行测试文件的最大结点编号

struct LinkNode//邻接点记录
{
    int t;    //终点的index
    int dis;  //距离
    struct LinkNode* next;   //指向下一个指针
};
typedef struct LinkNode linknode;

struct Node//邻接表结构
{
    linknode *linklist;  //指向终点组成的链表

    int min_dis;         //从指定点到该点的最短距离
    int visited;         //是否被访问过

    //pFibNode FNode;    //指向斐波那契堆结点
    hNode BNode;
}sourcelist[MAX_NODE];            //下标为起点编号
typedef struct Node node;

//建立邻接表，结果存储在node数组s[]中
void BuildLinkList();
//返回子邻接表最后一个结点的地址
linknode* GetLastLinknode(linknode* root);
//找到编号为source的点到所有点的shortestpath
void Dijkstra(int source, int terminal);
//查找子邻接表中是否存在编号为x的点
linknode *FindXinLink(int x, linknode* linkroot);

binomialHeap BHeap = NULL;

int main()
{
    BHeap = createBH();
    //建立邻接表
    BuildLinkList();
    printf("Build linklist successfull!\n");
    int source, terminal;
    printf("input '-1' to quit\n");
    printf("Please input the query:");
    scanf("%d", &source);
    while(source != -1)
    {
        scanf("%d", &terminal);
        if(source < MAX_NODE && terminal < MAX_NODE)
        {
            Dijkstra(source, terminal);
        }
        else
        {
            printf("#ERROR# input is too big\n");
        }
        printf("Please input the query:");
        scanf("%d", &source);
    }

    printf("Bye!\n");
    system("pause");
    return 0;
}

//建立邻接表，结果存储在node数组s[]中
void BuildLinkList()
{
    //初始化node数组
    for(int i = 0; i < MAX_NODE; i++) sourcelist[i].visited = -1;
    int maxx = -1;

    printf("Building linklist...\n");
    FILE *fp = fopen("..\\documents//B_connect.txt", "r");
    if(!fp)
    {
        printf("打开B_connect.txt失败!\n");
        return;
    }

    printf("100%%[▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧▧]");
    int s;    //起点
    int t;    //终点
    int dis;  //s-t的距离

    //建立邻接表
    while(!feof(fp))
    {
        fscanf(fp, "%d %d %d\n", &s, &t, &dis);
        if(s > maxx) maxx = s;
        if(t > maxx) maxx = t;
        //printf("handling: %d %d %d\n", s, t, dis);          
        if(sourcelist[s].visited && sourcelist[t].visited)//两点均不存在邻接表中，在结尾插入新的结点
        {
            //printf("00\n");
            //s->t
            sourcelist[s].visited = 0;
            sourcelist[s].min_dis = INF;
            //初始化首条邻边
            linknode *tmp = (linknode*)malloc(sizeof(linknode));
            tmp->t = t;
            tmp->dis = dis;
            tmp->next = NULL;
            sourcelist[s].linklist = tmp;

            //t->s
            sourcelist[t].visited = 0;
            sourcelist[t].min_dis = INF;
            //初始化首条邻边
            tmp = (linknode*)malloc(sizeof(linknode));
            tmp->t = s;
            tmp->dis = dis;
            tmp->next = NULL;
            
            sourcelist[t].linklist = tmp;
        }
        else if(!sourcelist[s].visited && !sourcelist[t].visited)//s,t均在邻接表中
        {
            //printf("11\n");
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
        else if(sourcelist[s].visited && !sourcelist[t].visited)//s不在，t在
        {
            //printf("01\n");
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
            //初始化首条邻边
            subtmp = (linknode*)malloc(sizeof(linknode));
            subtmp->t = t;
            subtmp->dis = dis;
            subtmp->next = NULL;
            sourcelist[s].linklist = subtmp;
        }
        else if(!sourcelist[s].visited&& sourcelist[t].visited)//s在，t不在
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
            //初始化首条邻边
            subtmp = (linknode*)malloc(sizeof(linknode));
            subtmp->t = s;
            subtmp->dis = dis;
            subtmp->next = NULL;
            sourcelist[t].linklist = subtmp;
        }        
    }
    //printf("max node index is %d\n", maxx);
    return ;
}

//返回子邻接表最后一个结点的地址
linknode* GetLastLinknode(linknode* root)
{
    if(root->next == NULL) return root;
    else                   return GetLastLinknode(root->next);
}

//找到编号为source的点到所有点的shortestpath
void Dijkstra(int source, int terminal)
{
    time_t start = clock();
    hNode curnode;
    BHeap = createBH();
    
    //初始化 - 将所有点标为unvisited
    //若邻接，min_dis标为边长；否则表为INF
    int i = 1;
    while(i < MAX_NODE)
    {
        sourcelist[i].visited = 0;
        sourcelist[i].BNode = NULL;
        //查找curnode是否在sourcenode的子邻接表中
        linknode *pos = FindXinLink(i, sourcelist[source].linklist);

        if(i == source)
        {
            sourcelist[i].min_dis = 0;
            sourcelist[i].BNode = create_hNode(0, source);
        }
        else 
        {
            sourcelist[i].min_dis = INF;
        }
        i++;
    }
    //起点入堆, value - min_dis, position - 点编号
    BHeap = bhInsert(BHeap,sourcelist[source].BNode);

    //for(i = 1; i < MAX_NODE; i++)
    while(BHeap->root)//堆非空时
    {
        curnode = bhMinimum(BHeap);                  //curnode->position为该点编号  
        int index = curnode->position;
        if(sourcelist[index].visited)
        {
            bhExtractMin(&BHeap);
            continue; //已访问则跳过
        } 
        sourcelist[index].visited = 1;          //指定点标为已访问
        bhExtractMin(&BHeap);              //释放min结点空间
        
        //更新所有和min_node相连且unvisited的点
        linknode* cur_update = sourcelist[index].linklist;
        //if(!cur_update) printf("nothing...\n");
        while(cur_update)
        {
            int t = cur_update->t;
            if(!sourcelist[t].visited && cur_update->dis+sourcelist[index].min_dis < sourcelist[t].min_dis)
            {
                if(sourcelist[t].min_dis == INF)
                {
                    sourcelist[t].BNode = create_hNode(cur_update->dis+sourcelist[index].min_dis, t);
                    BHeap = bhInsert(BHeap, sourcelist[t].BNode);
                }  
                else
                {
                    bhDecreaseKey(BHeap, sourcelist[t].BNode, cur_update->dis+sourcelist[index].min_dis);
                }
                sourcelist[t].min_dis = cur_update->dis+sourcelist[index].min_dis;
            }
            cur_update = cur_update->next;
        }
        //if(sourcelist[terminal].visited)
         //   break;
    }
    time_t stop = clock();
    if(sourcelist[terminal].min_dis == INF) printf("%d and %d are unconnected!\n", source, terminal);
    else printf("The shortest path between %d and %d is %d.\n", source, terminal, sourcelist[terminal].min_dis);
    free(BHeap);
    printf("cost time %.0fms\n", difftime(stop, start));
    return;
}

//查找子邻接表中是否存在编号为x的点
linknode *FindXinLink(int x, linknode* linkroot)
{
    if(!linkroot) return NULL;

    if(linkroot->t == x) return linkroot;
    else                 return FindXinLink(x, linkroot->next);
}