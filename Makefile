CC = gcc
LDFLAGS = -Wall

rtd: lib
	$(CC) $(LDFLAGS) -c -o bin/linux/rtd.o main.c
	$(CC) $(LDFLAGS) -Wl,-rpath=/usr/local/lib/ -o bin/linux/rtd bin/linux/rtd.o -Lbin/linux -lrtd

.PHONY: lib
lib:
	@mkdir -p bin/linux
	$(CC) $(LDFLAGS) -fPIC -c -o lib/librtd.o lib/rtd.c
	$(CC) $(LDFLAGS) -s -shared -Wl,-soname,librtd.so -o bin/linux/librtd.so lib/librtd.o

.PHONY: install
install:
	@echo "Copying executable to /usr/local/bin/rtd"
	@mv bin/linux/rtd /usr/local/bin/rtd
	@chmod a+x /usr/local/bin/rtd
	@echo "Copying shared object to /usr/local/lib/librtd.so"
	@mv bin/linux/librtd.so /usr/local/lib/librtd.so

.PHONY: uninstall
uninstall:
	@echo "Removing /usr/local/bin/rtd"
	@rm -rf /usr/local/bin/rtd
	@echo "Removing /usr/local/lib/librtd.so"
	@rm -rf /usr/local/lib/librtd.so

.PHONY: clean
clean:
	@echo "Removing object files (*.o)"
	@rm -f *.o lib/*.o bin/linux/*.o

.PHONY: distclean
distclean: clean
	@echo "Removing binaries in bin/linux"
	@rm -rf bin/linux
