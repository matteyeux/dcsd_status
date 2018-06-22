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
int device_mode(void) {
	static struct libusb_device_handle *device = NULL;
	static int devicemode = -1;

	libusb_init(NULL);
	
	devicemode = IPHONE_NORM_MODE;
	device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, devicemode);
	if (device != NULL){
		fprintf(stdout, "[i] state : normal\n");
		return 1;
	}

	devicemode = IPAD_NORM_MODE;
	device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, devicemode);
	if (device != NULL){
		fprintf(stdout, "[i] state : normal\n");
		return 1;
	}

	devicemode = RECV_MODE;
	device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, devicemode);
	if (device != NULL){
		fprintf(stdout, "[i] state : recovery\n");
		return 2;
	}

	devicemode = WTF_MODE;
	device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, devicemode);
	if (device != NULL){
		fprintf(stdout, "[i] state : dfu\n");
		return 3;
	}

	devicemode = DFU_MODE;
	device = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, devicemode);
	if (device != NULL){
		fprintf(stdout, "[i] state : dfu\n");
		return 3;
	}
	return 0;
}

/* function used to set leds on DCSD cable
 * normal mode : green
 * recovery mode : yellow
 * DFU/WTF mode : red
 * other : all leds are off
 * reset : all led are on
*/
int set_led(int led){
	struct ftdi_context *ftdi;
	int f;
	long int tab[5] = {0xF0, 0xF2, 0xF8, 0xF1, 0xFB};
	unsigned char buf[1];
	static int led_status = -1;

	if (led == led_status)
	{
		fprintf(stdout, "[i] state is unchanged, not doing anything\n");
		return 0;
	} else {
		led_status = led;
	}


	ftdi = ftdi_new();

	if (ftdi == 0){
		fprintf(stderr, "[e] ftdi_new failed\n");
		return -1;
	}
	/* 0x0403 : vendor ID */
	/* 0x8a88 : product ID*/
	f = ftdi_usb_open(ftdi, 0x0403, 0x8a88);

	while (f < 0){
		f = ftdi_usb_open(ftdi, 0x0403, 0x8a88);
		fprintf(stderr, "[e] unable to open ftdi device: %d (%s)\n", f, ftdi_get_error_string(ftdi));
		printf("waiting...\n");
		sleep(1);
	}

	fprintf(stdout, "[+] ftdi open succeeded: %d\n",f);

	f = ftdi_set_bitmode(ftdi, tab[led], BITMODE_CBUS);

	if (f < 0){
		fprintf(stderr, "[e] set_bitmode failed for 0x%ld, error %d (%s)\n", tab[led], f, ftdi_get_error_string(ftdi));
		ftdi_usb_close(ftdi);
		ftdi_free(ftdi);
		exit(-1);
	}
	f = ftdi_read_pins(ftdi, &buf[0]);

	if (f < 0){
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
