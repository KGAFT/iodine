#ifndef IODINE_H
#define IODINE_H

#include <stddef.h>
#include <stdint.h>

#define ASSERT_NO_PADDING(type, expected) _Static_assert(sizeof(type) == (expected), "Struct has padding")

#define max(a, b) ((a) > (b) ? (a) : (b))
  
typedef enum IodResult{
  IOD_SUCCESS = 0,
  IOD_FAILED,
  IOD_INCORRECT_SIZE
} IodResult;

typedef enum IodFieldTypeEn{
  BYTE = 0,
  INT_16,
  UINT_16,
  INT_32,
  UINT_32,
  F_32,
  INT_64,
  UINT_64,
  F_64,
  INT_128,
  UINT_128,
  IOD_STRUCT,
} IodFieldTypeEn;

typedef struct IodFieldType{
  uint64_t explicitSize;
  uint64_t offset;
  uint64_t type;
} IodFieldType;

ASSERT_NO_PADDING(IodFieldType, sizeof(uint64_t)*3);

typedef struct IodSTypeDescriptor{
  uint32_t expectedValue;
} IodSTypeDescriptor;

typedef struct IodStructFields{
  IodFieldType* fields;
  uint64_t* fieldAmount;
  uint64_t entriesAmount;
} IodStructFields;

typedef struct IodStructDescriptor{
  IodFieldType* fields;
  uint64_t* fieldAmount;
  uint64_t entriesAmount;
  uint64_t size;
} IodStructDescriptor;
ASSERT_NO_PADDING(struct IodStructDescriptor, sizeof(void*)*2+sizeof(uint64_t)*2);

//Accessor must be behind ASSERT_NO_PADDING and order of fields and accessor must be the same
IodResult IodMakeStructDescriptor(IodStructFields* fields, void** pAccessorInOut, IodStructDescriptor* pDescriptorOut);
IodResult IodBeginStruct(IodStructDescriptor* desc, void** accessor, void *buffer, size_t bufferSize);

#endif
