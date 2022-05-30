#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <vector>
#include <time.h>

#define MAXLEVEL 5 //Skip_List的最高层数，可以更大
#define MIN(a, b) (a>b)?b:a
#define MAX(a, b) (a>b)?a:b

using namespace std;

int genlevel();//随机生成层数，向上拓展概率p = 0.25

class node
{
    friend class skiplist;
    protected:
        int key;//插入序号，没啥用
        int val;//数据
        node* next[MAXLEVEL+1];
    public:
        node(int k, int v) : key(k), val(v) {
            for(int i = 0; i <= MAXLEVEL; i++) next[i] = NULL;
        }
};

class skiplist
{
    protected:
        int level;//当前最高层数
        node* head;
    public:
        skiplist() {};
        skiplist(int l) : level(l), head(NULL) {};
        void inihead(int k, int v)  {
            this->head = new node(k, v);
        }
        void insert(int k, int v);
        void dele(int v);
        void search(int v);
        void print();
};

void skiplist::insert(int k, int v)//key是插入，v是值
{
    int l = genlevel();//为新结点随机产生level
    printf("now insert %d %d, level = %d\n", k, v, l);
    this->level = MAX(this->level, l);//更新skip_list的最大level
    node *tmp = new node(k, v);
    node *pre[MAXLEVEL+1], *cur = this->head;

    for(int i = MAXLEVEL; i >= 1; i--)//寻找新结点在每一level的前置结点
    {
        while(cur->next[i]!= NULL && cur->next[i]->val < v) cur = cur->next[i];
        pre[i] = cur;
    }

    for(int i = l; i >= 1; i--)//在每一level中插入新结点
    {
        tmp->next[i] = pre[i]->next[i];
        pre[i]->next[i] = tmp;
    }
    return;
}

void skiplist::dele(int v)
{
    node *pre[MAXLEVEL+1];
    node *cur = this->head;
    //在skip_list中查找v
    for(int i = MAXLEVEL; i >= 1; i--)
    {
        while(cur->next[i] && cur->next[i]->val < v) cur = cur ->next[i];
        pre[i] = cur;
    }

    if(cur->next[1]==NULL || cur->next[1]->val != v)
    {   
        cout << "Delete Failed : " << v << " IS Not Found" << endl; 
        return;
    }

    for(int i = MAXLEVEL; i >= 1; i--)
    {
        if(pre[i]->next[i]) pre[i]->next[i] = pre[i]->next[i]->next[i];
    }
    cout << "Delete Success: " << v << " Is Deleted" << endl; 
    return;
}

void skiplist::search(int v)
{
    cout << "search result: ";
    node *p = this->head;
    int i = MAXLEVEL;
    while(i>0)
    {
        if(p->next[i]) 
        {
            while(p->next[i] && p->next[i]->val < v) p = p->next[i] ;
            if(p->next[i]  && v == p->next[i]->val)
            {
                cout << v << " is FOUND" << endl;
                return;
            }
        }
        i--;
    }
    cout << v << " is NOT found" << endl;
    return;
}

void skiplist::print()
{
    cout << "Print Begin ---" << endl;
    node *q;
    int i = this->level;
    while(i>0)
    {
        if(this->head->next[i])
        {
            q = this->head->next[i];
            cout << "<level " << i << ">: " << endl;
            while(q)
            {
                cout << "[" << q->key << ", " << q->val << "]  "; 
                q = q->next[i];  
            }
            cout << endl;
        }
        i--;
    }
}

skiplist* ini()
{
    cout << "Initialize Begin ---" << endl;
    skiplist* s0 = new skiplist(1);//ini_level = 1
    s0->inihead(0, 0);
    srand(time(0));
    return s0;
}

int genlevel()
{
    int level = 1;
    while(rand()%4==0)
    {
        level++;
        if(level == MAXLEVEL) return MAXLEVEL;
    }
    return level;
}


int main()
{
    srand((int)time(0));
    double duration;
	clock_t start, stop; 
	int n, m;
	cout << "Enter the number of nodes you want to insert:" ;
	cin >> n;
	cout << "Enter the number of nodes you want to delete:" ;
	cin >> m;
	
    skiplist* s_list = ini();
    /* test for insertion */
    start = clock();
    for(int i = 1; i < n; i++)
        s_list->insert(i, i+50);
    stop = clock();
    duration  = (double)(stop - start)/CLK_TCK;
    cout << endl;
    cout << "insert cost for "<< n << " nodes is: "<< duration << endl;
    /* test for deletion */
    start = clock();
    for(int i = 60;i < 60+m; i++)    
		s_list->dele(i);
	stop = clock();
    duration  = (double)(stop - start)/CLK_TCK;
    cout << endl;
    cout << "deletion cost for "<< m << " nodes is: "<< duration << endl;
   // s_list->print();
   
   
    s_list->search(80);

    start = clock(); 
    s_list->search(91);
	stop = clock();
    duration  = (double)(stop - start)/CLK_TCK;
    cout << "search cost is: "<< duration << endl;
    return 0;
}
