CC=gcc
CFLAGS=-l asound -Wall

alsavolcontrol: alsavolcontrol.c
	$(CC) $(CFLAGS) -o alsavolcontrol alsavolcontrol.c

install:
	cp ./alsavolcontrol /usr/local/bin/alsavolcontrol

clean:
	rm -f alsavolcontrol

uninstall:
	rm -f /usr/local/bin/alsavolcontrol