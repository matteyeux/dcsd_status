#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ftdi.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>
#include <include/dcsd_status.h>

/* function to return state of device :
 * 1 : normal mode
 * 2 : recovery mode
 * 3 : DFU/WTF mode
 * 0 : idk yet
*/
int device_mode(void)
{
	struct libusb_device_handle *device = NULL;
	int ret = 0;
	int device_state[5] = {IPHONE_NORM_MODE, IPAD_NORM_MODE, RECV_MODE, WTF_MODE, DFU_MODE};
	static char *device_status[] = {NULL, "normal", "recovery", "dfu", "unknown"};

	libusb_init(NULL);

	for (int i = 0; i < 5; i++) {
		device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, device_state[i]);
		if (device != NULL) {
			if (i == 1 || i == 4)
				i += 1;
			fprintf(stdout, "[i] state : %s\n", device_status[i]); break;
			ret = i;
			goto out;
		}
	}

out:
		if (device != NULL) libusb_release_interface(device, 0);
		libusb_exit(0);
		return ret;
}

/* function used to set leds on DCSD cable
 * normal mode : green
 * recovery mode : yellow
 * DFU/WTF mode : red
 * other : all leds are off
 * reset : all led are on
*/
int set_led(int led)
{
	struct ftdi_context *ftdi;
	int f;
	long int tab[5] = {0xF0, 0xF2, 0xF8, 0xF1, 0xFB};
	unsigned char buf[1];
	static int led_status = -1;

	if (led == led_status) {
		fprintf(stdout, "[i] state is unchanged, not doing anything\n");
		return 0;
	} else {
		led_status = led;
	}


	ftdi = ftdi_new();

	if (ftdi == 0) {
		fprintf(stderr, "[e] ftdi_new failed\n");
		return -1;
	}
	/* 0x0403 : vendor ID */
	/* 0x8a88 : product ID*/
	f = ftdi_usb_open(ftdi, 0x0403, 0x8a88);

	while (f < 0) {
		f = ftdi_usb_open(ftdi, 0x0403, 0x8a88);
		fprintf(stderr, "[e] unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(ftdi));
		printf("waiting...\n");
		sleep(1);
	}

	fprintf(stdout, "[+] ftdi open succeeded: %d\n",f);

	f = ftdi_set_bitmode(ftdi, tab[led], BITMODE_CBUS);

	if (f < 0) {
		fprintf(stderr, "[e] set_bitmode failed for 0x%ld, error %d (%s)\n", tab[led], f, ftdi_get_error_string(ftdi));
		ftdi_usb_close(ftdi);
		ftdi_free(ftdi);
		exit(-1);
	}

	f = ftdi_read_pins(ftdi, &buf[0]);

	if (f < 0) {
		fprintf(stderr, "[e] read_pins failed, error %d (%s)\n", f, ftdi_get_error_string(ftdi));
		ftdi_usb_close(ftdi);
		ftdi_free(ftdi);
		exit(-1);
	}

	printf("0x%01x\n", buf[0] & 0x0f);

	//printf("disabling bitbang mode\n");
	//ftdi_disable_bitbang(ftdi);

	ftdi_usb_close(ftdi);
	ftdi_free(ftdi);

	return 0;
}
