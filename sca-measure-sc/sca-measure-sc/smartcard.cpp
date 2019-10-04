
#include <stdlib.h>

#include "smartcard.h"

pt_device sc_ptd;

int sc_ret;
int sc_ret_rx;
int sc_ret_tx;

BOOL sc_flag_inserted = FALSE;
BOOL sc_flag_poweron = FALSE;
BOOL sc_flag_rx_error = FALSE;
BOOL sc_flag_tx_error = FALSE;
unsigned int sc_tx_error_index = FALSE;

double sc_v = 5, sc_vmin, sc_vmax, sc_vstep;
double sc_f = 4, sc_fmin, sc_fmax, sc_fstep;

unsigned char sc_buf1[BUFLEN];
unsigned int len_sc_buf1;
unsigned char sc_buf2[BUFLEN];
unsigned int len_sc_buf2;

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

		sc_ret = sc_card_setting();
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

int sc_card_setting()
{
	sc_ret = pt_set_f_d(&sc_ptd, 372, 1);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_f_d [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] F/D to 372\n");

	sc_ret = pt_set_cgt(&sc_ptd, 12);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_cgt [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] CGT to 12\n");

	sc_ret = pt_set_communication_channel(&sc_ptd, SMARTCARD);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_communication_channel [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] Select smart card transceiver as communication channel\n");

	sc_ret = pt_set_gain(&sc_ptd, 100);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_gain [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] Gain to 100%\n");

	sc_ret = pt_set_offset(&sc_ptd, 0);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_offset [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] Offset to 0.0v\n");

	sc_ret = pt_set_armed(&sc_ptd, FALSE);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_armed [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] Disarm trigger, DC/DC shutdown events\n");

	sc_ret = pt_set_trigger_delay(&sc_ptd, 0);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_trigger_delay [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] Trigger delay set to 0\n");

	sc_ret = pt_set_dcdc_off_delay(&sc_ptd, 0);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_dcdc_off_delay [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] DC/DC shutdown delay set to 0\n");

	sc_ret = pt_set_dcdc_off_duration(&sc_ptd, 5000000);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_dcdc_off_duration [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] DC/DC shutdown duration set to 5 second\n");

	return 0;
err:
	return -1;
}

int sc_card_setting2()
{
	sc_ret = pt_enable_tx_error_checking(&sc_ptd, TRUE);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_enable_tx_error_checking [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] Enable transmitter inspecting NACK sent from smart card\n");

	sc_ret = pt_enable_rx_error_signaling(&sc_ptd, TRUE);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_enable_rx_error_signaling [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] Enable receiver signaling NACK to smart card\n");

	sc_ret = pt_set_tx_error_handling(&sc_ptd, HALT);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_tx_error_handling [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] Abort transmission as soon as a NACK is detected\n");

	sc_ret = pt_set_read_timeout(&sc_ptd, TIMEOUT_RXTX);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_read_timeout [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] pt_set_read_timeout %d\n", TIMEOUT_RXTX);

	sc_ret = pt_set_write_timeout(&sc_ptd, TIMEOUT_RXTX);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_set_write_timeout [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[setting] pt_set_write_timeout %d\n", TIMEOUT_RXTX);

	return 0;
err:
	return -1;
}

int sc_card_power_on()
{
	sc_ret = pt_is_smartcard_powered(&sc_ptd, &sc_flag_poweron);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_is_smartcard_powered [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[status] sc power %s\n", sc_flag_poweron ? "on" : "off");

	sc_ret = pt_smartcard_powerup(&sc_ptd);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_smartcard_powerup [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "now sc power on\n");

	sc_ret = pt_is_smartcard_powered(&sc_ptd, &sc_flag_poweron);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_is_smartcard_powered [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[status] sc power %s\n", sc_flag_poweron ? "on" : "off");

	if (sc_ret == PT_OK) {
		Sleep(TIMEOUT_ATR);

		sc_read(&sc_ptd, sc_buf1, BUFLEN, 0, &len_sc_buf1);
		if (sc_ret_rx != PT_OK) {
			sca_fprintf(fd_log, "Error : pt_get_rx_error_status [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
			goto err;
		}
		if (sc_flag_rx_error == TRUE) {
			sca_fprintf(fd_log, "Error : pt_get_rx_error_status [flag = TRUE]  // %s ( %d line )\n", __FILE__, __LINE__);
			goto err;
		}
		if (sc_ret != PT_OK) {
			sca_fprintf(fd_log, "Error : pt_read [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
			goto err;
		}
		sc_card_print_ATR(sc_buf1, len_sc_buf1);

		sc_ret = sc_card_setting2();
		if (sc_ret) goto err;
	}
	return 0;
err:
	sc_card_power_off();
	return -1;
}

int sc_card_power_off()
{
	if (sc_flag_poweron == FALSE) return 0;

	sc_ret = pt_is_smartcard_powered(&sc_ptd, &sc_flag_poweron);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_is_smartcard_powered [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[status] sc power %s\n", sc_flag_poweron ? "on" : "off");

	sc_ret = pt_smartcard_powerdown(&sc_ptd);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_smartcard_powerdown [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "now sc power off\n");

	sc_ret = pt_is_smartcard_powered(&sc_ptd, &sc_flag_poweron);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_is_smartcard_powered [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[status] sc power %s\n", sc_flag_poweron ? "on" : "off");

	return 0;
err:
	return -1;
}

int sc_card_warmreset()
{
	sc_ret = pt_smartcard_warm_reset(&sc_ptd);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_smartcard_warm_reset [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "now warm reset\n");

	sc_ret = pt_is_smartcard_powered(&sc_ptd, &sc_flag_poweron);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_is_smartcard_powered [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[status] sc power %s\n", sc_flag_poweron ? "on" : "off");

	if (sc_ret == PT_OK) {
		Sleep(TIMEOUT_ATR);

		sc_read(&sc_ptd, sc_buf1, BUFLEN, 0, &len_sc_buf1);
		if (sc_ret_rx != PT_OK) {
			sca_fprintf(fd_log, "Error : pt_get_rx_error_status [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
			goto err;
		}
		if (sc_flag_rx_error == TRUE) {
			sca_fprintf(fd_log, "Error : pt_get_rx_error_status [flag = TRUE]  // %s ( %d line )\n", __FILE__, __LINE__);
			goto err;
		}
		if (sc_ret != PT_OK) {
			sca_fprintf(fd_log, "Error : pt_read [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
			goto err;
		}
		sc_card_print_ATR(sc_buf1, len_sc_buf1);

		sc_ret = sc_card_setting2();
		if (sc_ret) goto err;
	}
	return 0;
err:
	sc_card_power_off();
	return -1;
}

int sc_card_coldreset()
{
	sc_ret = pt_smartcard_cold_reset(&sc_ptd);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_smartcard_warm_reset [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "now cold reset\n");

	sc_ret = pt_is_smartcard_powered(&sc_ptd, &sc_flag_poweron);
	if (sc_ret != PT_OK) {
		sca_fprintf(fd_log, "Error : pt_is_smartcard_powered [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
		goto err;
	}
	sca_fprintf(fd_log, "[status] sc power %s\n", sc_flag_poweron ? "on" : "off");

	if (sc_ret == PT_OK) {
		Sleep(TIMEOUT_ATR);

		sc_read(&sc_ptd, sc_buf1, BUFLEN, 0, &len_sc_buf1);
		if (sc_ret_rx != PT_OK) {
			sca_fprintf(fd_log, "Error : pt_get_rx_error_status [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
			goto err;
		}
		if (sc_flag_rx_error == TRUE) {
			sca_fprintf(fd_log, "Error : pt_get_rx_error_status [flag = TRUE]  // %s ( %d line )\n", __FILE__, __LINE__);
			goto err;
		}
		if (sc_ret != PT_OK) {
			sca_fprintf(fd_log, "Error : pt_read [%02X]  // %s ( %d line )\n", sc_ret, __FILE__, __LINE__);
			goto err;
		}
		sc_card_print_ATR(sc_buf1, len_sc_buf1);

		sc_ret = sc_card_setting2();
		if (sc_ret) goto err;
	}
	return 0;
err:
	sc_card_power_off();
	return -1;
}

void sc_card_print_APDU_Lc(unsigned char buf[], int buf_len)
{
	int i;
	sca_fprintf(fd_log, "APDU >> ");

	if (buf_len < 5) {
		for (i = 0; i < buf_len; i++) {
			sca_fprintf(fd_log, "%02X ", buf[i]);
		}
		sca_fprintf(fd_log, "--> incorrect apdu !\n");
	}
	else if (buf[4] + 5 != buf_len) {
		for (i = 0; i < 5; i++) {
			sca_fprintf(fd_log, "%02X ", buf[i]);
		}
		sca_fprintf(fd_log, "- ");
		for (i = 5; i < buf_len; i++) {
			sca_fprintf(fd_log, "%02X ", buf[i]);
		}
		sca_fprintf(fd_log, "--> incorrect apdu !\n");
	}

	for (i = 0; i < 5; i++) {
		sca_fprintf(fd_log, "%02X ", buf[i]);
	}
	sca_fprintf(fd_log, "- ");
	for (i = 5; i < buf_len; i++) {
		sca_fprintf(fd_log, "%02X ", buf[i]);
	}
	sca_fprintf(fd_log, "\n");
}

void sc_card_print_APDU_Le(unsigned char buf[], int buf_len)
{
	int i;
	sca_fprintf(fd_log, "APDU >> ");

	if (buf_len < 5) {
		for (i = 0; i < buf_len; i++) {
			sca_fprintf(fd_log, "%02X ", buf[i]);
		}
		sca_fprintf(fd_log, "--> incorrect apdu !\n");
	}

	for (i = 0; i < 5; i++) {
		sca_fprintf(fd_log, "%02X ", buf[i]);
	}
	sca_fprintf(fd_log, "\n");
}

void sc_card_print_ATR(unsigned char buf[], int buf_len)
{
	int i;
	sca_fprintf(fd_log, "ATR  << ");

	for (i = 0; i < buf_len; i++) {
		sca_fprintf(fd_log, "%02X ", buf[i]);
	}
	sca_fprintf(fd_log, "\n");
}

void sc_card_print_RESPONSE(unsigned char buf[], int buf_len)
{
	int i;
	sca_fprintf(fd_log, "RET  << ");

	if (buf_len < 3) {
		for (i = 0; i < buf_len; i++) {
			sca_fprintf(fd_log, "%02X ", buf[i]);
		}
		sca_fprintf(fd_log, "--> incorrect response !\n");
		return;
	}

	for (i = 0; i < buf_len - 2; i++) {
		sca_fprintf(fd_log, "%02X ", buf[i]);
	}
	sca_fprintf(fd_log, "- ");
	for (; i < buf_len; i++) {
		sca_fprintf(fd_log, "%02X ", buf[i]);
	}
	sca_fprintf(fd_log, "\n");
}

int sc_read(pt_device* sc_ptd, unsigned char* buf, unsigned int buflen, unsigned int response_len, unsigned int* actual_response_len)
{
	memset(buf, 0, buflen);
	sc_ret = pt_read(sc_ptd, buf, response_len, actual_response_len);
	sc_ret_rx = pt_get_rx_error_status(sc_ptd, &sc_flag_rx_error);
	sc_ret_tx = pt_get_tx_error_status(sc_ptd, &sc_flag_tx_error, &sc_tx_error_index);

	return sc_ret | sc_ret_rx | sc_ret_tx;
}

int sc_write1(pt_device* sc_ptd, unsigned char* apdu_wo_len, unsigned char apdu_len, unsigned char* apdu_data)
{
	memcpy(sc_buf2, apdu_wo_len, 4);
	sc_buf2[APDU_POS_LEN] = apdu_len;
	if (apdu_data != NULL) {
		memcpy(sc_buf2 + 5, apdu_data, apdu_len);
		sc_ret = pt_write(sc_ptd, sc_buf2, 5 + apdu_len);
	}
	else {
		sc_ret = pt_write(sc_ptd, sc_buf2, 5);
	}

	sc_ret_rx = pt_get_rx_error_status(sc_ptd, &sc_flag_rx_error);
	sc_ret_tx = pt_get_tx_error_status(sc_ptd, &sc_flag_tx_error, &sc_tx_error_index);

	if (apdu_data != NULL && 4 + apdu_len > 5) sc_card_print_APDU_Lc(sc_buf2, 5 + apdu_len);
	else sc_card_print_APDU_Le(sc_buf2, 5);

	return sc_ret | sc_ret_rx | sc_ret_tx;
}

int sc_write2(pt_device* sc_ptd, unsigned char* apdu, unsigned int len)
{
	sc_ret = pt_write(sc_ptd, apdu, len);
	sc_ret_rx = pt_get_rx_error_status(sc_ptd, &sc_flag_rx_error);
	sc_ret_tx = pt_get_tx_error_status(sc_ptd, &sc_flag_tx_error, &sc_tx_error_index);

	if (len > 5) sc_card_print_APDU_Lc(apdu, len);
	else sc_card_print_APDU_Le(apdu, len);

	return sc_ret | sc_ret_rx | sc_ret_tx;
}
