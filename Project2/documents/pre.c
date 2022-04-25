#include<stdio.h>
#include<string.h>
int main()
{
    int max_index = -1; //记录最大点数

    FILE *fp1 = fopen("test.txt", "r");
    if(!fp1)
    {
        printf("open test.txt failed!\n");
        return -1;
    }
    FILE *fp3 = fopen("B_connect.txt", "w");
    char tmp; //检查行首是否为‘a’
    char line[60] = "\0"; //丢弃的字符串
    int s1, t1, dis1;
    int s2, t2, dis2;
    printf("handling...\n");
    //处理至'a'开头的行 - 数据行
    fscanf(fp1, "%c", &tmp);
    while(!feof(fp1))
    {
        if(tmp == 'a') break;                 // 找到数据，跳出循环处理
        if(tmp != 'a') fgets(line, 60, fp1);  // 丢弃该行剩余部分
        fscanf(fp1, "%c", &tmp);
    }

    fscanf(fp1, "%d %d %d\n", &s1, &t1, &dis1);
    if(s1 > max_index) max_index = s1;
    if(t1 > max_index) max_index = t1;
    while(!feof(fp1))
    {
        fscanf(fp1, "%c %d %d %d\n", &tmp, &s2, &t2, &dis2);
        if(s2 > max_index) max_index = s2;
        if(t2 > max_index) max_index = t2;
        //双向联通 - 输出 B + 单行数据
        if(s1 == t2 && s2 == t1 && dis1 == dis2)
        {
            fprintf(fp3, "%d %d %d\n", s1, t1, dis1);
            fscanf(fp1, "%c %d %d %d\n", &tmp, &s1, &t1, &dis1);
            if(s1 > max_index) max_index = s1;
        if(t1 > max_index) max_index = t1;
        }
        //单向联通 - 输出 S + 单行数据
        else
        {
            printf("S %d %d %d\n", s1, t1, dis1);
            s1 = s2; t1 = t2; dis1 = dis2;
        }
    }


    printf("max index = %d\n", max_index);
    printf("finished!\n");
    fclose(fp1);
    fclose(fp3);
    return 0;
}