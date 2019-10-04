#pragma once

#ifndef __SCA_H__
#define __SCA_H__

#include "LeCroyDSO.h"
#include "config.h"
#include "smartcard.h"

extern FILE* fd_key;
extern FILE* fd_pt;
extern FILE* fd_ct;
extern FILE* fd_log;
extern FILE* fd_tr;

extern char buf_fd_key[];
extern char buf_fd_pt[];
extern char buf_fd_ct[];
extern char buf_fd_log[];
extern char buf_fd_tr[];

extern int rv;

void sca_fprintf(FILE* fp, const char* format, ...);

int open_fd();
void close_fd();

int LeCroyDSOConnect_wrapper(const char* ip_add);
int LeCroyDSODisconnect_wrapper();

void test();

#endif //__SCA_H__