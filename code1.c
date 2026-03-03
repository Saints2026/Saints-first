//生成100~999之间的水仙花数（一个三位整数，其每个位上的立方和等于它本身）
#include <stdio.h>
int main()
{
    int temp,i;
    printf("100~999之间的水仙花数是：");
    for(i = 100; i <= 999; i++)
    {
        temp = i;
        int units = temp % 10;
        int tens = ( temp % 100 ) / 10;
        int hundreds = temp / 100;

        int sum = units * units * units + tens * tens * tens + hundreds * hundreds * hundreds;
        
        if(sum == i)
            printf("%d ",i);
      
    }
    return 0;
}