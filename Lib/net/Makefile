CC = gcc

libnet.a: libnet.c libnet.h
	@echo "Compiling lib:net module.."
	$(CC) -c libnet.c -o libnet.o
	ar rcs libnet.a libnet.o
	@echo "Compiling lib:net module done.."

install:
	cp libnet.a ../

.PHONY: clean
clean:
	-rm libnet.a
	-rm *.o
