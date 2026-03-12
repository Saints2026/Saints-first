//字符串练习1：统计字符串长度（不用strlen）
/*编写一个C程序，输入一个字符串（不超过100个字符），统计并输出该字符串的长度。
要求：不得使用strlen或其它库函数，必须通过循环遍历字符串每个字符实现计数。
输入要求：输入一行字符串，可包含字母、数字、空格及常见符号。
输出要求：输出一个整数，表示该字符串的实际长度（不含末尾结束符\0）*/
// #include <stdio.h>
// #include <string.h>
// #define MAX_LEN 100

// int main()
// {
//     char str[MAX_LEN];
//     int len = 0;
//     // gets(str);
//     fgets(str,MAX_LEN,stdin);
//     //去除换行符
//     str[strcspn(str,"\n")] = '\0';
    
//     for(int i = 0; str[i] != '\0'; i++)
//     {
//         len++;
//     }

//     printf("字符串的实际长度是：%d\n",len);

//     return 0;
// }

//字符串练习2：字符串反转
/*编写一个C程序，输入一个字符串，将其完全反转后输出。
例如输入“abcd”,输出“dcba”。要求使用字符数组实现，不得使用库函数strrev
输入要求：输入一行字符串，长度不超过100.
输出要求：输出反转后的字符串*/
// #include <stdio.h>
// #include <string.h>
// #define MAX_LEN 100

// int main()
// {
//     char str[MAX_LEN];
//     char temp;
//     int len = 0;
//     fgets(str,MAX_LEN,stdin);
//     str[strcspn(str,"\n")] = '\0';

//     for(int i = 0; str[len] != '\0'; len++);

//     //双指针交换实现字符串反转
//     for(int i = 0, j = len - 1; i < j; i++, j--)
//     {
//         temp = str[i];//1；把左指针的字符暂存到temp
//         str[i] = str[j];//2:把右指针的字符覆盖到左指针位置
//         str[j] = temp;//3：把/temp里的字符覆盖到右指针位置
//     }

//     printf("反转后的字符串：%s\n",str);

//     return 0;
// }

//字符串练习3：统计元音字母个数
/*编写一个C程序，输入一个字符串，统计其中元音字母（A、E、I、O、U）
及小写（a、e、i、o、u）的个数并输出。不区分大小写，需同时统计大小写。
输入要求：输入一行字符串，可包含字母、数字、空格等。
输出要求：输出一个整数，表示元音字母的总个数。*/
#include <stdio.h>
#include <string.h>
#define MAX_LEN 100

int main()
{
    char str[MAX_LEN];
    int count = 0;

    fgets(str,MAX_LEN,stdin);
    str[strcspn(str,"\n")] = '\0';

    for(int i = 0; str[i] != '\0'; i++)
    {
        if(str[i] == 'a' || str[i] == 'A' ||
           str[i] == 'e' || str[i] == 'E' ||
           str[i] == 'i' || str[i] == 'I' ||
           str[i] == 'o' || str[i] == 'O' ||
           str[i] == 'u' || str[i] == 'U')
           {
            count++;
           }
    }

    printf("元音字母个数为：%d\n",count);
    
    return 0;
}