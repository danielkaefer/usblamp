CC = g++
CFLAGS = -Wall -std=c++0x
LIBS = -lusb
SRC = main.cpp USBLamp.cpp USBLamp.hpp
BIN = usblamp

main: $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $+ $(LIBS)

install:
	cp $(BIN) /usr/bin/$(BIN)
	chown root:root /usr/bin/$(BIN)
	chmod u+s /usr/bin/$(BIN)

.PHONY: clean
clean:
	rm -f $(BIN)
