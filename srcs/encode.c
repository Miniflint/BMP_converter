#include "../inc/bmp.h"
#include "../inc/main.h"

void cleanup(t_header *h, t_node *root, BYTE *buf, char *message)
{
    if (root)
        free_linked_list(root);
    if (buf)
        free(buf);
    if (h)
        free(h);
    if (message)
        fprintf(stderr, "%s\n", message);
}

BYTE *readfile_bychar(char *filename, int *readSize)
{
    int fd;
    BYTE *buffer;
    int size;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "%s %s\n", filename, strerror(errno));
        return (NULL);
    }
    size = get_size(fd);
    buffer = (BYTE *) malloc(sizeof(char) * (size + 1));
    if (!buffer)
    {
        close(fd);
        return (NULL);
    }
    *readSize = read(fd, buffer, size);
    close(fd);
    if (*readSize == -1)
    {
        free(buffer);
        fprintf(stderr, "%s\n", strerror(errno));
        return (NULL);
    }
    buffer[*readSize] = 0;
    return (buffer);
}

int writeAll(t_header *h, t_node *root, char *filename)
{
    FILE *f;

    if (!root)
        return (1);
    f = fopen(filename, "wb");
    if (!f)
    {
        fprintf(stderr, "Could not open the file to write\n");
        return (1);
    }
    write_all_with_linked_list_buffer(h, root, f);
    fclose(f);
    return (0);
}


/*
    dad explanation on getting a rectangular shape to have the less amount of whitespace possible
    using divisors in commun
*/
int handle_array_divisor(t_header **h, unsigned int size_linked_list, int size_read)
{
    int *arrayDivisors;
    int sizeArray;
    long square;
    long height;

    square = fastSqrt((int) size_linked_list);
    height = square;
    arrayDivisors = (int *) malloc(sizeof(int) * (int) (square * 2));
    if (!arrayDivisors)
        return (1);
    sizeArray = get_divisors(size_linked_list, arrayDivisors, (int) (square * 2)) - 1;
    if (!sizeArray)
    {
        sizeArray = 2;
        brut_force_numbers(&square, &height, size_linked_list);
        arrayDivisors[1] = height;
        arrayDivisors[2] = square;
    }
    if (sizeArray == 1)
        *h = __write_init_headers(arrayDivisors[1], arrayDivisors[1]);
    else if ((sizeArray & 1) == 0)
        *h = __write_init_headers(arrayDivisors[(sizeArray / 2) + 1], arrayDivisors[(sizeArray / 2)]);
    else
        *h = __write_init_headers(arrayDivisors[(sizeArray / 2)], arrayDivisors[(sizeArray / 2)]);
    free(arrayDivisors);
    if (!(*h))
        return (1);
    (*h)->header[0x06] = (size_read & 0x000000FF);
    (*h)->header[0x07] = (size_read & 0x0000FF00) >> 8;
    (*h)->header[0x08] = (size_read & 0x00FF0000) >> 16;
    (*h)->header[0x09] = (size_read & 0xFF000000) >> 24;
    return (0);
}

int encode(char *filename, char *outfile)
{
    t_node *root;
    t_header *h;
    BYTE *buf;
    unsigned int size_linked_list;
    int size_read;

    root = create_empty_node();
    if (!root)
        return (1);
    buf = readfile_bychar(filename, &size_read);
    if (!buf)
    {
        cleanup(NULL, root, NULL, NULL);
        return (1);
    }
    size_linked_list = parse(buf, &root, size_read);
    if (!size_linked_list)
    {
        cleanup(NULL, root, buf, "Error with the file. reading 0");
        return (1);
    }
    if (handle_array_divisor(&h, size_linked_list, size_read))
    {
        cleanup(h, root, buf, "Error mallocing the list");
        return (1);
    }
    writeAll(h, root, outfile);
    cleanup(h, root, buf, NULL);
    return (0);
}
