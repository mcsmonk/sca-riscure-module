
#include "sca.h"

int LeCroyDSOConnect_wrapper(const char* ip_add)
{
	rv = LeCroyDSOConnect(ip_add);
	if (!rv) {
		sca_fprintf(fd_log, "LeCroyDSOConnect fail // %s ( %d line )\n", __FILE__, __LINE__);
		return -1;
	}
	sca_fprintf(fd_log, "DSO Connect Success // %s ( %d line )\n", __FILE__, __LINE__);
	return 0;
}

int LeCroyDSODisconnect_wrapper()
{
	rv = LeCroyDSODisconnect();
	if (!rv) {
		sca_fprintf(fd_log, "LeCroyDSODisconnect fail // %s ( %d line )\n", __FILE__, __LINE__);
		return -1;
	}
	sca_fprintf(fd_log, "DSO Disconnect Success // %s ( %d line )\n", __FILE__, __LINE__);
	return 0;
}
