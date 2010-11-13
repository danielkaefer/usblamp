CC = g++
CFLAGS = -Wall
LIBS = -lusb
SRC = main.cpp USBLamp.cpp USBLamp.hpp
BIN = main

main: $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $+ $(LIBS)

.PHONY: clean
clean:
	rm -f $(BIN)
