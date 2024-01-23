#include "../inc/bmp.h"
#include "../inc/main.h"

BYTE *read_hex(t_header *header, int fd)
{
    BYTE *buf;
    int sizeRead;
    int sizeLeft;

    sizeLeft = header->fileSize - sizeof(header->header) + 1;
    buf = (BYTE *) malloc(sizeof(BYTE) * (sizeLeft));
    if (!buf)
    {
        printf("Malloc error");
        return (NULL);
    }
    my_memset(buf, 0, sizeLeft);
    lseek(fd, 0, header->header[0x0A]);
    sizeRead = read(fd, buf, sizeLeft);
    buf[sizeRead] = 0;
    close(fd);
    return (buf);
}

int write_text(char *str, char *outfile, int size)
{
    FILE *f;
    int ret;

    f = fopen(outfile, "w");
    if (!f)
    {
        fprintf(stderr, "%s %s\n", outfile, strerror(errno));
        return (1);
    }
    ret = size;
    fwrite(str, sizeof(char), ret, f);
    fclose(f);
    return (0);
}

int decode(char *filename, char *outfile)
{
    t_header *h;
    BYTE *buf;
    char *new_buf;
    int fd;
    int out;

    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        printf("[%s] %s\n", filename, strerror(errno));
        return (0);
    }
    h = __read_init_headers(filename, fd);
    if (!h)
        return (1);
    buf = read_hex(h, fd);
    close(fd);
    if (!buf)
    {
        free(h);
        return (1);
    }
    new_buf = read_parse_bytes(h, buf);
    free(buf);
    if (!new_buf)
    {
        free(h);
        return (1);
    }
    out = write_text(new_buf, outfile, h->realFileSize);
    free(new_buf);
    free(h);
    if (out)
        return (1);
    return (0);
}
