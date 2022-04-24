#include <stdio.h>
#include <stdlib.h>
#define INF 10000000

typedef enum{false, true} bool;
int ref[1001];
int fruitCount;
int mat[1001][1001];
int price[1001];
bool final[1001];
int minPrice, maxFruitCount;
int countR, countP, countX;
bool BK(bool R[], bool P[], bool X[]);
bool check(bool R[]);
int main()
{
    minPrice = INF;
    int relationCount;
    scanf("%d%d", &relationCount, &fruitCount);
    countP = fruitCount;
    int i, f1, f2;
    int count = 0;
    for(i = 0; i < relationCount; i++)
    {
        scanf("%d%d", &f1, &f2);
        if(ref[f1] == 0)    ref[f1] = ++count;
        if(ref[f2] == 0)    ref[f2] = ++count;
        if(!mat[ref[f1]][ref[f2]])  mat[ref[f1]][ref[f2]] = mat[ref[f2]][ref[f1]] = 1;
    }
    bool R[101], P[101], X[101];
    for(i = 1; i <= fruitCount; i++)
    {
        scanf("%d%d", &f1, &f2);
        if(ref[f1] == 0)    ref[f1] = ++count;
        price[ref[f1]] = f2;
        R[i] = X[i] = false;
        P[i] = true;
    }
    BK(R,P,X);
    printf("%d\n", maxFruitCount);
    bool hasPrint = false;
    for(i = 0; i < 1000; i++)
    {
        if(!ref[i]) continue;
        if(!final[ref[i]])    continue;
        if(hasPrint)    printf(" ");
        else hasPrint = true;
        if(final[ref[i]])   printf("%03d", i);
    }
    if(maxFruitCount != 0) printf("\n");
    printf("%d", minPrice);
    return 0;

}
bool BK(bool R[], bool P[], bool X[])
{
    bool find, judge;
    if(!countP && !countX)
    {
        find = check(R);
        return find;
    }
    int saveR, saveP, saveX;
    saveR = countR;
    saveP = countP;
    saveX = countX;
    bool nR[101], nP[101], nX[101];
    int i, j;
    for(i = 1; i <= fruitCount; i++)    //for each fruit in P
    {
        if(countR + countP < maxFruitCount)  return find;    //impossible to get more fruit//剪枝
        if(!P[i])   continue;
        for(j = 1; j <= fruitCount; j++)    //update sets
        {
            nR[j] = R[j];
            nP[j] = P[j];
            nX[j] = X[j];
            if(j == i)
            {
                if(nP[j])
                    nP[j] = false, countP--;
                if(nX[j])
                    nX[j] = false, countX--;
                if(!nR[j])  //add i into R to test whether the max solution should have i
                    nR[j] = true, countR++;
            }
            //nP = P ∩ N{i}，check whether fruit[j] is N{i}
            if(nP[j] && mat[i][j])  //j in P but collision(not neighbor)
            {
                nP[j] = false;
                countP--;
            }
            if(nX[j] && mat[i][j])
            {    
                nX[j] = false;
                countX--;
            }
        }
        judge = BK(nR, nP, nX);
        if(!find)   find = judge;
        //update P and X
        countP = saveP;
        countX = saveX;
        countR = saveR;
        if(P[i])    //P = P - {v}
            P[i] = false, countP--, saveP = countP;
        if(!X[i])   //X = X∪{v}
            X[i] = true, countX++, saveX = countX;
    }
    return find;
}
bool check(bool R[])
{
    int i;
    if(countR > maxFruitCount)  //more fruit
    {
        minPrice = 0;
        for(i = 1; i <= fruitCount; i++)
        {
            if(R[i])    minPrice += price[i];
            final[i] = R[i];
        }
        maxFruitCount = countR;
        return true;
    }
    else if(countR < maxFruitCount)
        return false;
    else if(countR == maxFruitCount)
    {
        int p = 0;
        for(i = 1; i <= fruitCount; i++)
        {
            if(R[i])    p+=price[i];
        }
        if(p >= minPrice)   return false;
        else 
        {
            minPrice = p;
            for(i = 1; i <= fruitCount; i++)
                final[i] = R[i];
            return true;
        }
    }
}