CC = gcc
TARGET = dcsd_status

CFLAGS ?=
LDFLAGS ?=
SRC = src

ifeq ($(shell uname), Linux)
	CFLAGS += -c -Wall -I. -I/usr/local/include/libftdi1 -g -o
	LDFLAGS += -L/usr/local/lib -lusb-1.0 -lftdi1
endif

ifeq ($(shell uname), Darwin)
	CFLAGS += -c -Wall -I. -I/usr/local/Cellar/libftdi/1.4/include/libftdi1/ -g 
	LDFLAGS += -L/usr/local/Cellar/libftdi/1.4/lib -L/usr/local/Cellar/libusb/1.0.22/lib -lftdi1 -lusb-1.0
endif

OBJECTS = src/main.o src/dcsd_status.o src/deamon.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

src/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean : 
	rm -rf $(OBJECTS) $(TARGET)
