//
// Created by Vin on 2021/10/28.
//
#include "tlv.h"

/* ffff out 255 255 */
uint8_t *tlv_HexToBytes(char *data, uint32_t len)
{
    uint8_t *dst = (uint8_t *) malloc(sizeof(uint8_t) * len / 2);
    unsigned int u = 0;
    uint32_t p = 0; /*pianyiliang */
    while (p < len && sscanf(data + p, "%2x", &u) == 1)
    {
        dst[p / 2] = (uint8_t) u;
        p += 2;
    }
    return dst;
}

void tlv_SwapEndian(uint8_t *data, uint32_t len)
{
    uint8_t temp;
    for (uint32_t i = 0; i < len / 2; i++)
    {
        temp = data[i];
        data[i] = data[len - 1 - i];
        data[len - 1 - i] = temp;
    }

}

long tlv_FileLen(FILE *file)
{
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}
