#include <iodine.h>

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define ENDIAN 0
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define ENDIAN 1
#else
    #error "Unknown endianness"
#endif


const char LE_OR_BE = ENDIAN;  
const char SYSTEM_DESCRIPTOR = LE_OR_BE<<0;

void IodMakeStructDescriptor(struct IodStructFields* fields, void* accessor, struct IodStructDescriptor* desc){
  
}

void IodBeginStruct(void* buffer, size_t bufferSize){

}
