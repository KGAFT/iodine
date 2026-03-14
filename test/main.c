#include <iodine.h>
#include <stdio.h>
#include <string.h>

struct Acc {
    uint16_t *fField;
    char *sField;
    uint64_t *tField;
    float *ffField;
    double *ddField;
    double *arrField;
};

ASSERT_NO_PADDING(struct Acc, 6*sizeof(void*));


int main() {
    struct IodFieldType types[6];
    uint64_t amounts[6];
    amounts[0] = 1;
    amounts[1] = 1;
    amounts[2] = 1;
    amounts[3] = 1;
    amounts[4] = 1;
    amounts[5] = 55;
    memset(types, 0, sizeof(struct IodFieldType) * 6);
    types[0].type = UINT_16;
    types[1].type = BYTE;
    types[2].type = UINT_64;
    types[3].type = F_32;
    types[4].type = F_64;
    types[5].type = F_64;
    struct IodStructFields fields;
    fields.fields = types;
    fields.fieldAmount = amounts;
    fields.entriesAmount = 6;
    struct Acc acc;
    memset(&acc, 0, sizeof(struct Acc));
    struct IodStructDescriptor desc;
    enum IodResult res = IodMakeStructDescriptor(&fields, (void **) &acc, &desc);
    printf("%i\n", res);
    printf("%i\n", (int) acc.fField);
    return 0;
}
