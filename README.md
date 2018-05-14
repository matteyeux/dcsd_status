# dcsd_status
Switch on the LED corresponding to the Apple device status (Normal/Recovery/DFU) 

[demo](https://youtu.be/BreVZON1wdE)

## Usage 
```
usage : ./dcsd_status [OPTIONS]
 -s	stop deamon
 -n	no daemon
 -r	reset
 -h	help
```

## Build
Here is how to build _dcsd_status_ you need install [libusb](https://github.com/libusb/libusb) and [libftdi](https://www.intra2net.com/en/developer/libftdi/index.php) : 

#### libusb
```bash
$ git clone https://github.com/libusb/libusb; cd libusb
$ ./autogen.sh
$ make && sudo make install
```

#### libftdi
```bash
$ wget https://www.intra2net.com/en/developer/libftdi/download/libftdi1-1.4.tar.bz2
$ tar jxvf libftdi1-1.4.tar.bz2
$ cd libftdi1-1.4
$ cmake . && sudo make install
```

#### dcsd_status
Run `make` <br>
You can enable debugging mode when runnning make with `DEBUG` flag : `make DEBUG=1`

## Known issues
iDevices are not recognized on macOS High Sierra due to [libusb](https://github.com/libusb/libusb/issues/290). Check [#1](https://github.com/matteyeux/dcsd_status/issues/1). <br>
You can apply [this patch](https://github.com/matteyeux/dcsd_status/blob/master/libusb.patch) to fix libusb, or use [my fork](https://github.com/matteyeux/libusb.git).
