#include <iodine.h>
#include <stb_ds.h>
#include <iodine_alloc.h>
#include <string.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define ENDIAN 0
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define ENDIAN 1
#else
    #error "Unknown endianness"
#endif


const char LE_OR_BE = ENDIAN;  
const char SYSTEM_DESCRIPTOR = LE_OR_BE<<0;
typedef struct {struct IodFieldType key; uint64_t value;} SortedFieldsMap;


void initializeDesc(struct IodStructDescriptor* desc, uint64_t len) {
  uint64_t fieldsSize = sizeof(struct IodFieldType)*len;
  struct IodFieldType* fields = iodmalloc(fieldsSize);
  memset(fields, 0, fieldsSize);
  fieldsSize = sizeof(uint64_t)*len;
  uint64_t* fieldsAmount = iodmalloc(fieldsSize);
  memset(fieldsAmount, 0, fieldsSize);
  desc->fields = fields;
  desc->fieldAmount = fieldsAmount;
}

typedef struct uint128_t{
  uint64_t low;
  uint64_t high;
} uint128_t;

#define SIZE_CMP(type, field) max(sizeof(type),  field->explicitSize)

uint64_t getSize(struct IodFieldType* field){
  switch(field->type){
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

enum IodResult IodMakeStructDescriptor(struct IodStructFields* fields, void** accessor, struct IodStructDescriptor* desc){
  if (fields->entriesAmount > 0) {
      SortedFieldsMap* fieldsMap = NULL;
      hmdefault(fieldsMap, 0);
      for(uint64_t i = 0; i< fields->entriesAmount; i++){
	uint64_t currentCount = hmget(fieldsMap, fields->fields[i])+fields->fieldAmount[i];
	hmput(fieldsMap, fields->fields[i], currentCount);
      }
      uint64_t len = hmlen(fieldsMap);
      initializeDesc(desc, len);
      
      //Aligning from the most expensive type to the least expensive
      uint64_t offsetCounter = 0;
      for(int32_t i = IOD_STRUCT; i>=0; i--){
	for(uint64_t ii = 0; ii< fields->entriesAmount; ii++){
	  if (fields->fields[ii].type == i){
	    uint64_t size = getSize(&fields->fields[ii]);
	    if(size==0){
	      return IOD_FAILED_INCORRECT_SIZE;
	    }
	    accessor[ii] = (void*) offsetCounter;
	    offsetCounter+=size;
	  }
	}
      }
      desc->size = offsetCounter;
      return IOD_SUCCESS;
  }
  return IOD_FAILED;
}

void IodBeginStruct(void* buffer, size_t bufferSize){

}


