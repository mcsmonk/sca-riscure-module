
//const unsigned char command_selectdf[5 + 7] = { 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD4, 0x10, 0x65, 0x09, 0x90, 0x30, 0x10 }; // ����
const unsigned char command_selectdf[5 + 7] = { 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD4, 0x10, 0x65, 0x09, 0x90, 0x00, 0x10 }; // ����������
//const unsigned char command_selectdf[5 + 7] = { 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD4, 0x10, 0x65, 0x09, 0x90, 0x00, 0x20 }; // ����ȭ��
//const unsigned char command_selectdf[5 + 7] = { 0x00, 0xA4, 0x04, 0x00, 0x07, 0xD4, 0x10, 0x65, 0x09, 0x90, 0x00, 0x30 }; // ����������(������)
const unsigned int command_len_selectdf = sizeof(command_selectdf);

const unsigned char command_getencipher[4] = { 0x90, 0xE4, 0x00, 0x81 };
const unsigned int command_len_getencipher = sizeof(command_getencipher);

const unsigned char command_getresponse[4] = { 0x00, 0xC0, 0x00, 0x00 };
const unsigned int command_len_getresponse = sizeof(command_getresponse);