
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "card.h"

BOOL card_flag_power = FALSE;
BOOL card_flag_inserted = FALSE;
BOOL card_flag_rx_err_check = FALSE;
BOOL card_flag_tx_err_check = FALSE;
unsigned int err_index = 0;

double v, vmin, vmax, vstep;
double f, fmin, fmax, fstep;

unsigned char card_buf_inner[1023];
unsigned char card_buf_inner2[1023];
unsigned int card_command_len_response;

void card_print_APDU_Lc(unsigned char buf[], unsigned int buf_len)
{
	int i, j;
	printf("APDU >> ");
	for (i = 0; i < 5; i++) {
		printf("%02X ", buf[i]);
		if (i == 4 && buf[4]) {
			printf("- ");
			for (j = 1; j <= buf[4]; j++) {
				printf("%02X", buf[i + j]);
			}
		}
	}
	printf("\n");
}

void card_print_APDU_Le(unsigned char buf[], unsigned int buf_len)
{
	int i;
	printf("APDU >> ");
	for (i = 0; i < 5; i++) {
		printf("%02X ", buf[i]);
	}
	printf("\n");
}

void card_print_RESPONSE(unsigned char buf[], int buf_len)
{
	int i;
	printf("RESPONSE << ");
	if (buf_len > 2) {
		for (i = 0; i < buf_len - 2; i++) {
			printf("%02X", buf[i]);
		}
		printf(" ");
		printf("%02X%02X\n", buf[buf_len - 2], buf[buf_len - 1]);
	}
	else if (buf_len == 2) {
		printf("%02X%02X\n", buf[buf_len - 2], buf[buf_len - 1]);
	}
	else if (buf_len) {
		for (i = 0; i < buf_len; i++) {
			printf("%02X", buf[i]);
		}
	}
}

void card_pt_device_open()
{
	unsigned int count;

	printf(" * START\n\t %s\n", __FUNCTION__);

	card_ret_ptd = pt_device_list(&count);
	CARD_PRINT_FN_RET(pt_device_list, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("%d devices are detected", count);
	}
	printf("\n");

	card_ret_ptd = pt_device_get_info(&card_ptd, 0);
	CARD_PRINT_FN_RET(pt_device_get_info, card_ret_ptd);
	printf("\n");

	card_ret_ptd = pt_open(&card_ptd);
	CARD_PRINT_FN_RET(pt_open, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("powertrace is connected");
	}
	printf("\n");

	printf(" * E N D\n\n");
}

void card_pt_device_close()
{
	printf(" * START\n\t %s\n", __FUNCTION__);

	card_ret_ptd = pt_close(&card_ptd);
	CARD_PRINT_FN_RET(pt_close, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("powertrace is closed");
	}
	printf("\n");

	printf(" * E N D\n\n");
}

void card_pt_device_print_info()
{
	printf(" * START\n\t %s\n", __FUNCTION__);

	printf("\t\tpt_sdk_get_version : %s\n", pt_sdk_get_version());
	printf("\t\tpt_sdk_get_build_time : %s\n", pt_sdk_get_build_time());

	card_ret_ptd = pt_get_version(&card_ptd, card_buf_inner, sizeof(card_buf_inner));
	CARD_PRINT_FN_RET(pt_get_version, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("powertracer version : %s", card_buf_inner);
	}
	printf("\n");

	CARD_PRINT_STRING(card_ptd.serialNumber);
	CARD_PRINT_STRING(card_ptd.description);
	CARD_PRINT_UCHAR(card_ptd.version.bitstream_version);
	CARD_PRINT_UCHAR(card_ptd.version.firmware_version);
	CARD_PRINT_ULONG(card_ptd.locationId);

	v = 5.0;
	card_ret_ptd = pt_set_smartcard_voltage(&card_ptd, v);
	CARD_PRINT_FN_RET(pt_set_smartcard_voltage, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("voltage : %f v", v);
	}
	printf("\r\n");

	card_ret_ptd = pt_get_smartcard_voltage(&card_ptd, &v);
	CARD_PRINT_FN_RET(pt_get_smartcard_voltage, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("voltage : %f v", v);
	}
	printf("\n");
	

	card_ret_ptd = pt_get_smartcard_voltage_boundary(&card_ptd, &vmin, &vmax);
	printf("\t\t"); CARD_PRINT_FN_RET(pt_get_smartcard_voltage_boundary, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("voltage range : %f ~ %f v", vmin, vmax);
	}
	printf("\n");

	card_ret_ptd = pt_get_smartcard_voltage_stepsize(&card_ptd, &vstep);
	CARD_PRINT_FN_RET(pt_get_smartcard_voltage_stepsize, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("voltage stepsize : %f v", vstep);
	}
	printf("\n");

	card_ret_ptd = pt_get_smartcard_frequency(&card_ptd, &f);
	CARD_PRINT_FN_RET(pt_get_smartcard_frequency, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("frequency : %f MHz", f);
	}
	printf("\n");

	card_ret_ptd = pt_get_smartcard_frequency_boundary(&card_ptd, &fmin, &fmax);
	CARD_PRINT_FN_RET(pt_get_smartcard_frequency_boundary, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("frequency range : %f ~ %f MHz", fmin, fmax);
	}
	printf("\n");

	card_ret_ptd = pt_get_smartcard_frequency_stepsize(&card_ptd, &fstep);
	CARD_PRINT_FN_RET(pt_get_smartcard_frequency_stepsize, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("frequency stepsize : %f MHz", fstep);
	}
	printf("\n");


	card_ret_ptd = pt_is_smartcard_inserted(&card_ptd, &card_flag_inserted);
	CARD_PRINT_FN_RET(pt_is_smartcard_inserted, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("card is %s", card_flag_inserted ? "detected" : "not detected");
	}
	printf("\n");

	printf(" * E N D\n\n");
}

void card_pt_device_power_on()
{
	printf(" * START\n\t %s\n", __FUNCTION__);

	card_ret_ptd = pt_is_smartcard_powered(&card_ptd, &card_flag_power);
	CARD_PRINT_FN_RET(pt_is_smartcard_powered, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] sc power %s", card_flag_power ? "on" : "off");
	}
	printf("\n");

	card_ret_ptd = pt_smartcard_powerup(&card_ptd);
	CARD_PRINT_FN_RET(pt_smartcard_powerup, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("now sc power on");
	}
	printf("\n");

	card_ret_ptd = pt_is_smartcard_powered(&card_ptd, &card_flag_power);
	CARD_PRINT_FN_RET(pt_is_smartcard_powered, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] sc power %s", card_flag_power ? "on" : "off");
	}
	printf("\n");

	if (card_ret_ptd == PT_OK) {
		Sleep(200);

		memset(card_buf_inner, 0, sizeof(card_buf_inner));
		card_ret_ptd = pt_read(&card_ptd, card_buf_inner, 0, &card_command_len_response);
		card_ret_ptd_rx = pt_get_rx_error_status(&card_ptd, &card_flag_rx_err_check);
		if (card_flag_rx_err_check) {
			CARD_PRINT_FN_RET(pt_get_rx_error_status, card_ret_ptd_rx);
			printf("\n");
		}
		CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
		if (card_ret_ptd == PT_OK) {
			CARD_PRINT_ULONG(card_command_len_response);
			if (card_command_len_response) {
				card_print_RESPONSE(card_buf_inner, card_command_len_response);
			}
		}
	}

	printf(" * E N D\n\n");
}

void card_pt_device_power_off()
{
	printf(" * START\n\t %s\n", __FUNCTION__);

	card_ret_ptd = pt_is_smartcard_powered(&card_ptd, &card_flag_power);
	CARD_PRINT_FN_RET(pt_is_smartcard_powered, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] sc power %s", card_flag_power ? "on" : "off");
	}
	printf("\n");

	card_ret_ptd = pt_smartcard_powerdown(&card_ptd);
	CARD_PRINT_FN_RET(pt_smartcard_powerdown, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("now sc power off");
	}
	printf("\n");

	card_ret_ptd = pt_is_smartcard_powered(&card_ptd, &card_flag_power);
	CARD_PRINT_FN_RET(pt_is_smartcard_powered, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] sc power %s", card_flag_power ? "on" : "off");
	}
	printf("\n");

	printf(" * E N D\n\n");
}

void card_pt_device_coldreset()
{
	printf(" * START\n\t %s\n", __FUNCTION__);

	card_ret_ptd = pt_smartcard_cold_reset(&card_ptd);
	CARD_PRINT_FN_RET(pt_smartcard_cold_reset, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("now cold reset");
	}
	printf("\n");

	printf(" * E N D\n\n");
}

void card_pt_device_warmreset()
{
	printf(" * START\n\t %s\n", __FUNCTION__);

	card_ret_ptd = pt_smartcard_warm_reset(&card_ptd);
	CARD_PRINT_FN_RET(pt_smartcard_warm_reset, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("now warm reset");
	}
	printf("\n");

	card_ret_ptd = pt_set_read_timeout(&card_ptd, 1000);
	CARD_PRINT_FN_RET(pt_set_read_timeout, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] read timeout : %d", 1000);
	}
	printf("\n");

	//card_ret_ptd = pt_set_write_timeout(&card_ptd, 2500);
	//CARD_PRINT_FN_RET(pt_set_write_timeout, card_ret_ptd);
	//if (card_ret_ptd == PT_OK) {
	//	printf("[status] read timeout : %d", 2000);
	//}
	//printf("\n");

	Sleep(200);

	if (card_ret_ptd == PT_OK) {
		memset(card_buf_inner, 0, sizeof(card_buf_inner));
		//card_ret_ptd = pt_read(&card_ptd, card_buf_inner, sizeof(card_buf_inner), &card_command_len_response);
		card_ret_ptd = pt_read(&card_ptd, card_buf_inner, 0, &card_command_len_response);
		card_ret_ptd_rx = pt_get_rx_error_status(&card_ptd, &card_flag_rx_err_check);
		if (card_flag_rx_err_check) {
			CARD_PRINT_FN_RET(pt_get_rx_error_status, card_ret_ptd_rx);
			printf("\n");
		}
		CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
		if (card_ret_ptd == PT_OK) {
			CARD_PRINT_ULONG(card_command_len_response);
			if (card_command_len_response) {
				card_print_RESPONSE(card_buf_inner, card_command_len_response);
			}
		}
	}
	printf("\n");

	printf(" * E N D\n\n");
}

void card_pt_device_print_channel()
{
	pt_com_channel comchannel;

	printf(" * START\n\t %s\n", __FUNCTION__);

	card_ret_ptd = pt_get_communication_channel(&card_ptd, &comchannel);
	CARD_PRINT_FN_RET(pt_set_communication_channel, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] communicate with %s channel", comchannel == SMARTCARD ? "SMARTCARD" : "EXT_SERIAL");
	}
	printf("\n");

	card_ret_ptd = pt_is_rx_error_signaling_enabled(&card_ptd, &card_flag_rx_err_check);
	CARD_PRINT_FN_RET(pt_is_rx_error_signaling_enabled, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] pt_is_rx_error_signaling_enabled : %s", card_flag_rx_err_check ? "True" : "False");
	}
	printf("\n");

	card_ret_ptd = pt_enable_rx_error_signaling(&card_ptd, TRUE);
	CARD_PRINT_FN_RET(pt_enable_rx_error_signaling, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] pt_enable_rx_error_signaling : True");
	}
	printf("\n");

	card_ret_ptd = pt_is_rx_error_signaling_enabled(&card_ptd, &card_flag_rx_err_check);
	CARD_PRINT_FN_RET(pt_is_rx_error_signaling_enabled, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] pt_is_rx_error_signaling_enabled : %s", card_flag_rx_err_check ? "True" : "False");
	}
	printf("\n");

	card_ret_ptd = pt_is_tx_error_checking_enabled(&card_ptd, &card_flag_tx_err_check);
	CARD_PRINT_FN_RET(pt_is_tx_error_checking_enabled, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] pt_is_tx_error_checking_enabled : %s", card_flag_tx_err_check ? "True" : "False");
	}
	printf("\n");

	card_ret_ptd = pt_enable_tx_error_checking(&card_ptd, TRUE);
	CARD_PRINT_FN_RET(pt_enable_tx_error_checking, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] pt_enable_tx_error_checking : True");
	}
	printf("\n");

	card_ret_ptd = pt_is_tx_error_checking_enabled(&card_ptd, &card_flag_tx_err_check);
	CARD_PRINT_FN_RET(pt_is_tx_error_checking_enabled, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] pt_is_tx_error_checking_enabled : %s", card_flag_tx_err_check ? "True" : "False");
	}
	printf("\n");

	card_ret_ptd = pt_set_tx_error_handling(&card_ptd, HALT);
	CARD_PRINT_FN_RET(pt_set_tx_error_handling, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		printf("[status] pt_set_tx_error_handling : HALT");
	}
	printf("\n");

	printf(" * E N D\n\n");
}

void card_pt_device_select_DF()
{
	memset(card_buf_inner, 0, sizeof(card_buf_inner));
	card_ret_ptd = pt_read(&card_ptd, card_buf_inner, sizeof(card_buf_inner), &card_command_len_response);
	//card_ret_ptd = pt_read(&card_ptd, card_buf_inner, 0, &card_command_len_response);
	card_ret_ptd_rx = pt_get_rx_error_status(&card_ptd, &card_flag_rx_err_check);
	if (card_flag_rx_err_check) {
		CARD_PRINT_FN_RET(pt_get_rx_error_status, card_ret_ptd_rx);
		printf("\n");
	}
	CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		CARD_PRINT_ULONG(card_command_len_response);
		if (card_command_len_response) {
			card_print_RESPONSE(card_buf_inner, card_command_len_response);
		}
	}
	printf("\n");

	memset(card_buf_inner, 0, sizeof(card_buf_inner));
	card_ret_ptd = pt_read(&card_ptd, card_buf_inner, sizeof(card_buf_inner), &card_command_len_response);
	//card_ret_ptd = pt_read(&card_ptd, card_buf_inner, 0, &card_command_len_response);
	card_ret_ptd_rx = pt_get_rx_error_status(&card_ptd, &card_flag_rx_err_check);
	if (card_flag_rx_err_check) {
		CARD_PRINT_FN_RET(pt_get_rx_error_status, card_ret_ptd_rx);
		printf("\n");
	}
	CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		CARD_PRINT_ULONG(card_command_len_response);
		if (card_command_len_response) {
			card_print_RESPONSE(card_buf_inner, card_command_len_response);
		}
	}
	printf("\n");

	memset(card_buf_inner, 0, sizeof(card_buf_inner));
	card_ret_ptd = pt_read(&card_ptd, card_buf_inner, sizeof(card_buf_inner), &card_command_len_response);
	//card_ret_ptd = pt_read(&card_ptd, card_buf_inner, 0, &card_command_len_response);
	card_ret_ptd_rx = pt_get_rx_error_status(&card_ptd, &card_flag_rx_err_check);
	if (card_flag_rx_err_check) {
		CARD_PRINT_FN_RET(pt_get_rx_error_status, card_ret_ptd_rx);
		printf("\n");
	}
	CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		CARD_PRINT_ULONG(card_command_len_response);
		if (card_command_len_response) {
			card_print_RESPONSE(card_buf_inner, card_command_len_response);
		}
	}
	printf("\n");

	card_ret_ptd = pt_write(&card_ptd, command_selectdf, command_len_selectdf);
	card_ret_ptd_tx = pt_get_tx_error_status(&card_ptd, &card_flag_tx_err_check, &err_index);
	if (card_flag_tx_err_check) {
		CARD_PRINT_FN_RET(pt_get_tx_error_status, card_ret_ptd_tx);
		printf("err_index : %04X", err_index);
		printf("\n");
	}
	CARD_PRINT_FN_RET(pt_write, card_ret_ptd);
	card_print_APDU_Lc(command_selectdf, command_len_selectdf);
	printf("\n");

	Sleep(500); // shsh

	memset(card_buf_inner, 0, sizeof(card_buf_inner));
	card_ret_ptd = pt_read(&card_ptd, card_buf_inner, sizeof(card_buf_inner), &card_command_len_response);
	//card_ret_ptd = pt_read(&card_ptd, card_buf_inner, 0, &card_command_len_response);
	card_ret_ptd_rx = pt_get_rx_error_status(&card_ptd, &card_flag_rx_err_check);
	if (card_flag_rx_err_check) {
		CARD_PRINT_FN_RET(pt_get_rx_error_status, card_ret_ptd_rx);
		printf("\n");
	}
	CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		CARD_PRINT_ULONG(card_command_len_response);
		if (card_command_len_response) {
			card_print_RESPONSE(card_buf_inner, card_command_len_response);
		}
	}
	printf("\n");

	memcpy(card_buf_inner2, card_buf_inner, card_command_len_response);
	memcpy(card_buf_inner, command_getresponse, command_len_getresponse);
	card_buf_inner[4] = card_buf_inner2[card_command_len_response - 1];
	card_ret_ptd = pt_write(&card_ptd, card_buf_inner, command_len_getresponse + 1);
	card_ret_ptd_tx = pt_get_tx_error_status(&card_ptd, &card_flag_tx_err_check, &err_index);
	if (card_flag_tx_err_check) {
		CARD_PRINT_FN_RET(pt_get_tx_error_status, card_ret_ptd_tx);
		printf("err_index : %04X", err_index);
		printf("\n");
	}
	CARD_PRINT_FN_RET(pt_write, card_ret_ptd);
	card_print_APDU_Le(card_buf_inner, command_len_getresponse + 1);
	printf("\n");

	Sleep(200); // shsh

	memset(card_buf_inner, 0, sizeof(card_buf_inner));
	card_ret_ptd = pt_read(&card_ptd, card_buf_inner, sizeof(card_buf_inner), &card_command_len_response);
	//card_ret_ptd = pt_read(&card_ptd, card_buf_inner, 0, &card_command_len_response);
	card_ret_ptd_rx = pt_get_rx_error_status(&card_ptd, &card_flag_rx_err_check);
	if (card_flag_rx_err_check) {
		CARD_PRINT_FN_RET(pt_get_rx_error_status, card_ret_ptd_rx);
		printf("\n");
	}
	CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		CARD_PRINT_ULONG(card_command_len_response);
		if (card_command_len_response) {
			card_print_RESPONSE(card_buf_inner, card_command_len_response);
		}
	}
	printf("\n");

	memcpy(card_buf_inner, command_getencipher, command_len_getencipher);
	card_buf_inner[4] = 32;
	for (int i = 5; i < 5 + 32; i++)
		card_buf_inner[i] = i - 5;

	pt_set_armed(&card_ptd, TRUE);
	card_ret_ptd = pt_write(&card_ptd, card_buf_inner, command_len_getencipher + 1 + 32);
	pt_set_armed(&card_ptd, FALSE);

	card_ret_ptd_tx = pt_get_tx_error_status(&card_ptd, &card_flag_tx_err_check, &err_index);
	if (card_flag_tx_err_check) {
		CARD_PRINT_FN_RET(pt_get_tx_error_status, card_ret_ptd_tx);
		printf("err_index : %04X", err_index);
		printf("\n");
	}
	CARD_PRINT_FN_RET(pt_write, card_ret_ptd);
	card_print_APDU_Lc(card_buf_inner, command_len_getencipher + 1 + 32);
	printf("\n");

	Sleep(200); // shsh

	memset(card_buf_inner, 0, sizeof(card_buf_inner));
	card_ret_ptd = pt_read(&card_ptd, card_buf_inner, sizeof(card_buf_inner), &card_command_len_response);
	//card_ret_ptd = pt_read(&card_ptd, card_buf_inner, 0, &card_command_len_response);
	card_ret_ptd_rx = pt_get_rx_error_status(&card_ptd, &card_flag_rx_err_check);
	if (card_flag_rx_err_check) {
		CARD_PRINT_FN_RET(pt_get_rx_error_status, card_ret_ptd_rx);
		printf("\n");
	}
	CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		CARD_PRINT_ULONG(card_command_len_response);
		if (card_command_len_response) {
			card_print_RESPONSE(card_buf_inner, card_command_len_response);
		}
	}
	printf("\n");

	if (card_command_len_response >= 2) {
		memcpy(card_buf_inner2, card_buf_inner, card_command_len_response);
		memcpy(card_buf_inner, command_getresponse, command_len_getresponse);
		card_buf_inner[4] = card_buf_inner2[card_command_len_response - 1];
		card_ret_ptd = pt_write(&card_ptd, card_buf_inner, command_len_getresponse + 1);
		card_ret_ptd_tx = pt_get_tx_error_status(&card_ptd, &card_flag_tx_err_check, &err_index);
		if (card_flag_tx_err_check) {
			CARD_PRINT_FN_RET(pt_get_tx_error_status, card_ret_ptd_tx);
			printf("err_index : %04X", err_index);
			printf("\n");
		}
		CARD_PRINT_FN_RET(pt_write, card_ret_ptd);
		card_print_APDU_Le(card_buf_inner, command_len_getresponse + 1);
		printf("\n");
	}

	Sleep(200); // shsh

	memset(card_buf_inner, 0, sizeof(card_buf_inner));
	card_ret_ptd = pt_read(&card_ptd, card_buf_inner, sizeof(card_buf_inner), &card_command_len_response);
	//card_ret_ptd = pt_read(&card_ptd, card_buf_inner, 0, &card_command_len_response);
	card_ret_ptd_rx = pt_get_rx_error_status(&card_ptd, &card_flag_rx_err_check);
	if (card_flag_rx_err_check) {
		CARD_PRINT_FN_RET(pt_get_rx_error_status, card_ret_ptd_rx);
		printf("\n");
	}
	CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		CARD_PRINT_ULONG(card_command_len_response);
		if (card_command_len_response) {
			card_print_RESPONSE(card_buf_inner, card_command_len_response);
		}
	}
	printf("\n");

	memset(card_buf_inner, 0, sizeof(card_buf_inner));
	card_ret_ptd = pt_read(&card_ptd, card_buf_inner, sizeof(card_buf_inner), &card_command_len_response);
	//card_ret_ptd = pt_read(&card_ptd, card_buf_inner, 0, &card_command_len_response);
	card_ret_ptd_rx = pt_get_rx_error_status(&card_ptd, &card_flag_rx_err_check);
	if (card_flag_rx_err_check) {
		CARD_PRINT_FN_RET(pt_get_rx_error_status, card_ret_ptd_rx);
		printf("\n");
	}
	CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		CARD_PRINT_ULONG(card_command_len_response);
		if (card_command_len_response) {
			card_print_RESPONSE(card_buf_inner, card_command_len_response);
		}
	}
	printf("\n");
}

void card_pt_device_()
{
	;
}


//card_ret_ptd = pt_set_communication_channel(&card_ptd, EXT_SERIAL);
//CARD_PRINT_FN_RET(pt_set_communication_channel, card_ret_ptd);
//if (card_ret_ptd == PT_OK) {
//	printf("communicate with EXT_SERIAL channel");
//}
//printf("\n");

////pt_set_armed
////pt_is_armed
////pt_set_trigger_delay
////pt_set_trigger_delay
////pt_set_dcdc_off_delay
////pt_set_dcdc_off_duration
////pt_get_dcdc_off_delay
////pt_get_dcdc_off_duration
////pt_set_clock_off_delay
////pt_set_clock_off_duration
////pt_get_clock_off_delay
////pt_get_clock_off_duration