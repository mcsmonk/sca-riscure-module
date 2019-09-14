
#ifndef __SMARTCARD_H__
#define __SMARTCARD_H__

#include "sca.h"
#include "powertracer_api.h"
#include "config_APDU.h"

#define BUFLEN 1024

extern pt_device sc_ptd;
extern int sc_ret;
extern int sc_ret_rx;
extern int sc_ret_tx;

int sc_device_open();
int sc_device_close();
int sc_device_print_info();
int sc_card_power_on();
int sc_card_power_off();

//#define CARD_PRINT_FN_RET(func,ret) printf("\t\t" #func "\n\t\t\tret = [%d] // ", ret)
//#define CARD_PRINT_UCHAR(x) printf(#x " : %u\n", x)
//#define CARD_PRINT_STRING(x) printf(#x " : %s\n", x)
//#define CARD_PRINT_ULONG(x) printf(#x " : %lu\n", x)


#endif //__SMARTCARD_H__
