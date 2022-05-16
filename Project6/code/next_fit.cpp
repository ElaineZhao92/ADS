#include <iostream>
#include <vector>
#define MAX(x,y) (x>y)?x:y
#define MIN(x,y) (x<y)?x:y
using namespace std;
void result_next_fit(vector<int> a, vector<int> b);

int w;   //容器宽度
int n;   //待插入长方形个数

class rec//长方形
{
  public:
  int width;
  int height; 

  rec() = delete;
  rec(int w, int h) : width(w), height(h) {};
};


class bin        //基于该层第一个长方形建立的桶
{
  public:
  int ban_x = -1;  //已占用宽度
  int height = -1; //桶高

  bin(){};

  bool check(rec& r)//当前bin是否能装下长方形r，允许旋转
  {
    cout << "@check" << endl;
    if     (this->ban_x+r.width<=w  && this->height>=r.height) return true;
    else if(this->ban_x+r.height<=w && this->height>=r.width)//rotate
    {
      cout << "  $ROTATE" << endl;
      int tmp = r.width;
      r.width = r.height;
      r.height = tmp;
      return true;
    } 
    return false;
  }
  bool insert(rec& r)
  {
    cout << "@insert" << endl;
    if(this->height==-1)//该桶首次插入长方形，初始化桶高=长方形的较短边
    {
      cout << "  #ini" << endl;
      this->height = MIN(r.width, r.height);
      this->ban_x = MAX(r.height, r.width);
      return true;
    }
    else if(this->check(r))//插的下，更新已占用桶宽
    {
      this->ban_x += r.width;
      return true;
    }
    return false;
  }
  
};


int main()
{
  int t;             //待插入长方形个数
  vector <int> a, b;
  cout << "Please input the width of the texture: ";
  cin >> w;
  cout << "Please input the number of rectangle: ";
  cin >> n;
  for(int i=0; i<n; i++)
  {
    cin >> t;
    a.push_back(t);
    cin >> t;
    b.push_back(t);
  }
  result_next_fit(a,b);
  
  return 0;
}
void result_next_fit(vector<int> a, vector<int> b)
{
  vector <bin*> shelf;//所有桶
  shelf.push_back(new bin());//首个桶
  for(int i = 0; i < n; i++)
  {
    rec cur_r(a[i],b[i]);
    if(shelf.back()->insert(cur_r)==false)//当前桶插不下，新建桶并重新插入
    {
      shelf.push_back(new bin());
      shelf.back()->insert(cur_r);
    }
  }
  //输出结果
  int tot = 0;
  int n_bin = shelf.size();
  for(int i = 0; i < n_bin; i++) 
  {
    cout << "height[" << i <<"] = " << shelf[i]->height << endl;
    tot += shelf[i]->height;
  }
  cout << "total height = " << tot << endl;
  }