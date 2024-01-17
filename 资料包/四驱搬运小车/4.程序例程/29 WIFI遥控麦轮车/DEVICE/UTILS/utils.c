#include "utils.h"
#include "string.h"


char tempBuf[10];
char *itoa(int num)
{	
	char *str = tempBuf;
	int radix = 10;
	/*������*/
	char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned int unum;/*�м����*/
	char temp;
	int i = 0, j, k;
	/*ȷ��unum��ֵ*/
	if(radix == 10 && num < 0)/*ʮ���Ƹ���*/
	{
		unum = (unsigned int)-num;
		str[i++] = '-';
	} else
		unum = (unsigned int)num;/*�������*/
	/*ת��*/
	do {
		str[i++] = index[unum % (unsigned)radix];
		unum /= radix;
	} while(unum);
	str[i] = '\0';
	/*����*/
	if(str[0] == '-')
		k = 1;/*ʮ���Ƹ���*/
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
    int flag = 1;                                 //������¼���������Ǹ���  
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
            if (ret >= 0xFFFF)                 //�ж��Ƿ������  
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

