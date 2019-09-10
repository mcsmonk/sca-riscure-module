#pragma once

#ifndef __SCA_H__
#define __SCA_H__

#include "config.h"
#include "LeCroyDSO.h"

extern FILE* fd_pt;
extern FILE* fd_ct;
extern FILE* fd_log;
extern FILE* fd_tr;

extern char buf_fd_pt[];
extern char buf_fd_ct[];
extern char buf_fd_log[];
extern char buf_fd_tr[];

extern int rv;

int open_fd();
void close_fd();

int LeCroyDSOConnect_wrapper(const char* ip_add, FILE* fp);
int LeCroyDSODisconnect_wrapper(FILE* fp);

#endif //__SCA_H__