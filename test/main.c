#include <iodine.h>
#include <stdio.h>
#include <string.h>
struct Acc{
  uint16_t* fField;
  char* sField;
  uint64_t* tField;
};
ASSERT_NO_PADDING(struct Acc, 3*sizeof(void*));

  
int main(){
  struct IodFieldType types[3];
  uint64_t amounts[3];
  amounts[0] = 1;
  amounts[1] = 1;
  amounts[2] = 1;
  memset(types, 0, sizeof(struct IodFieldType)*3);
  types[0].type = UINT_16;
  types[1].type = BYTE;
  types[2].type = UINT_64;
  struct IodStructFields fields;
  fields.fields = types;
  fields.fieldAmount = amounts;
  fields.entriesAmount = 3;
  struct Acc acc;
  memset(&acc, 0,sizeof(struct Acc));
  struct IodStructDescriptor desc;
  enum IodResult res = IodMakeStructDescriptor(&fields, &acc, &desc);
  printf("%i\n", res);
  printf("%i\n",(int) acc.fField);
	 return 0;
}
