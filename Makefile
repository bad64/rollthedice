SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)

CC = gcc
CFLAGS = -O2

rtd: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: install
install:
	cp rtd /usr/local/bin/rtd
	chmod a+x /usr/local/bin/rtd

.PHONY: uninstall
uninstall:
	rm -rf /usr/local/bin/rtd

.PHONY: clean
clean:
	rm *.o

.PHONY: distclean
distclean:
	rm *.o rollthedice
