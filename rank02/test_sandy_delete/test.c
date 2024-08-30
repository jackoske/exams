#include <stdio.h>
#include <unistd.h>

int ft_strlen(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}

int ft_putchar(char c)
{
    write(1, &c, 1);
    return 0;
}

int ft_putstr(char *str)
{
    int i = 0;
    while (str[i] != '\0') {
        ft_putchar(str[i]);
        i++;
    }
    return 0;
}

int ft_putstr_i(char *str, int i)
{
    while (i >= 0) {
        ft_putchar(str[i]);
        i--;
    }
    return 0;
}

int ft_is_space(char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
    {
        return 1;
    }
    return 0;
}

int putstr_while_not_space(char *str)
{
    int i = 0;
    while (!ft_is_space(str[i]) && str[i] != '\0')
    {
        ft_putchar(str[i]);
        i++;
    }
    return 0;
} 

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        ft_putstr("Error\n");
        return 1;
    }
    int i = ft_strlen(argv[1]) - 1;
    int len = i;
    while (i > 0)
    {
        int j = 0;
        while (!ft_is_space(argv[1][i - j]) && argv[1][i - j] != '\0')
            j++;

        putstr_while_not_space(&(argv[1][i - j + 1])); // Adjusted indexing
        i = i - j;
	while (ft_is_space(argv[1][i]) && argv[1][i] != '\0')
	    i--;
	if (argv[1][i] != ' ' && argv[1][i] != '\0')
        	ft_putchar(' '); // Add space between words
    }

    return 0;
}

