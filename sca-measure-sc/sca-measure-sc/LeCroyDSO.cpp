
#include <afx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <direct.h>
#include <time.h>
#include "LeCroyDSO.h"

//#import "C://Program Files//LeCroy//ActiveDSO//ActiveDSO.ocx"
#import "C:\\Program Files\\LeCroy\\ActiveDSO\\ActiveDSO.ocx" 

using namespace ACTIVEDSOLib;

int DSO_Connect_Flag	= FALSE;
int MAX_POINTS			= 100000000;
_DActiveDSOPtr activeDSO;  

char ProjectPath[256]="";

int LeCroyDSOConnect(const char * ip_add) 
{
	// TODO: Add your control notification handler code here
	char OC_DsoAddr[256];
	int ret=0;

	// Get IP Address
	sprintf(OC_DsoAddr,"IP:%s", ip_add);
	
	// Connect
	if(DSO_Connect_Flag == FALSE)
	{
	   // Initialize OLE 2.0 libraries
        if (S_OK != OleInitialize(NULL))
            return FALSE;

	    // create a smart pointer for ActiveDSO
        HRESULT hr = activeDSO.GetActiveObject(__uuidof(ActiveDSO));
        if (FAILED(hr))
        {
            hr = activeDSO.CreateInstance(__uuidof(ActiveDSO));
            if (FAILED(hr)) 
                _com_issue_error(hr);
        }
	}

	// success : TRUE, fail : FALSE;
	ret = activeDSO->MakeConnection(OC_DsoAddr);
 	
	if(ret == 0)	DSO_Connect_Flag = FALSE;
	else			DSO_Connect_Flag = TRUE;

	return DSO_Connect_Flag;
}

int LeCroyDSODisconnect() 
{
	if(DSO_Connect_Flag == TRUE)
	{
		activeDSO->Disconnect();
		DSO_Connect_Flag = FALSE;
	}

	return DSO_Connect_Flag;
}

int LeCroyDSOIsConnect()
{
	// Connect    : true
	// Disconnect : false
	return DSO_Connect_Flag;
}

char* LeCroyDSOReceiveAllChannel(const char * Command)
{
	char sendparam[256];
	static _bstr_t Result;

	sprintf(sendparam,"%s?", Command);
	activeDSO->WriteString(sendparam, 1); 
	Result = activeDSO->ReadString(50);

	return Result;
}

char* LeCroyDSOReceiveOneChennel(const char * Command, const char * Channel)
{
	char sendparam[256];
	static _bstr_t Result;

	sprintf(sendparam,"%s:%s?", Channel, Command);
	activeDSO->WriteString(sendparam, 1); 
	Result = activeDSO->ReadString(50);

	return Result;
}

void LeCroyDSOSendAllChannel(const char *Command, const char *param_value)
{
	char sendparam[256];

	sprintf(sendparam,"%s %s", Command, param_value);
	activeDSO->WriteString(sendparam, 1); 
}

void LeCroyDSOSendOneChannel(const char *Command, const char *param_value, const char *Channel)
{
	char sendparam[256];

	sprintf(sendparam,"%s:%s %s", Channel, Command, param_value);
	activeDSO->WriteString(sendparam, 1); 
}

void LeCroyDSOStoreWaveformAllPoints(char *Channel, char *buf, int *buflen)
{
	COleVariant  waveform;
	long UpperBound = 0;

	waveform.Attach(activeDSO->GetByteWaveform(Channel, MAX_POINTS, 0));
	SafeArrayGetUBound(waveform.parray, 1, &UpperBound);
	*buflen = 0;
	if( UpperBound > 0 ) {
		*buflen = waveform.parray->rgsabound[0].cElements;
		memcpy(buf, (char*)(waveform.parray->pvData), waveform.parray->rgsabound[0].cElements);
	}
}

void LeCroyDSOStoreWaveformSelectivePoints(char *Channel, char *WaveFileInfo, unsigned int start, unsigned int end)
{
	COleVariant  waveform;
	FILE *fp;
	long UpperBound = 0;

	fp = fopen(WaveFileInfo, "wb");
	waveform.Attach(activeDSO->GetByteWaveform(Channel, MAX_POINTS, 1));
	SafeArrayGetUBound(waveform.parray, 1, &UpperBound);
	if( (UpperBound > 0) && (UpperBound <= waveform.parray->rgsabound[0].cElements) )
	{
		fwrite((char*)(waveform.parray->pvData) + start, end-start, 1, fp);
		fclose(fp);
	}
}
