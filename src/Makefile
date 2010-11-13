CC = g++
CFLAGS = -Wall
LIBS = -lusb
SRC = main.cpp USBLamp.cpp USBLamp.hpp Color.cpp Color.hpp
BIN = main

main: $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $+ $(LIBS)

.PHONY: clean
clean:
	rm -f $(BIN)
