
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
	if (ret) goto err;

	ret = sc_write2(&sc_ptd, (unsigned char*) APDU_selectDF, APDU_selectDF[4] + 5);
	if (ret) goto err;

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, sc_buf1, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, sc_buf1, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, sc_buf1, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	//ret = sc_write1(&sc_ptd, (unsigned char*)APDU_getResponse, sc_buf1[2], NULL);
	//if (ret) goto err;

	ret = sc_write1(&sc_ptd, (unsigned char*)APDU_getResponse, 0x0A, NULL);
	if (ret) goto err;

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, sc_buf1, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	Sleep(TIMEOUT);

	ret = sc_read(&sc_ptd, sc_buf1, 0, &len_sc_buf1);
	if (ret) goto err;
	sc_card_print_RESPONSE(sc_buf1, len_sc_buf1);

	ret = sc_card_power_off();
	if (ret) goto err;

err:
	ret = sc_device_close();
	close_fd();
}
