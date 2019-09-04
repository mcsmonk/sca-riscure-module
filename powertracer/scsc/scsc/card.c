
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "card.h"

extern const unsigned char command_selectdf[];
extern const unsigned int command_len_selectdf;

BOOL card_flag_power = FALSE;
BOOL card_flag_inserted = FALSE;

double v, vmin, vmax, vstep;
double f, fmin, fmax, fstep;

unsigned char card_buf_inner[1024];
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

void card_print_RESPONSE(unsigned char buf[], unsigned int buf_len)
{
	unsigned int i;
	printf("RESPONSE << ");
	for (i = 0; i < buf_len - 2; i++) {
		printf("%02X", buf[i]);
	}
	printf(" %02X%02X\n", buf[i], buf[i + 1]);
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

	card_ret_ptd = pt_set_smartcard_voltage(&card_ptd, 5.0);
	CARD_PRINT_FN_RET(pt_get_smartcard_voltage, card_ret_ptd);
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

	printf(" * E N D\n\n");
}

void card_pt_device_select_DF()
{
	memset(card_buf_inner, 0, sizeof(card_buf_inner));
	card_ret_ptd = pt_read(&card_ptd, card_buf_inner, sizeof(card_buf_inner), &card_command_len_response);
	CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		CARD_PRINT_ULONG(card_command_len_response);
		if (card_command_len_response) {
			card_print_RESPONSE(card_buf_inner, card_command_len_response);
		}
	}

	card_ret_ptd = pt_write(&card_ptd, command_selectdf, command_len_selectdf);
	CARD_PRINT_FN_RET(pt_write, card_ret_ptd);
	card_print_APDU_Lc(command_selectdf, command_len_selectdf);

	memset(card_buf_inner, 0, sizeof(card_buf_inner));
	card_ret_ptd = pt_read(&card_ptd, card_buf_inner, sizeof(card_buf_inner), &card_command_len_response);
	CARD_PRINT_FN_RET(pt_read, card_ret_ptd);
	if (card_ret_ptd == PT_OK) {
		CARD_PRINT_ULONG(card_command_len_response);
		if (card_command_len_response) {
			card_print_RESPONSE(card_buf_inner, card_command_len_response);
		}
	}
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