#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define mod 1000000007
long long int resultB[501][501];
long long int resultR[501][501];

int main()
{
    int n;
    scanf("%d", &n);
    resultB[1][1] = resultR[1][1] = 1;
    resultB[2][1] = 2;
    int i;
    int bh, total, minBh, maxBh;
    //when we start from 3, there's no need to deal with cases when a subtree has 0 node
    for(total = 3; total <= n; total++) //total internal nodes
    {
        //we know total >= 2^bh - 1, bh >= h/2, (total <= 2^(h+1)-1), log2(total+1)/2 - 1 <= bh <= 2*log2(total+1) 
        minBh = log2(total+1)/2 - 1;
        maxBh = log2(total+1);
        for(bh = minBh; bh <= maxBh; bh++)
        {
            if(!bh) continue;
            for(i = 1; i < total-1; i++)    //i is the number of the nodes in the left subtree
            {
                resultB[total][bh] += ((resultB[i][bh-1] + resultR[i][bh]) % mod) * ((resultB[total-i-1][bh-1] + resultR[total-i-1][bh]) % mod);
                resultR[total][bh] += (resultB[i][bh-1] * resultB[total-i-1][bh-1]) % mod;
                resultB[total][bh] %= mod;
                resultR[total][bh] %= mod;
            }
        }
    }
    long long int result = 0;
    for(i = 0; i <= n; i++)
        result = (result + resultB[n][i]) % mod;
    printf("%lld\n", result);
    
    system("pause");
    return 0;
}
