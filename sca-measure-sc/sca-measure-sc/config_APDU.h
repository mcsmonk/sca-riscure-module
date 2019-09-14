#pragma once

#ifndef __CONFIG_APDU_H__
#define __CONFIG_APDU_H__

#define APDU_POS_CLS 0x00
#define APDU_POS_INS 0x01
#define APDU_POS_P1  0x02
#define APDU_POS_P2  0x03
#define APDU_POS_LEN 0x04

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Korea financial card*/
//extern const unsigned char APDU_select[];
//extern const unsigned char APDU_enc[];
//extern const unsigned char APDU_getResponse[];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* CNT */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 1. ECDSA sign SPA */

extern const unsigned char APDU_selectDF[];
extern const unsigned char APDU_getResponse[];
extern const unsigned char APDU_CtrDrbgClearInternalState[];
extern const unsigned char APDU_CtrDrbgInstantiate[];
extern const unsigned char APDU_CtrDrbgReseed[];
extern const unsigned char APDU_updateBinary[];
extern const unsigned char APDU_cntcEcdsaSign[];
extern const unsigned char APDU_readBinary[];
extern const unsigned char APDU_cntcEcdsaVerify[];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //__CONFIG_APDU_H__