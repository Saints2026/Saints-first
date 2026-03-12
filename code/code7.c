//函数练习1：素数判断函数
/*编写一个自定义函数isPrime，用于判断一个整数是否为素数。
函数原型：int isPrime(int num);
功能要求：若num是素数，返回1；否则返回0.
素数定义：大于1的自然数，除了1和它本身以外，不能被其他自然数整除。
输入要求：在main函数中读取一个整数n(n >= 2)，调用isPrime函数进行判断。
输出要求：根据函数返回值，输出：
若为素数：Yes;
若不是素数：No。*/
#include <stdio.h>
int isPrime(int num)
{
    if(num < 2)
    {
        return 0;
    }

    for(int i = 2; i * i <= num; i++)
    {
        if(num % 2 == 0)
        {
            return 0;
        }
    }

    return 1;
}

int main()
{
    int n;
    printf("请输入一个整数(大于1):");

    scanf("%d",&n);

    if(isPrime(n) == 1)
    {
        printf("Yes\n");
    }
    else
    {
        printf("No\n");
    }

    return 0;
}

//函数练习2：冒泡排序函数
/*编写一个自定义函数 bubbleSort，实现对整型数组的冒泡排序（升序）。
函数原型：void bubbleSort(int arr[], int n);
功能要求：对长度为 n 的整型数组 arr 进行升序排序，直接修改原数组。
排序规则：重复遍历数组，比较相邻元素，若逆序则交换，直到数组有序。
输入要求在 main 函数中：
先输入数组长度 n（1 ≤ n ≤ 100）；
再输入 n 个整数，存入数组。
输出要求调用 bubbleSort 函数排序后，输出排序后的数组，元素间用空格分隔，末尾无多余空格。*/
#include <stdio.h>
#define N 100
void bubbleSort(int arr[], int n)
{
    for(int i = 0; i < n; i++)
    {
        
    }
}

int main()
{
    int arr[N];
    int n;
    printf("数组的长度为：");
    scanf("%d",&n);

    for(int i = 0; i < n; i++)
    {
        scanf("%d",&arr[i]);
    }

    return 0;
}

//函数练习3：简单成绩统计函数
/*编写一个自定义函数 scoreStats，统计学生成绩的最高分、最低分、平均分。
函数原型：void scoreStats(int scores[], int n, int *max, int *min, double *avg);
功能要求：
输入：长度为 n 的成绩数组 scores；
输出：通过指针参数 max、min、avg 分别返回最高分、最低分、平均分（平均分保留 2 位小数）。
输入要求在 main 函数中：
先输入学生人数 n（1 ≤ n ≤ 50）；
再输入 n 个整数成绩（0 ≤ 成绩 ≤ 100）。*/