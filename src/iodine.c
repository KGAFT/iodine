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
typedef struct {enum IodFieldType key; uint64_t value;} SortedFieldsMap;


void initializeDesc(struct IodStructDescriptor* desc, uint64_t len) {
  uint64_t fieldsSize = sizeof(enum IodFieldType)*len;
  enum IodFieldType* fields = iodmalloc(fieldsSize);
  memset(fields, 0, fieldsSize);
  fieldsSize = sizeof(uint64_t)*len;
  uint64_t* fieldsAmount = iodmalloc(fieldsSize);
  memset(fieldsAmount, 0, fieldsSize);
  desc->fields = fields;
  desc->fieldAmount = fieldsAmount;
}

void IodMakeStructDescriptor(struct IodStructFields* fields, void* accessor, struct IodStructDescriptor* desc){
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
      for(int32_t i = IOD_STRUCT; i>=0; i--){
	for(uint64_t ii = 0; ii< fields->entriesAmount; ii++){
	  if (fields->fields[ii] == i){
	    
	  }
	}
      }
  }
}

void IodBeginStruct(void* buffer, size_t bufferSize){

}
