
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <direct.h>

#include "sca.h"

FILE* fd_pt = NULL;
FILE* fd_ct = NULL;
FILE* fd_log = NULL;
FILE* fd_tr = NULL;

char buf_fd_pt[1024];
char buf_fd_ct[1024];
char buf_fd_log[1024];
char buf_fd_tr[1024];

int rv;

unsigned char APDU_test[] = { 0x00, 0x84, 0x00, 0x00, 0x10 };

unsigned char APDU_selectDF[] = { 0x00, 0xA4, 0x04, 0x00, 0x0D, 0xF0, 0x4B, 0x45, 0x59, 0x50, 0x41, 0x49, 0x52, 0x20, 0x43, 0x4E, 0x54, 0x43 };
unsigned char APDU_getResponse_selectDF[] = { 0x00, 0xC0, 0x00, 0x00, 0x0A };

unsigned char APDU_CtrDrbgClearInternalState[] = { 0x80, 0xBB, 0x00, 0x05, 0x00 };
unsigned char APDU_CtrDrbgInstantiate[] = { 0x80, 0xAA, 0x00, 0x06, 0x04, 0x00, 0x00, 0x00, 0x00 };
unsigned char APDU_CtrDrbgReseed[] = { 0x80, 0xAA, 0x00, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00 };

unsigned char APDU_EcdhKeytokenGen[] = { 0x80, 0xAA, 0x00, 0x1D, 0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
unsigned char APDU_readBinary_0x60[] = { 0x00, 0xB0, 0x00, 0x00, 0x60 };

unsigned char APDU_updateBinary_0x60[5 + 0x60] = { 0x00, 0xD6, 0x00, 0x00, 0x60, };
unsigned char APDU_EcdhSesKeyGen[] = { 0x80, 0xAA, 0x00, 0x1E, 0x0A, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
unsigned char APDU_readBinary_0x40[] = { 0x00, 0xB0, 0x00, 0x00, 0x40 };

void sca_fprintf(FILE* fp, const char* format, ...)
{
	va_list ap;
	char buf[4096];

	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);

	fprintf(stderr, "%s", buf);
	if (fp != NULL) {
		fprintf(fp, "%s", buf);
	}
}

int open_fd()
{
	time_t timer;
	struct tm* time_tmp;

	int ret;
	
	ret = mkdir("trace");
	if (ret == 0) {
		fprintf(stderr, "mkdir folder : %s\n", FILE_DIR);
	}
	else {
		fprintf(stderr, "folder already exist ... maybe ?\n");
	}

	timer = time(NULL);
	time_tmp = localtime(&timer);

	sprintf(buf_fd_pt, "%s\\%s-%02d-%02d-%02d-%02d-%02d-%02d.txt",
		FILE_DIR, FILE_PREFIX_PT,
		(time_tmp->tm_year + 1900) % 100, time_tmp->tm_mon + 1, time_tmp->tm_mday,
		time_tmp->tm_hour, time_tmp->tm_min, time_tmp->tm_sec);
	fd_pt = fopen(buf_fd_pt, "w");

	sprintf(buf_fd_ct, "%s\\%s-%02d-%02d-%02d-%02d-%02d-%02d.txt",
		FILE_DIR, FILE_PREFIX_CT,
		(time_tmp->tm_year + 1900) % 100, time_tmp->tm_mon + 1, time_tmp->tm_mday,
		time_tmp->tm_hour, time_tmp->tm_min, time_tmp->tm_sec);
	fd_ct = fopen(buf_fd_ct, "w");

	sprintf(buf_fd_log, "%s\\%s-%02d-%02d-%02d-%02d-%02d-%02d.txt",
		FILE_DIR, FILE_PREFIX_LOG,
		(time_tmp->tm_year + 1900) % 100, time_tmp->tm_mon + 1, time_tmp->tm_mday,
		time_tmp->tm_hour, time_tmp->tm_min, time_tmp->tm_sec);
	fd_log = fopen(buf_fd_log, "w");

	sprintf(buf_fd_tr, "%s\\%s-%02d-%02d-%02d-%02d-%02d-%02d.tr",
		FILE_DIR, FILE_PREFIX_TR,
		(time_tmp->tm_year + 1900) % 100, time_tmp->tm_mon + 1, time_tmp->tm_mday,
		time_tmp->tm_hour, time_tmp->tm_min, time_tmp->tm_sec);
	fd_tr = fopen(buf_fd_tr, "wb");

	if (!fd_pt || !fd_ct || !fd_log || !fd_tr) {
		fprintf(stderr, "Error : file open fail // %s ( %d line )\n", __FILE__, __LINE__);
		goto err;
	}

	return 0;

err:
	close_fd();
	return -1;
}

void close_fd()
{
	if (fd_pt) {
		fclose(fd_pt);
		fd_pt = NULL;
	}

	if (fd_ct) {
		fclose(fd_ct);
		fd_ct = NULL;
	}

	if (fd_log) {
		fclose(fd_log);
		fd_log = NULL;
	}

	if (fd_tr) {
		fclose(fd_tr);
		fd_tr = NULL;
	}
}

int set_dso()
{
	rv = LeCroyDSOConnect_wrapper(DSO_IPADDR);
	//LeCroyDSOStoreWaveformAllPoints(DSO_CHANNEL, buf_trace, &buf_trace_len);
	rv = LeCroyDSODisconnect_wrapper();
	return 0;
}

void test()
{
	int ret;
	
	ret = open_fd();
	if (ret) goto err;

	ret = sc_device_open();
	if (ret) goto err;

	ret = sc_card_power_on();
	//if (ret) goto err;
	while (ret) ret = sc_card_power_on();

	ret = sc_card_warmreset();
	while (ret) ret = sc_card_warmreset();

	//ret = sc_card_coldreset();
	//while (ret) ret = sc_card_coldreset();

	ret = sc_write2(&sc_ptd, (unsigned char*)APDU_test, sizeof(APDU_test));
	if (ret) goto err;

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, (unsigned char*)sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_write2(&sc_ptd, (unsigned char*)APDU_readBinary_0x60, sizeof(APDU_readBinary_0x60));
	if (ret) goto err;

	Sleep(TIMEOUT*5);

	ret = sc_read(&sc_ptd, (unsigned char*)sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_write2(&sc_ptd, (unsigned char*)APDU_updateBinary_0x60, sizeof(APDU_updateBinary_0x60));
	if (ret) goto err;

	Sleep(TIMEOUT * 5);

	ret = sc_read(&sc_ptd, (unsigned char*)sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT * 5);

	pt_set_armed(&sc_ptd, TRUE);
	ret = sc_write2(&sc_ptd, (unsigned char*)APDU_selectDF, sizeof(APDU_selectDF));
	if (ret) goto err;

	Sleep(TIMEOUT * 30);

	ret = sc_read(&sc_ptd, (unsigned char *) sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, (unsigned char*)sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, (unsigned char*)sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, (unsigned char*)sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, (unsigned char*)sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_write1(&sc_ptd, (unsigned char*)APDU_getResponse_selectDF, APDU_getResponse_selectDF[APDU_POS_LEN], NULL);
	if (ret) goto err;
	//ret = sc_write2(&sc_ptd, (unsigned char*)APDU_getResponse_selectDF, sizeof(APDU_getResponse_selectDF));
	//if (ret) goto err;

	//Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, (unsigned char*)sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, (unsigned char*)sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, (unsigned char*)sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	ret = sc_write2(&sc_ptd, (unsigned char*)APDU_CtrDrbgInstantiate, sizeof(APDU_CtrDrbgInstantiate));
	if (ret) goto err;

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, (unsigned char*)sc_buf1, BUFLEN, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_card_power_off();
	if (ret) goto err;

err:
	ret = sc_device_close();
	close_fd();
}
