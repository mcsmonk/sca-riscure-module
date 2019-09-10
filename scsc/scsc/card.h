
#ifndef __CARD_H__
#define __CARD_H__

#include "powertracer_api.h"

pt_device card_ptd;
int card_ret_ptd;
int card_ret_ptd_rx;
int card_ret_ptd_tx;

extern const unsigned char command_selectdf[];
extern const unsigned int command_len_selectdf;
extern const unsigned char command_getencipher[];
extern const unsigned int command_len_getencipher;
extern const unsigned char command_getresponse[];
extern const unsigned int command_len_getresponse;

#define CARD_PRINT_FN_RET(func,ret) printf("\t\t" #func "\n\t\t\tret = [%d] // ", ret)
#define CARD_PRINT_UCHAR(x) printf(#x " : %u\n", x)
#define CARD_PRINT_STRING(x) printf(#x " : %s\n", x)
#define CARD_PRINT_ULONG(x) printf(#x " : %lu\n", x)

void card_pt_device_open();
void card_pt_device_close();
void card_pt_device_print_info();

void card_pt_device_power_on();
void card_pt_device_power_off();
void card_pt_device_coldreset();
void card_pt_device_warmreset();
void card_pt_device_print_channel();

void card_pt_device_select_DF();

void card_print_APDU_Lc(unsigned char buf[], unsigned int buf_len);
void card_print_APDU_Le(unsigned char buf[], unsigned int buf_len);
void card_print_RESPONSE(unsigned char buf[], unsigned int buf_len);

#endif //__CARD_H__
