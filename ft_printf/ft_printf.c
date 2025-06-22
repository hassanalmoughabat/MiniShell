
#include <unistd.h>
#include <stdarg.h>

void printstring(char *str,int *length)
{
    if (!str)
    	str ="(null)";
    while(*str)
    	*length += write(1, str++,1);
}

void printnum(long long int num ,int base ,int *length)
{
   char *hexa = "0123456789abcdef";
   if (num < 0)
   {
        num = num * -1;
        *length += write (1,"-",1);
        
   }
   if (num >= base)
        printnum((num / base),base,length);
    *length += write(1,&hexa[num % base],1);
}

int ft_printf(const char *tp, ... )
{   
    int length = 0;
    int i = 0;
    va_list     args;
    va_start(args,tp);
  
    while (tp[i] != '\0')
    {
        if ((tp[i] == '%') && (tp[i + 1] == 'd' || tp[i + 1] == 's' || tp[i + 1] == 'x'))
        {
           i++;
            if (tp[i] == 's')
                printstring(va_arg(args, char *), &length);
            if (tp[i] == 'd')
               	printnum((long long int)va_arg(args, int),10,&length);
            if (tp[i] == 'x')
               	printnum((long long int)va_arg(args, unsigned int),16,&length);
        }
         else
        {
        
            length += write (1, &tp[i],1);
        }
        i++;
    }
   return (va_end(args), length);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        write(1, "Usage: ./ft_printf <format_string>\n", 36);
        return 1;
    }
    
    int len = ft_printf(argv[1]);
    write(1, "\nLength: ", 9);
    printnum(len, 10, &len);
    write(1, "\n", 1);
    
    return 0;
}