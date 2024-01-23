#include "../inc/bmp.h"
#include "../inc/main.h"

static void __cleanup(t_header *h, int fd, char *message)
{
    if (h)
        free(h);
    if (fd)
        close(fd);
    if (message)
        fprintf(stderr, "%s\n", message);
}

int checks_BMPFileCorrect(t_fileHeader *fileHeader)
{
    int ret;

    ret = 0;
    if (fileHeader->headerName[0] != BITMAP_HEADER0 && fileHeader->headerName[1] != BITMAP_HEADER1)
    {
        printf("Could not read the headerName at 0x00->0x01\n");
        ret = 1;
    }
    if (fileHeader->headerFileSize[0] == 0)
    {
        printf("Could not read the headerFileSize at 0x02\n");
        ret = 1;
    }
    if (fileHeader->offsetPointer[0] != BITMAP_STARTPTR)
    {
        printf("Could not read the offset pointer at 0x0A\n");
        ret = 1;
    }
    return (ret);
}

int checks_BMPInfoCorrect(t_infoHeader *infoHeader)
{
    int ret;

    ret = 0;
    if (infoHeader->headerFixedSize[0] != BITMAP_HEADERSIZE)
    {
        printf("Wrong headerFixedSize number at 0x0E\n");
        ret = 1;
    }
    if (infoHeader->imageWidthInPixel[0] <= 0 && infoHeader->imageWidthInPixel[1] <= 0 && infoHeader->imageWidthInPixel[2] <= 0 && infoHeader->imageWidthInPixel[3] <= 0)
    {
        printf("Incorrect width\n");
        ret = 1;
    }
    if (infoHeader->imageHeightInPixel[0] <= 0 && infoHeader->imageHeightInPixel[1] <= 0 && infoHeader->imageHeightInPixel[2] <= 0 && infoHeader->imageHeightInPixel[3] <= 0)
    {
        printf("Incorrect height\n");
        ret = 1;
    }
    if (infoHeader->colorPlane[0] != BITMAP_COLORPLANE)
    {
        printf("Incorrect color Plane at 0x1A\n");
        ret = 1;
    }
    if (infoHeader->numberBitsPerPixel[0] != BITMAP_BITPERPIXEL)
    {
        printf("Incorrect Bitmap per pixel at 0x1E\n");
        ret = 1;
    }
    return (ret);
}

int __read_initBMPFileHeader(t_fileHeader *fileHeader, int fd)
{
    int size;

    if (fd == -1)
    {
        printf("%s\n", strerror(errno));
        return (1);
    }
    size = read(fd, fileHeader, sizeof(t_fileHeader));
    if (size == -1)
    {
        fprintf(stderr, "Could not read\n");
        return (1);
    }
    return (0);
}

int __read_initBMPinfoHeader(t_infoHeader *infoHeader, int fd)
{
    int size;

    if (fd == -1)
    {
        printf("%s\n", strerror(errno));
        return (1);
    }
    size = read(fd, infoHeader, sizeof(t_infoHeader));
    if (size == -1)
    {
        fprintf(stderr, "Could not read\n");
        return (1);
    }
    return (0);
}

static void __attribute_size_header(t_header *h)
{
    long headerFullSize;
    long headerRealFileSize;
    int widthFullSize;
    int heightFullSize;

    headerFullSize = h->header[0x02] + ((h->header[0x03] & 0x000000FF) << 8) + ((h->header[0x04] & 0x000000FF) << 16) +
                     ((h->header[0x05] & 0x000000FF) << 24);
    headerRealFileSize = h->header[0x06] + ((h->header[0x07] & 0x000000FF) << 8) + ((h->header[0x08] & 0x000000FF) << 16) +
                     ((h->header[0x09] & 0x000000FF) << 24);
    widthFullSize = h->header[0x12] + ((h->header[0x13] & 0x000000FF) << 8) + ((h->header[0x14] & 0x000000FF) << 16) +
                    ((h->header[0x15] & 0x000000FF) << 24);
    heightFullSize = h->header[0x16] + ((h->header[0x17] & 0x000000FF) << 8) + ((h->header[0x18] & 0x000000FF) << 16) +
                     ((h->header[0x19] & 0x000000FF) << 24);
    h->fileSize = headerFullSize;
    h->realFileSize = headerRealFileSize;
    h->width = widthFullSize;
    h->height = heightFullSize;
    if (!h->realFileSize)
        h->realFileSize = (h->width * h->height * 3);
}

t_header *__read_init_headers(char *filename, int fd)
{
    t_fileHeader fileHeader;
    t_infoHeader infoHeader;
    t_header *h;

    if (my_strstr(filename, ".bmp"))
    {
        printf("Wrong file extension\n");
        return (NULL);
    }
    h = (t_header *) malloc(sizeof(t_header) * 1);
    if (!h)
        return (NULL);
    if (get_size(fd) < (int) sizeof(h->header))
    {
        __cleanup(h, fd, "Could not recognize the file format");
        return (NULL);
    }
    if (__read_initBMPFileHeader(&fileHeader, fd))
    {
        __cleanup(h, fd, NULL);
        return (NULL);
    }
    if (checks_BMPFileCorrect(&fileHeader))
    {
        __cleanup(h, fd, NULL);
        return (NULL);
    }
    if (__read_initBMPinfoHeader(&infoHeader, fd))
    {
        __cleanup(h, fd, NULL);
        return (NULL);
    }
    if (checks_BMPInfoCorrect(&infoHeader))
    {
        __cleanup(h, fd, NULL);
        return (NULL);
    }
    my_memcpy(h->header, &fileHeader, sizeof(t_fileHeader));
    my_memcpy(h->header + sizeof(t_fileHeader), &infoHeader, sizeof(t_infoHeader));
    __attribute_size_header(h);
    return (h);
}
