#ifndef IODINE_H
#define IODINE_H

#include <stddef.h>
#include <stdint.h>

#define ASSERT_NO_PADDING(type, expected) _Static_assert(sizeof(type) == (expected), "Struct has padding")

enum IodFieldType{
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

struct IodSTypeDescriptor{
  uint32_t expectedValue;
};

struct IodStructFields{
  enum IodFieldType* fields;
  uint64_t* fieldAmount;
  uint64_t entriesAmount;
};

struct IodStructDescriptor{
  enum IodFieldType* fields;
  uint64_t* fieldAmount;
  uint64_t entriesAmount;
};
//Accessor must be behind ASSERT_NO_PADDING and order of fields and accessor must be the same
void IodMakeStructDescriptor(struct IodStructFields* fields, void* pAccessorInOut,struct IodStructDescriptor* pDescriptorOut);
void IodBeginStruct(void* buffer, size_t bufferSize);

#endif
