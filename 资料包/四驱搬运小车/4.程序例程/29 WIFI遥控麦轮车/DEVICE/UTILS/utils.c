#include "utils.h"
#include "string.h"


char tempBuf[10];
char *itoa(int num)
{	
	char *str = tempBuf;
	int radix = 10;
	/*索引表*/
	char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned int unum;/*中间变量*/
	char temp;
	int i = 0, j, k;
	/*确定unum的值*/
	if(radix == 10 && num < 0)/*十进制负数*/
	{
		unum = (unsigned int)-num;
		str[i++] = '-';
	} else
		unum = (unsigned int)num;/*其他情况*/
	/*转换*/
	do {
		str[i++] = index[unum % (unsigned)radix];
		unum /= radix;
	} while(unum);
	str[i] = '\0';
	/*逆序*/
	if(str[0] == '-')
		k = 1;/*十进制负数*/
	else k = 0;
	
	for(j = k; j <= (i - 1) / 2; j++) {
		temp = str[j];
		str[j] = str[i - 1 + k - j];
		str[i - 1 + k - j] = temp;
	}
	return str;
}


int atoi(char *str)  
{  
    int flag = 1;                                 //用来记录是正数还是负数  
    uint16_t ret = 0;  
                                      
    if (str == NULL)  
    {  
        return 0;  
    }  
    if (*str == '\0')  
    {  
        return (uint16_t)ret;  
    }  
  
    while (*str)  
    {  
        if (*str >= '0' && *str <= '9')  
        {  
            ret = ret * 10 + flag * (*str - '0');                  
            if (ret >= 0xFFFF)                 //判定是否溢出了  
            {  
                ret = 0;  
                break;  
            }  
        }  
        else  
        {  
            break;  
        }  
        str++;  
    }  

    return ret;  
}  

