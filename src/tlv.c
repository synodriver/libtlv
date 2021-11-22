#include "tlv.h"

tlvReader *tlvReadder_New()
{
    tlvReader *self = (tlvReader *) calloc(1, sizeof(tlvReader));
    if (self == NULL)
    {
        return NULL;
    }
    return self;
}

void tlvReader_Init(tlvReader *self, tlvBuffer *buffer)
{
    self->buffer = buffer;
}

void tlvReader_Del(tlvReader **self)
{
    if (*self != NULL)
    {
        free(*self);
        *self = NULL;
    }
}

uint8_t tlvReader_ReadByte(tlvReader *self, bool *tag)
{
    uint32_t readed = 0;
    uint8_t *data = self->buffer->read(self->buffer, 1, &readed);
    if (readed)
    {
        *tag = true;
    } else
    {
        *tag = false;
    }
    uint8_t ret = *data;
    free(data);
    return ret;
}

uint8_t *tlvReader_ReadBytes(tlvReader *self, uint32_t len, uint32_t *readed)
{
    return self->buffer->read(self->buffer, len, readed);
}

uint8_t *tlvReader_ReadBytesShort(tlvReader *self, uint32_t *readed)
{
    bool tag = false;
    uint8_t *data = tlvReader_ReadBytes(self, tlvReader_ReadUInt16(self, &tag), readed); //todo leak
    if (!tag)
    {
        *readed = 0;
        return 0;
    }
    return data;
}

uint16_t tlvReader_ReadUInt16(tlvReader *self, bool *tag)
{
    uint32_t readed = 0;
    uint8_t *data = tlvReader_ReadBytes(self, 2, &readed);
    if (readed != 2)
    {
        *tag = false;
        return 0;
    }
    *tag = true;
//    tlv_SwapEndian(data, 2);
    uint16_t ret = *(uint16_t *) data;
    free(data);
    data = NULL;
    return ret;
}

int32_t tlvReader_ReadInt32(tlvReader *self, bool *tag)
{
    uint32_t readed = 0;
    uint8_t *data = tlvReader_ReadBytes(self, 4, &readed);
    if (readed != 4)
    {
        *tag = false;
        return 0;
    }
    *tag = true;
//    tlv_SwapEndian(data, 4);
    uint32_t ret = *(int32_t *) data;
    free(data);
    data = NULL;
    return ret;
}

int64_t tlvReader_ReadInt64(tlvReader *self, bool *tag)
{
    uint32_t readed = 0;
    uint8_t *data = tlvReader_ReadBytes(self, 8, &readed);
    if (readed != 8)
    {
        *tag = false;
        return 0;
    }
    *tag = true;
//    tlv_SwapEndian(data, 8);
    int64_t ret = *(int64_t *) data;
    free(data);
    data = NULL;
    return ret;
}

char *tlvReader_ReadString(tlvReader *self, uint32_t *readed)
{
    bool tag = false;
    uint32_t len = tlvReader_ReadInt32(self, &tag);
    if (!tag)
    {
        *readed = 0;
        return NULL;
    }
    tlv_SwapEndian((uint8_t *) &len, 4);
    uint8_t *data = tlvReader_ReadBytes(self, len - 4, readed);

    return (char *) data;
}

char *tlvReader_ReadStringShort(tlvReader *self, uint32_t *readed)
{
    bool tag = false;
    uint8_t *data = tlvReader_ReadBytes(self, tlvReader_ReadUInt16(self, &tag), readed);
    if (!tag)
    {
        *readed = 0;
        return NULL;
    }
    return (char *) data;
}

char *tlvReader_ReadStringLimit(tlvReader *self, uint32_t limit, uint32_t *readed)
{
    uint8_t *data = tlvReader_ReadBytes(self, limit, readed);
    return (char *) data;
}

uint8_t *tlvReader_ReadAvailable(tlvReader *self, uint32_t *readed)
{
    return tlvReader_ReadBytes(self, self->buffer->len, readed);
}

/*****************************/


tlvWriter *tlvWriter_New()
{
    tlvWriter *self = (tlvWriter *) calloc(1, sizeof(tlvWriter));
    if (self == NULL)
    {
        return NULL;
    }
    return self;
}

void tlvWriter_Init(tlvWriter *self, tlvBuffer *buffer)
{
    self->buffer = buffer;
}

void tlvWriter_Del(tlvWriter **self)
{
    if (*self != NULL)
    {
        free(*self);
        *self = NULL;
    }
}

uint32_t tlvWriter_Write(tlvWriter *self, uint8_t *data, uint32_t len)
{
    return self->buffer->write(self->buffer, data, len);
}

/*  data= "ffffffff" */
uint32_t tlvWriter_WriteHex(tlvWriter *self, char *data, uint32_t len)
{
    uint8_t *dst = tlv_HexToBytes(data, len);
    uint32_t writed = tlvWriter_Write(self, dst, len / 2);
    free(dst);
    dst = NULL;
    return writed;
}

bool tlvWriter_WriteByte(tlvWriter *self, uint8_t data)
{
    return (bool) self->buffer->write(self->buffer, &data, 1);
}

bool tlvWriter_WriteUInt16(tlvWriter *self, uint16_t data)
{
    uint8_t *temp = (uint8_t *) &data;
    //tlv_SwapEndian(temp, 2);
    uint32_t writted = tlvWriter_Write(self, temp, 2);
    if (writted == 2)
    {
        return true;
    } else
    {
        return false;
    }
}

bool tlvWriter_WriteUInt32(tlvWriter *self, uint32_t data)
{
    uint8_t *temp = (uint8_t *) &data;
    //tlv_SwapEndian(temp, 4);
    uint32_t writted = tlvWriter_Write(self, temp, 4);
    if (writted == 4)
    {
        return true;
    } else
    {
        return false;
    }
}


bool tlvWriter_WriteUInt64(tlvWriter *self, uint64_t data)
{
    uint8_t *temp = (uint8_t *) &data;
    //tlv_SwapEndian(temp, 8);
    uint32_t writted = tlvWriter_Write(self, temp, 8);
    if (writted == 8)
    {
        return true;
    } else
    {
        return false;
    }
}

bool tlvWriter_WriteString(tlvWriter *self, char *data, uint32_t len)
{
    bool ok = tlvWriter_WriteUInt32(self, len + 4);
    if (!ok)
    {
        return false;
    }
    uint32_t writed = tlvWriter_Write(self, (uint8_t *) data, len);
    if (writed != len)
    {
        return false;
    }
    return true;
}

bool tlvWriter_WriteStringShort(tlvWriter *self, char *data, uint32_t len)
{
    return tlvWriter_WriteBytesShort(self, (uint8_t *) data, len);
}

bool tlvWriter_WriteBytesShort(tlvWriter *self, uint8_t *data, uint32_t len)
{
    bool ok = tlvWriter_WriteUInt16(self, (uint16_t) len);
    if (!ok)
    {
        return false;
    }
    uint32_t writed = tlvWriter_Write(self, data, len);
    if (writed != len)
    {
        return false;
    }
    return true;
}

bool tlvWriter_WriteBool(tlvWriter *self, bool data)
{
    bool ok = false;
    if (data)
    {
        ok = tlvWriter_WriteByte(self, 1);
    } else
    {
        ok = tlvWriter_WriteByte(self, 0);
    }
    return ok;
}
