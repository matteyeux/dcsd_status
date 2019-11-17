CC = gcc
TARGET = dcsd_status

CFLAGS ?= -g -Wall -Wextra
LDFLAGS ?=
DEBUG ?=
SRC = $(wildcard src/*.c)
OBJECTS = $(SRC:.c=.o)

ifeq ($(shell uname), Linux)
	CFLAGS += -c -Wall -I. -I/usr/include/libftdi1
	LDFLAGS += -lusb-1.0 -lftdi1
endif

ifeq ($(shell uname), Darwin)
	CFLAGS += -c -Wall -I. -I/usr/local/Cellar/libftdi/1.4/include/libftdi1/ -g
	LDFLAGS += -L/usr/local/Cellar/libftdi/1.4/lib -L/usr/local/Cellar/libusb/1.0.22/lib -lftdi1 -lusb-1.0
endif


ifeq ($(DEBUG), 1)
	CFLAGS += -DDEBUG
endif

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)


src/%.o : src/%.c
	$(CC) $(CFLAGS) -o $@ $<


install: $(TARGET)
	install -v $(TARGET) /usr/local/bin/


install_service: install
	cp dcsd_status.service /etc/systemd/system/


clean : 
	rm -rf $(OBJECTS) $(TARGET)
