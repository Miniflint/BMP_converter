#include "../inc/bmp.h"

void my_memcpy(void *dst, void *src, int size);
void my_memset(void *ptr, int c, int size);

/*
    https://fastbitlab.com/microcontroller-embedded-c-programming-lecture-116-bit-extraction

    ** also asked chatpgt how bit extraction works and how to store a large number into multiple small using hexadecimals **
    ** got a long 10-15 minutes read about bits, bytes, extraction etc **
    understood bit extraction
    (fileSize & 0x000000FF); extract the lower 8 bits;
    (fileSize & 0x000000FF) >> 8; shift by 8 so we can get the next 8 bits;


    AND operator:

    fileSize:    00000000 00000000 00000001 00000000 (256)
   &0x0000FF00:  00000000 00000000 11111111 00000000(255)
    fileSize>>8: 00000000 00000000 00000001 00000000(1)
                -----------------------------------
                 00000000 00000000 00000000 00000001 (1)
*/

static int __get_file_size(int width, int height)
{
    int padding;
    int rowSize;

    padding = width & 3;
    rowSize = (width * (24 / 8.0) + padding);
    return (rowSize * height + 54);
}

void __write_initBMPFileHeader(t_fileHeader *fileHeader, long fileSize)
{
    my_memset(fileHeader, 0, sizeof(t_fileHeader));
    fileHeader->headerName[0] = BITMAP_HEADER0;
    fileHeader->headerName[1] = BITMAP_HEADER1;
    fileHeader->headerFileSize[0] = (fileSize & 0x000000FF);
    fileHeader->headerFileSize[1] = (fileSize & 0x0000FF00) >> 8;
    fileHeader->headerFileSize[2] = (fileSize & 0x00FF0000) >> 16;
    fileHeader->headerFileSize[3] = (fileSize & 0xFF000000) >> 24;
    fileHeader->offsetPointer[0] = BITMAP_STARTPTR;
}

void __write_initBMPInfoHeader(t_infoHeader *infoHeader, int width, int height)
{
    int imageSize;

    imageSize = __get_file_size(width, height) - 54;
    my_memset(infoHeader, 0, sizeof(t_infoHeader));
    infoHeader->headerFixedSize[0] = BITMAP_HEADERSIZE;
    infoHeader->imageWidthInPixel[0] = (width & 0x000000FF);
    infoHeader->imageWidthInPixel[1] = (width & 0x0000FF00) >> 8;
    infoHeader->imageWidthInPixel[2] = (width & 0x00FF0000) >> 16;
    infoHeader->imageWidthInPixel[3] = (width & 0xFF000000) >> 24;
    infoHeader->imageHeightInPixel[0] = (height & 0x000000FF);
    infoHeader->imageHeightInPixel[1] = (height & 0x0000FF00) >> 8;
    infoHeader->imageHeightInPixel[2] = (height & 0x00FF0000) >> 16;
    infoHeader->imageHeightInPixel[3] = (height & 0xFF000000) >> 24;
    infoHeader->colorPlane[0] = BITMAP_COLORPLANE;
    infoHeader->numberBitsPerPixel[0] = BITMAP_BITPERPIXEL;
    infoHeader->imageSize[0] = (imageSize & 0x000000FF);
    infoHeader->imageSize[1] = (imageSize & 0x0000FF00) >> 8;
    infoHeader->imageSize[2] = (imageSize & 0x00FF0000) >> 16;
    infoHeader->imageSize[3] = (imageSize & 0xFF000000) >> 24;
}

void writeBMPHeader(t_header *header, FILE *f)
{
    fwrite(header->header, sizeof(BYTE), sizeof(header->header), f);
}

void write_all_with_linked_list_buffer(t_header *h, t_node *root, FILE *f)
{
    BYTE padding[4];
    int paddingSize;
    int fileSizeLeft;
    BYTE *bufferWrite;
    int i;
    int y;

    paddingSize = h->width % 4;
    writeBMPHeader(h, f);
    my_memset(padding, 0, 4);
    bufferWrite = (BYTE *) malloc(sizeof(BYTE) * (h->fileSize));
    if (!bufferWrite)
        return;
    fileSizeLeft = h->fileSize - sizeof(h->header);
    i = 1;
    y = 0;
    while (root->next)
    {
        bufferWrite[y] = root->b;
        bufferWrite[y + 1] = root->g;
        bufferWrite[y + 2] = root->r;
        y += 3;
        fileSizeLeft -= 3;
        if (i % h->width == 0)
        {
            my_memcpy(bufferWrite + y, padding, paddingSize);
            y += paddingSize;
            fileSizeLeft -= paddingSize;
        }
        root = root->next;
        i++;
    }
    if (fileSizeLeft > 0)
    {
        my_memset(bufferWrite + y, 0, fileSizeLeft);
        y += fileSizeLeft;
    }
    fwrite(bufferWrite, sizeof(BYTE), y, f);
    free(bufferWrite);
}

t_header *__write_init_headers(int width, int height)
{
    t_fileHeader fileHeader;
    t_infoHeader infoHeader;
    t_header *h;

    h = (t_header *) malloc(sizeof(t_header) * 1);
    if (!h)
        return (NULL);
    h->fileSize = __get_file_size(width, height);
    h->width = width;
    h->height = height;

    __write_initBMPFileHeader(&fileHeader, h->fileSize);
    __write_initBMPInfoHeader(&infoHeader, h->width, h->height);
    my_memcpy(h->header, &fileHeader, sizeof(t_fileHeader));
    my_memcpy(h->header + sizeof(t_fileHeader), &infoHeader, sizeof(t_infoHeader));
    return (h);
}
