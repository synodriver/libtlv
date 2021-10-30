//
// Created by Vin on 2021/10/28.
//
#include <string.h>
#include "src/tlv.h"

uint8_t *read_func(tlvBuffer *self, uint32_t len, uint32_t *readed)
{
    FILE *file = (FILE *) self->data;
    uint8_t *buffer = (uint8_t *) malloc(sizeof(uint8_t) * len);
    if (buffer == NULL)
    {
        *readed = 0;
        return NULL;
    }
    *readed = fread(buffer, sizeof(uint8_t), len, file);
    return buffer;
}


uint32_t write_func(tlvBuffer *self, uint8_t *data, uint32_t len)
{
    FILE *file = (FILE *) self->data;
    return fwrite(data, sizeof(uint8_t), len, file);
}

int printusage()
{
    printf("-h show help msg\n--input input file\n");
    return 0;
}


int main(int argc, char **argv)
{
    char *md5, *name, *url, *filename;
    if (argc < 3)
    {
        return printusage();
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        return printusage();
    } else if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--input") == 0)
    {
        filename = argv[2];
    }


    uint32_t size;
    tlvBuffer filebuffer;
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "no such file\n");
        return -1;
    }
    filebuffer.len = tlv_FileLen(file);
    filebuffer.data = (void *) file;
    filebuffer.read = &read_func;
    filebuffer.write = &write_func;

    tlvReader *reader = tlvReadder_New();
    if (reader == NULL)
    {
        fprintf(stderr, "malloc reader error\n");
        return -1;
    }
    tlvReader_Init(reader, &filebuffer);

    uint32_t readed = 0;
    md5 = (char *) tlvReader_ReadBytes(reader, 16, &readed);
    if (readed != 16)
    {
        fprintf(stderr, "md5 error\n");
        return -1;
    }
    printf("md5: ");
    fwrite(md5, sizeof(char), 16, stdout);
    printf("\n");

    bool ok = false;
    size = tlvReader_ReadInt32(reader, &ok);
    if (!ok)
    {
        fprintf(stderr, "size error\n");
        return -1;
    }
    printf("size: %u\n", size);


    name = tlvReader_ReadString(reader, &readed);
    if (!readed)
    {
        fprintf(stderr, "name error\n");
        return -1;
    }
    printf("name: ");
    fwrite(name, sizeof(char), readed, stdout);
    printf("\n");

    url = tlvReader_ReadString(reader, &readed);
    if (!readed)
    {
        fprintf(stderr, "url error\n");
        return -1;
    }
    printf("url: ");
    fwrite(url, sizeof(char), readed, stdout);
    printf("\n");

    free(md5);
    free(name);
    free(url);

    fclose(file);
    tlvReader_Del(&reader);
    return 0;
}