CC      = arm-linux-gcc
AR 	 	= arm-linux-ar
OBJCOPY = arm-linux-objcopy
CFLAGS  = -O2 -ffreestanding -Wall -Werror -nostdinc -I include -g
ASFLAGS = -nostdinc -I include -g

objects = errno.o exit.o read.o write.o

default: all

.PHONY: default all clean clobber

all: crt0.o libc.a

libc.a: $(objects)
	$(AR) rcs $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.S
	$(CC) -c $(ASFLAGS) -o $@ $<

clean:
	rm -f $(objects)

clobber: clean
	rm -f crt0.o libc.a
