
#include "card.h"

const unsigned char command_selectdf[5 + 7] = { 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD4, 0x10, 0x65, 0x09, 0x90, 0x00, 0x10 };
//const unsigned char command_selectdf[] = "00 A4 04 00 07 D4 10 65 09 90 00 10";
const unsigned int command_len_selectdf = sizeof(command_selectdf);