CC=gcc
LD=gcc
CFLAGS=-Wall -pedantic -std=c99 -Wno-format -g
LDFLAGS=
LIBS=-lX11
OBJS=soundkeys.o


.PHONY: all clean

all: soundkeys

.c.o:
	$(CC) $(CFLAGS) -c $<

soundkeys: $(OBJS)
	$(LD) $(LDFLAGS) $(LIBS) $(OBJS) -o soundkeys

clean:
	rm -f $(OBJS) soundkeys
