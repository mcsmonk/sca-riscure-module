#pragma once

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define DSO_BUF_LEN_MAX		100000000

//////////////////////////////////////////////////////////////

#define SCDEVICENUM			0

#define TRACE_NUM			10
#define TRACE_LEN_MAX		DSO_BUF_LEN_MAX

#define DSO_CHANNEL			"C2"
#define DSO_IPADDR			"163.152.90.101"

// FILE_DIR must be existed !!
#define FILE_DIR			".\\trace"

#define FILE_PREFIX			"sca-sh-"
#define FILE_PREFIX_PT		FILE_PREFIX##"plaintext"
#define FILE_PREFIX_CT		FILE_PREFIX##"ciphertext"
#define FILE_PREFIX_LOG		FILE_PREFIX##"log"
#define FILE_PREFIX_TR		FILE_PREFIX##"trace"

#define TIMEOUT				200
#define TIMEOUT_RXTX		200
#define TIMEOUT_ATR			200

//////////////////////////////////////////////////////////////

#endif //__CONFIG_H__