#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ftdi.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>
#include <include/dcsd_status.h>


/* function to return state of device :
 * 1 : normal mode
 * 2 : recovery mode
 * 3 : DFU/WTF mode
 * 0 : idk yet
 * TODO:
 * WTF mode may not be needed anymore as it is DFU
 * just replace DFU PID value with with WTF value
*/
int device_mode(void)
{
	libusb_device_handle *dev = NULL;
	int ret = 0;
	char serial_number[256];
	struct libusb_device_descriptor desc;

	int device_state[5] = {IPHONE_NORM_MODE, IPAD_NORM_MODE, RECV_MODE, WTF_MODE, DFU_MODE};
	static char *device_status[] = {NULL, "normal", "recovery", "dfu", "pwnd", "unknown"};

	libusb_init(NULL);

	/*
	* in this loop we check the current state
	* of the device
	*/
	for (int i = 0; i < 6; i++) {
		dev = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, device_state[i]);
		if (dev != NULL) {
			if (i == 0)
				i += 1;
			ret = i;
			fprintf(stdout, "[i] state : %s\n", device_status[i]); break;
			break;
		}
	}

	if (ret == 3) {
		libusb_device *device = NULL;

		device = libusb_get_device(dev);
		libusb_get_device_descriptor(device, &desc);
		libusb_get_string_descriptor_ascii(dev, desc.iSerialNumber,
                                           (unsigned char *)serial_number,
                                           sizeof(serial_number));

		if (strstr(serial_number, "PWND"))
			ret = 4;
	}

	if (dev != NULL)
		libusb_release_interface(dev, 0);
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
	int ret = 0;
	long int tab[6] = {0xF0, 0xF2, 0xF8, 0xF1, 0xF3, 0xFB};
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
		ret = -1;
		goto end;
	}

	f = ftdi_read_pins(ftdi, &buf[0]);

	if (f < 0) {
		fprintf(stderr, "[e] read_pins failed, error %d (%s)\n", f, ftdi_get_error_string(ftdi));
		ret = -1;
		goto end;
	}

	printf("0x%01x\n", buf[0] & 0x0f);

end:
	ftdi_usb_close(ftdi);
	ftdi_free(ftdi);

	return ret;
}
