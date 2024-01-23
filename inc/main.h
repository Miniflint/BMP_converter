#ifndef MAIN_H
#define MAIN_H

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BYTESREAD 3

typedef struct S_node
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    struct S_node *next;
} t_node;

typedef unsigned char BYTE;

int encode(char *filename, char *outfile);
int decode(char *filename, char *outfile);

unsigned int parse(BYTE *str, t_node **root, int size);
BYTE *readfile_bychar(char *filename, int *readSize);

/* linked list utils */
t_node *create_empty_node(void);
t_node *create_node_from_values(int r, int g, int b);
t_node *create_node_from_buffer(BYTE buffer[BYTESREAD]);
t_node *create_node_from_buffer_and_insert(BYTE buffer[BYTESREAD], t_node *insert_at);
void print_linked_list(t_node *root);
void free_linked_list(t_node *root);
void remove_last_node_linked_list(t_node *root);

/* utils */
int get_size(int fd);
long fastSqrt(int x);
char *my_strchr(char *s1, char c);
int my_strcmp(char *s1, char *s2);
int my_strstr(char *s1, char *s2);
char *rev_strldup(char *str, int size);
void my_memcpy(void *dst, void *src, int size);
void my_memset(void *ptr, int c, int size);
int handle_error(char *str, int error_code);
int get_divisors(int number, int *arr, int arrSize);
void brut_force_numbers(long *width, long *height, int total);

#endif
