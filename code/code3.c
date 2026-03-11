//100~999之间的水仙花数
// #include <stdio.h>
// int main()
// {
//     int num;
//     for(num = 100; num <= 999; num++)
//     {
//         int units = num % 10;
//         int tens = (num % 100) / 10;
//         int hundreds = num / 100;
//         int sum = units * units * units + tens * tens * tens + hundreds * hundreds * hundreds;

//         if(sum == num)
//         printf("%d\n",num);
//     }
//       return 0;
// }

//打印五行等腰三角形
#include <stdio.h>
int main()
{
    for(int i = 0; i < 5; i++)
    {
        for(int j = 0; j < 4 - i; j++)
        {
            printf(" ");
        }
        
        for(int k = 0; k < 2 * i - 1; k++)
        {
            printf("*");
        }
        printf("\n");
    }
    return 0;
} 
//（数组）输入五个整数，求和、平均值