//1~100求和
#include <stdio.h>
int main()
{
    int i , sum = 0;
    for(i = 1;i <= 100 ;i++)
    {
        sum += i;
    }

    printf("1到100的和是：%d",sum);

    return 0;
}