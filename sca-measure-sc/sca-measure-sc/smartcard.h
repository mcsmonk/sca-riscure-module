
#ifndef __SMARTCARD_H__
#define __SMARTCARD_H__

#include "sca.h"
#include "powertracer_api.h"


#define APDU_POS_CLS 0x00
#define APDU_POS_INS 0x01
#define APDU_POS_P1  0x02
#define APDU_POS_P2  0x03
#define APDU_POS_LEN 0x04

#define BUFLEN 1024

extern pt_device sc_ptd;
extern int sc_ret;
extern int sc_ret_rx;
extern int sc_ret_tx;

extern unsigned char sc_buf1[];
extern unsigned int len_sc_buf1;
extern unsigned char sc_buf2[];
extern unsigned int len_sc_buf2;

int sc_device_open();
int sc_device_close();
int sc_card_setting();
int sc_card_setting2();
int sc_device_print_info();
int sc_card_power_on();
int sc_card_power_off();
int sc_card_coldreset();
int sc_card_warmreset();

void sc_card_print_APDU_Lc(unsigned char buf[], int buf_len);
void sc_card_print_APDU_Le(unsigned char buf[], int buf_len);
void sc_card_print_RESPONSE(unsigned char buf[], int buf_len);
void sc_card_print_ATR(unsigned char buf[], int buf_len);

int sc_read(pt_device* sc_ptd, unsigned char* buf, unsigned int buflen, unsigned int response_len, unsigned int* actual_response_len);
int sc_write1(pt_device* sc_ptd, unsigned char* apdu_wo_len, unsigned char apdu_len, unsigned char* apdu_data);
int sc_write2(pt_device* sc_ptd, unsigned char* apdu, unsigned int len);


#endif //__SMARTCARD_H__
