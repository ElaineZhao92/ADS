#include <iostream>
#include <algorithm>
#include <queue>
#include <string>
using namespace std;

//用于建立标准huffmantree
typedef struct TreeNode
{
    int level;
    char key;
    int freq;
    TreeNode *lchild;
    TreeNode *rchild;
}tnode;
tnode* t[500];

//用于建立二叉树检测是否满足前缀
typedef struct CheckNode
{
    CheckNode* lchild;
    CheckNode* rchild;
}cnode;
cnode* cT = NULL;

tnode * buildtnode(int level,char key, int f,TreeNode *lchild,TreeNode *rchild);//新建tnode节点
void insert(tnode* address);//向堆中插入合并后结点
void PercolateUp(int child);//插入新结点后上滤
void PercolateDown(int parent);//将n个结点完全插入堆后下滤
void swap(int a, int b);//用于交换两个节点
tnode* DeleteMin();
int LevelOrder(tnode* root);//计算标准WPL
cnode* buildcnode();
int buildtree(string code);//将编码为code的结点插入二叉树
void FreeTree(cnode* t);//freetree供下次使用
bool cmp(string c1, string c2);
int t_size = 0;


int main()
{
    int freqlist[160] = {0};//对每个字符的频率进行留档
    int n,f,k,j; //字符个数,频率
    char c;
    cin >> n;
    for(int i = 0; i < n; i++)
    {
        cin >> c >> f;
        //在freqlist中留档
        freqlist[c] = f;
        tnode* cur_tnode =buildtnode(-1,c,f,NULL,NULL);
        t[t_size++] = cur_tnode;
    }

    for(int i = (n-1)/2 ; i >= 0 ; i--) PercolateDown(i); //adjust min heap

    //建标准Huffman树，计算标准WPL
    for(int i = 1; i < n; i++)//n-1次merge
    {
        tnode* n1 = DeleteMin();
        tnode* n2 = DeleteMin();
        tnode* cur_tnode = buildtnode(-1,'$',n1->freq + n2->freq,n1,n2);
        insert(cur_tnode);       
    }
    tnode* fn = DeleteMin();    
    //level-order
    int WPL = LevelOrder(fn);
    //检验case
    int n_case;
    cin >> n_case;
    for(int i = 0; i < n_case; i++)
    {
        int curWPL = 0;
        string code[100];
        for(int j = 0 ; j < n; j++)
        {
            cin >> c >> code[j]; getchar();
            curWPL += code[j].length()*freqlist[c];
        }
        if(curWPL != WPL)
        {
            cout << "No" << endl;
            continue;
        }
        //根据code降序排序
        sort(code, code+n, cmp);
        /* O(N^2)复杂度比较，简单但是慢 */
        /*for(k=0;k<n-1;k++)
        {
            for(j=k+1; j<n; j++)
            {
                if(code[j] == code[k].substr(0, code[j].size()))
                {
                    printf("No\n");
                    break;
                }     
            }
            if(j!=n) break;
        }
        if(j==n && k==n-1)  printf("Yes\n");*/
        /*建树判断*/
        cT = buildcnode();
        for(int i = 0; i < n ; i++)//从长的开始build
        {
            if(buildtree(code[i]) == 0)
            {
                cout << "No" << endl;
                break;
            }
            if(i==n-1) cout << "Yes" << endl;
        }
        FreeTree(cT);
    }
    return 0;
}


tnode * buildtnode(int level,char key, int f,TreeNode *lchild,TreeNode *rchild)//新建
{
    tnode* curnode = (tnode*)malloc(sizeof(tnode));
    curnode->level = level;
    curnode->key = key;
    curnode->freq = f;
    curnode->lchild = lchild;
    curnode->rchild = rchild;
    return curnode;
}

cnode* buildcnode()
{
    cnode* cur = (cnode*)malloc(sizeof(cnode));
    cur->lchild = cur->rchild = NULL;
    return cur;
}


void insert(tnode* address)
{
    //插入堆尾
    t[t_size] = address;
    PercolateUp(t_size);

    t_size++;
}

void PercolateUp(int child)
{
    int c = child;
    int p = (c-1)/2;
    while(p>=0)
    {
        if(t[c]->freq < t[p]->freq)
        {
            //swap
            swap(c,p);
            //go up
            c = p;
            p = (c-1)/2;
        }
        else break;
    }
}

void PercolateDown(int parent)
{
    int p = parent;
    int c = 2*p + 1;
    while(c < t_size)
    {
        if(c+1 < t_size && t[c+1]->freq < t[c]->freq) ++c;
        if(t[c]->freq < t[p]->freq)
        {
            swap(c,p);
            //go up
            p = c;
            c = 2*p + 1;
        }
        else break;
    }
}

tnode* DeleteMin()
{
    //首尾交换
    swap(0,t_size-1);
    t_size--;
    PercolateDown(0);

    return t[t_size];
}

int LevelOrder(tnode* root)//返回标准WPL
{
    int WPL = 0;
    queue <tnode*> q;
    //root enqueue
    root->level = 0;
    q.push(root);

    while(!q.empty())
    {
        //dequeue
        tnode* pop = q.front();
        q.pop();

        if(pop->lchild && pop->rchild)//非leaf，左右孩子入队
        {
            pop->lchild->level = pop->rchild->level = pop->level + 1;
            q.push(pop->lchild);
            q.push(pop->rchild);
        }
        else
            WPL += pop->level*pop->freq;
    }
    return WPL;
}

void FreeTree(cnode* t)
{
    if(t)
    {
        if(t->lchild)
        {
            FreeTree(t->lchild);
            t->lchild = NULL;
        }
        if(t->rchild)
        {
            FreeTree(t->rchild);
            t->rchild = NULL;
        }
    }
    t = NULL;
    return;
}

int buildtree(string code)
{
    cnode* cur = cT;
    int len = code.size();
    for(int i = 0; i < len-1; i++)
    {
        if(code[i] == '0')
        {
            if(cur->lchild == NULL)
                cur->lchild = buildcnode();
            cur = cur->lchild;
        }
        else
        {
            if(cur->rchild == NULL)
                cur->rchild = buildcnode();
            cur = cur->rchild;
        }
    }
    if(code[len-1] == '0')
    {
        if(cur->lchild) return 0;
        else
        {
            cur->lchild = buildcnode();
            cur = cur->lchild;
        }
    }
    else
    {   
        if(cur->rchild) return 0;
        else
        {
            cur->rchild = buildcnode();
            cur = cur->rchild;
        }
    }
    if(cur->lchild || cur->rchild)
        return 0;
    return 1;
}

bool cmp(string c1, string c2)
{
    return c1.length() > c2.length();
}//sort code by length

void swap(int a, int b)
{
    tnode* tmp = t[a];
    t[a] = t[b];
    t[b] = tmp;
}