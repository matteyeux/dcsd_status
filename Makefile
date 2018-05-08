CC = gcc
TARGET = dcsd_status

CFLAGS = -c -Wall -I. -I/usr/local/include/libftdi1 -g -o
LDFLAGS = -L/usr/local/lib -lusb-1.0 -lftdi1
SRC = src

OBJECTS = src/main.o src/dcsd_status.o src/deamon.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

src/%.o : src/%.c
	$(CC) $(CFLAGS) $@ $<

clean : 
	rm -rf $(OBJECTS) $(TARGET)
