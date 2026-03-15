#include <iodine.h>
#include <stb_ds.h>
#include <iodine_alloc.h>
#include <string.h>
#include <sha2.h>

#include "iod_endian.h"

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define ENDIAN 0
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define ENDIAN 1
#else
    #error "Unknown endianness"
#endif


constexpr char LE_OR_BE = ENDIAN;
constexpr char SYSTEM_DESCRIPTOR = LE_OR_BE << 0;

typedef struct {
    IodFieldType key;
    uint64_t value;
} SortedFieldsMap;


void initializeDesc(IodStructDescriptor *desc, uint64_t len) {
    uint64_t fieldsSize = sizeof(IodFieldType) * len;
    IodFieldType *fields = iodmalloc(fieldsSize);
    memset(fields, 0, fieldsSize);
    fieldsSize = sizeof(uint64_t) * len;
    uint64_t *fieldsAmount = iodmalloc(fieldsSize);
    memset(fieldsAmount, 0, fieldsSize);
    desc->fields = fields;
    desc->fieldAmount = fieldsAmount;
    desc->size = 0;
}

typedef struct uint128_t {
    uint64_t low;
    uint64_t high;
} uint128_t;

#define SIZE_CMP(type, field) max(sizeof(type),  field->explicitSize)

uint64_t getSize(IodFieldType *field) {
    switch (field->type) {
        case BYTE:
            return SIZE_CMP(char, field);
        case INT_16:
        case UINT_16:
            return SIZE_CMP(short, field);
        case UINT_32:
        case INT_32:
            return SIZE_CMP(uint32_t, field);
        case INT_64:
        case UINT_64:
            return SIZE_CMP(uint64_t, field);
        case F_32:
            return SIZE_CMP(float, field);
        case F_64:
            return SIZE_CMP(double, field);
        case INT_128:
        case UINT_128:
            return SIZE_CMP(uint128_t, field);
        case IOD_STRUCT:
            return field->explicitSize;
    }
    return 0;
}

IodResult IodMakeStructDescriptor(IodStructFields *fields, uint64_t *accessor,
                                  IodStructDescriptor *desc) {
    if (fields->entriesAmount > 0) {
        SortedFieldsMap *fieldsMap = NULL;
        hmdefault(fieldsMap, 0);
        for (uint64_t i = 0; i < fields->entriesAmount; i++) {
            uint64_t currentCount = hmget(fieldsMap, fields->fields[i]) + fields->fieldAmount[i];
            hmput(fieldsMap, fields->fields[i], currentCount);
        }
        uint64_t len = hmlen(fieldsMap);
        initializeDesc(desc, len);
        hmfree(fieldsMap);

        //Aligning from the most expensive type to the least expensive one
        uint64_t offsetCounter = 0;
        uint64_t descCounter = 0;
        for (int32_t i = IOD_STRUCT; i >= 0; i--) {
            for (uint64_t ii = 0; ii < fields->entriesAmount; ii++) {
                if ((int32_t) fields->fields[ii].type == i) {
                    uint64_t size = getSize(&fields->fields[ii]) * fields->fieldAmount[ii];
                    if (size == 0) {
                        return IOD_INCORRECT_SIZE;
                    }
                    accessor[ii] = offsetCounter;
                    desc->fields[descCounter].offset = offsetCounter;
                    offsetCounter += size;
                    desc->fields[descCounter].type = i;
                    desc->fieldAmount[descCounter] = fields->fieldAmount[ii];
                    desc->fields[descCounter].explicitSize = size;
                    descCounter++;
                }
            }
        }
        desc->size = offsetCounter + (2 + 4 * fields->entriesAmount)* sizeof(uint64_t)  + sizeof(uint64_t) * fields->entriesAmount + 1;
        desc->entriesAmount = descCounter;
        return IOD_SUCCESS;
    }
    return IOD_FAILED;
}

void *writeDescriptor(void *buffer, const IodStructDescriptor *desc, bool systemEndian);

void *writeAccessor(void *buffer, uint64_t *accessor, uint64_t fieldsAmount, bool systemEndian);

IodResult IodBeginStruct(IodStructDescriptor *desc, bool hashCheck, bool le, uint64_t *accessor, void *buffer,
                         size_t bufferSize) {
    if (desc->size+hashCheck?2568/8:0 > bufferSize) {
        return IOD_INCORRECT_SIZE;
    }

    memset(buffer, 0, bufferSize);
    bool systemEndian = true;
    if (le && ENDIAN == 0) {
        ((char*)buffer)[0] |= SYSTEM_DESCRIPTOR;
        ((char*)buffer)[0] |= ((char)hashCheck) << 1;
    } else {
        systemEndian = false;
        ((char*)buffer)[0] |= ((!ENDIAN)<<0);
        ((char*)buffer)[0] |= ((char)hashCheck) << 1;
    }

    uint64_t descStart = 1;
    if (hashCheck) {
        descStart += 256 / 8;
    }
    void *accBegin = writeDescriptor(buffer + descStart, desc, systemEndian);
    void *dataBegin = writeAccessor(accBegin, accessor, desc->entriesAmount, systemEndian);
    for (uint64_t i = 0; i < desc->entriesAmount; i++) {
        accessor[i] = (uint64_t) dataBegin + accessor[i];
    }
    return IOD_SUCCESS;
}

#define ENDI_CHECK_SWAP64(infoBuff, value, systemEndian) infoBuff = value; if (!systemEndian){bswap64(infoBuff);}
#define WRITE_AND_INCREMENT(buffer_ptr, value_ptr, type) memcpy(buffer_ptr, value_ptr, sizeof(type)); buffer_ptr += sizeof(type);

void *writeDescriptor(void *buffer, const IodStructDescriptor *desc, bool systemEndian) {
    uint64_t infoBuff = 0;
    ENDI_CHECK_SWAP64(infoBuff, desc->entriesAmount, systemEndian)
    WRITE_AND_INCREMENT(buffer, &infoBuff, uint64_t)
    ENDI_CHECK_SWAP64(infoBuff, desc->size, systemEndian)
    WRITE_AND_INCREMENT(buffer, &infoBuff, uint64_t)
    for (uint64_t i = 0; i < desc->entriesAmount; i++) {
        //Writing field meta
        IodFieldType *field = &desc->fields[i];
        ENDI_CHECK_SWAP64(infoBuff, field->explicitSize, systemEndian)
        WRITE_AND_INCREMENT(buffer, &infoBuff, uint64_t)

        ENDI_CHECK_SWAP64(infoBuff, field->offset, systemEndian)
        WRITE_AND_INCREMENT(buffer, &infoBuff, uint64_t)

        ENDI_CHECK_SWAP64(infoBuff, field->type, systemEndian)
        WRITE_AND_INCREMENT(buffer, &infoBuff, uint64_t)


        //Writing amounts of field if array
        ENDI_CHECK_SWAP64(infoBuff, desc->fieldAmount[i], systemEndian)
        WRITE_AND_INCREMENT(buffer, &infoBuff, uint64_t)
    }
    return buffer;
}

void *writeAccessor(void *buffer, uint64_t *accessor, uint64_t fieldsAmount, bool systemEndian) {
    uint64_t infoBuff = 0;
    for (uint64_t i = 0; i < fieldsAmount; i++) {
        ENDI_CHECK_SWAP64(infoBuff, accessor[i], systemEndian)
        WRITE_AND_INCREMENT(buffer, &infoBuff, uint64_t)
    }
    return buffer;
}
