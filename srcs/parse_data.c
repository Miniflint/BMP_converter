#include "../inc/bmp.h"
#include "../inc/main.h"

unsigned int parse(BYTE *str, t_node **root, int size)
{
    int i;
    int y;
    BYTE buf[BYTESREAD + 1];
    unsigned int size_linked_list;

    i = 0;
    size_linked_list = 0;
    while (i < size)
    {
        y = 0;
        while (i < size && y < BYTESREAD)
        {
            buf[y] = (str[i] * 1);
            y++;
            i++;
        }
        *root = create_node_from_buffer_and_insert(buf, *root);
        size_linked_list++;
        buf[0] = 0;
        buf[1] = 0;
        buf[2] = 0;
    }
    return (size_linked_list);
}

char *read_parse_bytes(t_header *h, BYTE *bytes)
{
    char *buffer;
    char *revert;
    int paddingSize;
    int i;
    int y;

    buffer = (char *) malloc(sizeof(char) * (h->fileSize));
    if (!buffer)
        return (NULL);
    my_memset(buffer, 0, h->fileSize);
    i = 0;
    y = 0;
    paddingSize = h->width & 3;
    while (y < (h->width * h->height * 3))
    {
        if (y && y % (h->width * 3) == 0)
            i += paddingSize;
        buffer[y] = (char) bytes[i];
        i++;
        y++;
    }
    buffer[y] = 0;
    revert = rev_strldup((char *) buffer, (h->width * h->height * 3));
    free(buffer);
    return (revert);
}
