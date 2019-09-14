
#include <stdlib.h>

#include "smartcard.h"

pt_device sc_ptd;

int sc_ret;
int sc_ret_rx;
int sc_ret_tx;

BOOL sc_flag_inserted = FALSE;

double sc_v = 5.0, sc_vmin, sc_vmax, sc_vstep;
double sc_f = 1.0, sc_fmin, sc_fmax, sc_fstep;

unsigned char sc_buf1[BUFLEN];
unsigned char sc_buf2[BUFLEN];

int sc_device_open()
{
	unsigned int count;

	sc_ret = pt_device_list(&count);
	if (sc_ret == PT_OK) {
		sca_fprintf(fd_log, "pt_device_list detect device -> %d\n", count);
	}
	else {
		sca_fprintf(fd_log, "pt_device_list fail [0x%02X] // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		return -1;
	}

	// Assume : device #0 are used
	sc_ret = pt_device_get_info(&sc_ptd, SCDEVICENUM);
	if (sc_ret == PT_OK) {
		sca_fprintf(fd_log, "pt_device_get_info success\n");
	}
	else {
		sca_fprintf(fd_log, "pt_device_get_info fail [0x%02X] // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		return -1;
	}
	
	sc_ret = pt_open(&sc_ptd);
	if (sc_ret == PT_OK) {
		sca_fprintf(fd_log, "pt_open success\n");
		sc_ret = sc_device_print_info();
		if (sc_ret) return -1;
	}
	else {
		sca_fprintf(fd_log, "pt_open fail [0x%02X] // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		return -1;
	}

	return 0;
}

int sc_device_close()
{
	sc_ret = pt_close(&sc_ptd);

	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "pt_close fail [0x%02X] // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		return -1;
	}

	sca_fprintf(fd_log, "pt_close success\n");

	return 0;
}

int sc_device_print_info()
{
	sca_fprintf(fd_log, "==== powertracer info ===============\n");
	
	sca_fprintf(fd_log, "pt_sdk_get_build_time : %s\n", pt_sdk_get_build_time());

	sc_ret = pt_get_version(&sc_ptd, sc_buf1, sizeof(sc_buf1));
	
	if (sc_ret != PT_OK) goto err;

	sca_fprintf(fd_log, "powertracer version : %s (<-PCB v . bitstream v . firmware v)\n", sc_buf1);
	sca_fprintf(fd_log, "serial number : %s\n", sc_ptd.serialNumber);
	sca_fprintf(fd_log, "location id : %lu\n", sc_ptd.locationId);
	sca_fprintf(fd_log, "description : %s\n", sc_ptd.description);

	sc_ret = pt_set_smartcard_voltage(&sc_ptd, sc_v);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_smartcard_voltage [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "card voltage : %f v\n", sc_v);

	sc_ret = pt_get_smartcard_voltage_boundary(&sc_ptd, &sc_vmin, &sc_vmax);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_get_smartcard_voltage_boundary [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "card voltage range : %f ~ %f v\n", sc_vmin, sc_vmax);

	sc_ret = pt_get_smartcard_voltage_stepsize(&sc_ptd, &sc_vstep);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_get_smartcard_voltage_stepsize [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "card voltage stepsize : %f v\n", sc_vstep);


	sc_ret = pt_set_smartcard_frequency(&sc_ptd, sc_f, &sc_f);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_smartcard_frequency [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "card frequency : %f MHz\n", sc_f);

	sc_ret = pt_get_smartcard_frequency_boundary(&sc_ptd, &sc_fmin, &sc_fmax);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_get_smartcard_frequency_boundary [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "card frequency range : %f ~ %f MHz\n", sc_fmin, sc_fmax);

	sc_ret = pt_get_smartcard_frequency_stepsize(&sc_ptd, &sc_fstep);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_get_smartcard_frequency_stepsize [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "card frequency stepsize : %f MHz\n", sc_fstep);


	sc_ret = pt_is_smartcard_inserted(&sc_ptd, &sc_flag_inserted);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_is_smartcard_inserted [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "card is %s\n", sc_flag_inserted ? "detected" : "not detected");

	sca_fprintf(fd_log, "==== powertracer info end ===========\n");
	return 0;
err:
	sca_fprintf(fd_log, "==== powertracer info end ===========\n");
	return -1;
}

int sc_card_power_on()
{
	return 0;
err:
	return -1;
}

int sc_card_power_off()
{
	return 0;
err:
	return -1;
}

//void card_pt_device_coldreset();
//void card_pt_device_warmreset();
//void card_pt_device_print_channel();
//void card_pt_device_select_DF();
//void card_print_APDU_Lc(unsigned char buf[], unsigned int buf_len);
//void card_print_APDU_Le(unsigned char buf[], unsigned int buf_len);
//void card_print_RESPONSE(unsigned char buf[], unsigned int buf_len);