
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "powertracer_api.h"
#include "card.h"

#define _CRT_SECURE_NO_WARNINGS


int main(int argc, char* argv[])
{
	card_pt_device_open();
	card_pt_device_print_info();
	card_pt_device_power_on();
	card_pt_device_coldreset();
	card_pt_device_print_channel();

	card_pt_device_warmreset();
	card_pt_device_select_DF();

	card_pt_device_power_off();
	card_pt_device_close();

	return 0;
}