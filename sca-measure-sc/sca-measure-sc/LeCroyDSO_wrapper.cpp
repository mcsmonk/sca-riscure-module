
#include "sca.h"

int LeCroyDSOConnect_wrapper(const char* ip_add, FILE* fp)
{
	rv = LeCroyDSOConnect(ip_add);
	if (!rv) {
		fprintf(stderr, "LeCroyDSOConnect fail // %s ( %d line )\n", __FILE__, __LINE__);
		if (fp != NULL) {
			fprintf(fp, "LeCroyDSOConnect fail // %s ( %d line )\n", __FILE__, __LINE__);
		}
		return -1;
	}
	fprintf(stderr, "DSO Connect Success // %s ( %d line )\n", __FILE__, __LINE__);
	if (fp != NULL) {
		fprintf(fp, "DSO Connect Success // %s ( %d line )\n", __FILE__, __LINE__);
	}
	return 0;
}

int LeCroyDSODisconnect_wrapper(FILE* fp)
{
	rv = LeCroyDSODisconnect();
	if (!rv) {
		fprintf(stderr, "LeCroyDSODisconnect fail // %s ( %d line )\n", __FILE__, __LINE__);
		if (fp != NULL) {
			fprintf(fp, "LeCroyDSODisconnect fail // %s ( %d line )\n", __FILE__, __LINE__);
		}
		return -1;
	}
	fprintf(stderr, "DSO Disconnect Success // %s ( %d line )\n", __FILE__, __LINE__);
	if (fp != NULL) {
		fprintf(fp, "DSO Disconnect Success // %s ( %d line )\n", __FILE__, __LINE__);
	}
	return 0;
}
