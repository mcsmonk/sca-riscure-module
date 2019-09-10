
#ifndef __LECROYDSO_H__
#define __LECROYDSO_H__

extern char ProjectPath[256];

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxctl.h>         // MFC support for ActiveX Controls

// Delete the two includes below if you do not wish to use the MFC
//#include <afxdb.h>			// MFC database classes
//#include <afxdao.h>			// MFC DAO database classes
#include <afxtempl.h>		// Support for CArray template, etc.

/////////////////////////////////////////////////////////////////////////////
// Disital Storage Oscilloscope API for LeCroy
int   LeCroyDSOConnect(const char *ip_add);
int   LeCroyDSODisconnect();
int   LeCroyDSOIsConnect();
char* LeCroyDSOReceiveAllChannel(const char * Command);
char* LeCroyDSOReceiveOneChennel(const char * Command, const char * Channel);
void  LeCroyDSOSendAllChannel(const char *Command, const char *param_value);
void  LeCroyDSOSendOneChannel(const char *Command, const char *param_value, const char *Channel);
//void  LeCroyDSOStoreWaveformAllPoints(char *Channel, char *WaveFileInfo);
void LeCroyDSOStoreWaveformAllPoints(char *Channel, char *buf, int *buflen);
void  LeCroyDSOStoreWaveformSelectivePoints(char *Channel, char *WaveFileInfo, unsigned int start, unsigned int end);



#endif
