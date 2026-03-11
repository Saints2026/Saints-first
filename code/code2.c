//复习code1:找出100~999之前的水仙花数
// #include <stdio.h>
#include <windows.h>  // 必须包含这个头文件
// int main()
// {
//     SetConsoleOutputCP(65001); // 65001 是 UTF-8 的代码页编号
//     SetConsoleCP(65001);

//     int num;
//     for(num = 100; num <= 999; num++)
//     {
//         int units = num % 10;
//         int tens = (num % 100) / 10;
//         int hundreds = num / 100;

//         int sum = units * units * units + tens * tens * tens + hundreds * hundreds * hundreds;

//         if(sum == num)
//         printf("%d是水仙花数\n",num);
//     }

//     return 0;
// }

//打印五行等腰三角形
#include <stdio.h>
int main()
{
    int i,j,k;
    for (i = 0; i < 5; i++)
    {
        for(j = 0; j < 4 - i; j++)
        printf(" ");

        for(k = 0; k < 2 * i + 1; k++)
        printf("*");

        printf("\n");
    }
    return 0;
}