
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//const unsigned char APDU_[] = {};
//const int APDU__len = sizeof();
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Korea financial card*/
//const unsigned char APDU_select[] = { 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD4, 0x10, 0x65, 0x09, 0x90, 0x00, 0x10 };
//const unsigned char APDU_enc[4] = { 0x90, 0xE4, 0x00, 0x81 };
//const unsigned char APDU_getResponse[4] = { 0x00, 0xC0, 0x00, 0x00 };
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* CNT */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* 1. ECDSA sign SPA */
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 00 A4 04 00 len -- DF name / AID
const unsigned char APDU_selectDF[] = { 0x00, 0xA4, 0x04, 0x00, 0x0D, 0xF0, 0x4B, 0x45, 0x59, 0x50, 0x41, 0x49, 0x52, 0x20, 0x43, 0x4E, 0x54, 0x43 };

// 00 C0 00 00 len
const unsigned char APDU_getResponse[] = { 0x00, 0xC0, 0x00, 0x00 };

// 80 BB 00 05 00
const unsigned char APDU_CtrDrbgClearInternalState[] = { 0x80, 0xBB, 0x00, 0x05, 0x00 };

// 80 AA 00 06 04 -- pbPersonalizationString (2) || uPersonalizationStringSize (2)
// !! pbPersonalizationString == 0000 -> NULL
const unsigned char APDU_CtrDrbgInstantiate[] = { 0x80, 0xAA, 0x00, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00 };

// 80 AA 00 07 04 -- pbAdditionalInput (2) || uAdditionalInputSize (2)
// !! pbAdditionalInput == 0000 -> NULL
const unsigned char APDU_CtrDrbgReseed[] = { 0x80, 0xAA, 0x00, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00 };

// 00 D6 pos1 pos2 len -- Data
// !! MSB of pos1 must be 0
const unsigned char APDU_updateBinary[] = { 0x00, 0xD6, 0x00, 0x00 };

// 80 AA 00 1B 0C -- pbR (2) || pbS (2) || pbMessage (2) || uSize (2) || iBlockType (2) || pbPriKey (2)
// !! Message, PriKey가 EF(INOUT)의 첫 바이트부터 각각 Size, 32 바이트 크기로 저장되어 있어야 함
// !! 수행 후 출력이 발생하는 경우 EF(INOUT)의 첫 바이트부터 각각 32, 32 바이트 크기의 R, S가 저장
const unsigned char APDU_cntcEcdsaSign[] = { 0x80, 0xAA, 0x00, 0x1B, 0x0C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x80, 0x00, 0x05, 0xFF, 0xFF };

// 00 B0 pos1 pos2 len
// !! MSB of pos1 must be 0
const unsigned char APDU_readBinary[] = { 0x00, 0xB0, 0x00, 0x00 };

// 80 AA 00 1C 0E -- pbMessage (2) || uSize (2) || iBlockType (2) || pbR (2) || pbS (2) || pbPubKeyX (2) || pbPubKeyY (2)
// !! Message, R, S, PubKeyX, PubKeyY가 EF(INOUT)의 첫 바이트부터 각각 Size, 32, 32, 32, 32 바이트 크기로 저장되어 있어야 함
const unsigned char APDU_cntcEcdsaVerify[] = { 0x80, 0xAA, 0x00, 0x1C, 0x0E, 0xFF, 0xFF, 0x00, 0x80, 0x00, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////