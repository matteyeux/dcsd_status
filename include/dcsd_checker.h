/*
	all the data is from lsusb and based on libirecovery: 
	Bus 001 Device 004: ID 05ac:12a8 Apple, Inc. iPhone5/5C/5S/6
	I tested every state of some Apple devices, but it seems normal mode
	value is not the same on iPads and iPhones.
*/
#define VENDOR_ID		(int)0x05AC
#define NORM_MODE		(int)0x12a8
#define RECV_MODE		(int)0x1281
#define WTF_MODE		(int)0x1227
#define DFU_MODE		(int)0x1222

int device_mode(void);
int set_led(int led);
