#ifndef LIBTLV_TLV_H
#define LIBTLV_TLV_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __WINDOWS__
#define sscanf sscanf_s
#define API __declspec(dllexport)
#else
#define API
#endif

/*
 * Maybe file or buffer in memory
 * */
typedef struct tlvBuffer
{
    void *data;
    uint32_t len;
    uint8_t *(*read)(struct tlvBuffer *, uint32_t, uint32_t *); /*length wanto read, length readed, return readed data*/
    uint32_t (*write)(struct tlvBuffer *, uint8_t *, uint32_t); /*data, length wanto write, return writed*/
} tlvBuffer;

/*Reader*/
/**************************************************************/
typedef struct tlvReader
{
    tlvBuffer *buffer;
} tlvReader;

API tlvReader *tlvReadder_New();

API void tlvReader_Init(tlvReader *self, tlvBuffer *buffer);

API void tlvReader_Del(tlvReader **self);

API uint8_t tlvReader_ReadByte(tlvReader *self, bool *tag);

API uint8_t *tlvReader_ReadBytes(tlvReader *self, uint32_t len, uint32_t *readed); /*shadow copy ; len of char return*/

API uint8_t *tlvReader_ReadBytesShort(tlvReader *self, uint32_t *readed);

API API uint16_t tlvReader_ReadUInt16(tlvReader *self, bool *tag);

API int32_t tlvReader_ReadInt32(tlvReader *self, bool *tag);

API int64_t tlvReader_ReadInt64(tlvReader *self, bool *tag);

API char *tlvReader_ReadString(tlvReader *self, uint32_t *readed);

API char *tlvReader_ReadStringShort(tlvReader *self, uint32_t *readed);

API char *tlvReader_ReadStringLimit(tlvReader *self, uint32_t limit, uint32_t *readed);

API uint8_t *tlvReader_ReadAvailable(tlvReader *self, uint32_t *readed);


/*Writer*/
/**************************************************************/
typedef struct tlvWriter
{
    tlvBuffer *buffer;
} tlvWriter;

API tlvWriter *tlvWriter_New();

API void tlvWriter_Init(tlvWriter *self, tlvBuffer *buffer);

API void tlvWriter_Del(tlvWriter **self);

API uint32_t tlvWriter_Write(tlvWriter *self, uint8_t *data, uint32_t len);  /* return length of write */

API uint32_t tlvWriter_WriteHex(tlvWriter *self, char *data, uint32_t len); /* return length of write */

API bool tlvWriter_WriteByte(tlvWriter *self, uint8_t data);

API bool tlvWriter_WriteUInt16(tlvWriter *self, uint16_t data);

API bool tlvWriter_WriteUInt32(tlvWriter *self, uint32_t data);

API bool tlvWriter_WriteUInt64(tlvWriter *self, uint64_t data);

API bool tlvWriter_WriteString(tlvWriter *self, char *data, uint32_t len);

API bool tlvWriter_WriteStringShort(tlvWriter *self, char *data, uint32_t len);

API bool tlvWriter_WriteBytesShort(tlvWriter *self, uint8_t *data, uint32_t len);

API bool tlvWriter_WriteBool(tlvWriter *self, bool data);
/********************/
API uint8_t *tlv_HexToBytes(char *data, uint32_t len);

API void tlv_SwapEndian(uint8_t *data, uint32_t len);

API long tlv_FileLen(FILE *file);
#endif //LIBTLV_TLV_H
