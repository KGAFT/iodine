#ifndef IODINE_H
#define IODINE_H

#include <stddef.h>
#include <stdint.h>

#define ASSERT_NO_PADDING(type, expected) _Static_assert(sizeof(type) == (expected), "Struct has padding")

#define max(a, b) ((a) > (b) ? (a) : (b))
  
 enum IodResult{
  IOD_SUCCESS = 0,
  IOD_FAILED,
  IOD_FAILED_INCORRECT_SIZE
};

enum IodFieldTypeEn{
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
};

struct IodFieldType{
  uint64_t explicitSize;
  uint64_t type;
};

ASSERT_NO_PADDING(struct IodFieldType, sizeof(uint64_t)+sizeof(uint64_t));

struct IodSTypeDescriptor{
  uint32_t expectedValue;
};

struct IodStructFields{
  struct IodFieldType* fields;
  uint64_t* fieldAmount;
  uint64_t entriesAmount;
};

struct IodStructDescriptor{
  struct IodFieldType* fields;
  uint64_t* fieldAmount;
  uint64_t entriesAmount;
  uint64_t size;
};
//Accessor must be behind ASSERT_NO_PADDING and order of fields and accessor must be the same
enum IodResult IodMakeStructDescriptor(struct IodStructFields* fields, void** pAccessorInOut,struct IodStructDescriptor* pDescriptorOut);
void IodBeginStruct(void* buffer, size_t bufferSize);

#endif
