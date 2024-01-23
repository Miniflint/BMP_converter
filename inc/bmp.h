#ifndef BMP_H
#define BMP_H

#include "../inc/main.h"

/*
    https://elcharolin.wordpress.com/2018/11/28/read-and-write-bmp-files-in-c-c

    and from what i saw:

    BITMAP HEADER FILE:
        2bytes; 0x00: headerName
        4bytes; 0x02: headerFileSize
        4bytes; 0x06: Reserved/Unused // i used it to store the real file size
        4bytes; 0x0A: headerOffsetPointer

    BITMAP INFORMATION HEADER:
        4bytes; 0x0E: headerFixedSize // always 40
        4bytes; 0x12: imageWidthInPixel
        4bytes; 0x16: imageHeightInPixel
        2bytes; 0x1A: ColorPlane // always 1
        2bytes; 0x1C: numberBitPerPixel // 1, 4, 8, 15, 24, 32, or 64
        4bytes; 0x1E: compressionMethod // 0
        4bytes; 0x22: imageSize //can be 0 if image is not compressed else size of compressed image
        4bytes; 0x26: horizontalResolution
        4bytes; 0x2A: VerticalResolution
        4bytes; 0x2E: numberColorUsed
        4bytes; 0x32: numberImportantColors


    BITMAP PIXEL INFORMATIONS:
        headerOffsetPointer; start
        LOOP READ:
            Blue, Green, Red * width
            2bytes: padding
*/
#define BITMAP_HEADER0 'B'
#define BITMAP_HEADER1 'M'
#define BITMAP_STARTPTR 54
#define BITMAP_HEADERSIZE 40
#define BITMAP_COLORPLANE 1
#define BITMAP_BITPERPIXEL 24

typedef unsigned char BYTE;

typedef struct S_WriteBitmapHeaderFile
{
    BYTE headerName[2];
    BYTE headerFileSize[4];
    BYTE __RESERVED_UNUSED[4];
    BYTE offsetPointer[4];
} t_fileHeader;

typedef struct S_WriteBitmapInformationHeader
{
    BYTE headerFixedSize[4];
    BYTE imageWidthInPixel[4];
    BYTE imageHeightInPixel[4];
    BYTE colorPlane[2];
    BYTE numberBitsPerPixel[2];
    BYTE compressionMethod[4];
    BYTE imageSize[4];
    BYTE horizontalResolution[4];
    BYTE verticalResolution[4];
    BYTE numberColorsUsed[4];
    BYTE numberImportantColors[4];
} t_infoHeader;

typedef struct S_BMPHeader
{
    BYTE header[sizeof(t_fileHeader) + sizeof(t_infoHeader)];
    long fileSize;
    long realFileSize;
    int width;
    int height;
} t_header;

/* FUNCTION BMP */
void writeBMPHeader(t_header *header, FILE *f);
void write_all_with_linked_list_buffer(t_header *h, t_node *root, FILE *f);

/* UTILS BMP */
t_header *__read_init_headers(char *filename, int fd);
int __read_initBMPFileHeader(t_fileHeader *fileHeader, int fd);
int __read_initBMPinfoHeader(t_infoHeader *infoHeader, int fd);

t_header *__write_init_headers(int width, int height);
void __write_initBMPFileHeader(t_fileHeader *fileHeader, long fileSize);
void __write_initBMPInfoHeader(t_infoHeader *infoHeader, int width, int height);

BYTE *read_hex(t_header *header, int fd);
char *read_parse_bytes(t_header *h, BYTE *bytes);

#endif
