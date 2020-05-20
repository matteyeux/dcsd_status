# dcsd_status
Switch on the LED corresponding to the Apple device status (Normal/Recovery/DFU) 

[demo](https://youtu.be/BreVZON1wdE)

### Usage 
```
usage : ./dcsd_status [OPTIONS]
 -s	stop deamon
 -n	no daemon
 -r	reset
 -h	help
```

### Dependances
- libusb
- libftdi

### Build
To build dcsd_status, run `make`. You can enable debugging mode when runnning make with `DEBUG` flag : `make DEBUG=1`

### Known issues
iDevices are not recognized on macOS High Sierra due to [libusb](https://github.com/libusb/libusb/issues/290). Check [#1](https://github.com/matteyeux/dcsd_status/issues/1). <br>
You can apply [this patch](https://github.com/matteyeux/dcsd_status/blob/master/libusb.patch) to fix libusb, or use [my fork](https://github.com/matteyeux/libusb.git).

---

If you want to have some fun with your DCSD, check this [repo](https://github.com/matteyeux/dcsd_led)
