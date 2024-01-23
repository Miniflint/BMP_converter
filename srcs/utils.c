#include "../inc/main.h"

long fastSqrt(int x)
{
    long i;

    if (x < 4)
        return ((x == 0) ? 0 : 1);
    i = x / (x / 2);
    while (i * i < x)
        i++;
    return (i);
}

void my_memset(void *ptr, int c, int size)
{
    int i;
    unsigned char *p;

    p = (unsigned char *) ptr;
    i = 0;
    while (i < size)
    {
        *p = (unsigned char) c;
        p++;
        i++;
    }
}

void my_memcpy(void *dst, void *src, int size)
{
    int i;
    unsigned char *p;

    i = 0;
    p = (unsigned char *) dst;
    while (i < size)
    {
        *p++ = *(unsigned char *) src++;
        i++;
    }
}

int get_divisors(int number, int *arr, int arrSize)
{
    int i;
    int y;

    i = 0;
    while (i < arrSize)
    {
        arr[i] = 0;
        i++;
    }
    i = 2;
    y = 1;
    while (i < number && y < arrSize)
    {
        if (number % i == 0)
        {
            arr[y] = i;
            y++;
        }
        i++;
    }
    return (y);
}

void brut_force_numbers(long *width, long *height, int total)
{
    int newWidth;
    int newHeight;

    newWidth = *width;
    newHeight = *height;
    while (newWidth * (newHeight - 1) >= total)
        newHeight--;
    while (newHeight > 1 && (newWidth + 1) * (newHeight - 1) >= total)
    {
        newWidth++;
        newHeight--;
    }
    *width = newWidth;
    *height = newHeight;
}

int my_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);
}

char *my_strchr(char *s1, char c)
{
    int i;

    i = 0;
    while (s1[i])
    {
        if (s1[i] == c)
            return (&s1[i]);
        i++;
    }
    return (NULL);
}

int my_strstr(char *s1, char *s2)
{
    int i;
    int y;

    i = 0;
    while (s1[i])
    {
        y = 0;
        while (s1[i + y] && s1[i + y] == s2[y])
        {
            y++;
            if (!s2[y])
                return (0);
        }
        i++;
    }
    return (1);
}

int get_size(int fd)
{
    int size;

    lseek(fd, 0, SEEK_END);
    size = lseek(fd, 0, SEEK_CUR);
    lseek(fd, 0, SEEK_SET);
    return (size);
}

char *rev_strldup(char *str, int size)
{
    char *new_str;
    int i;
    int y;

    i = 0;
    y = size - 1;
    new_str = (char *) malloc(sizeof(char) * size + 1);
    if (!new_str)
        return (NULL);
    while (y >= 0)
    {
        new_str[i] = str[y];
        i++;
        y--;
    }
    new_str[i] = 0;
    return (new_str);
}
