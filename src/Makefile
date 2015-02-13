CC = g++
CFLAGS = -Wall
LIBS = -lusb
SRC = main.cpp USBLamp.cpp Color.cpp
BIN = usblamp

main: $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $+ $(LIBS)

install:
	install -m 4755 $(BIN) /usr/bin

archive_dir = /tmp/usblamp
file_name = usblamp.sh
label = "Install WebMail Notifier Linux driver"
startup_script = "cp $(BIN) /usr/bin/$(BIN); chown root:root /usr/bin/$(BIN); chmod u+s /usr/bin/$(BIN)"

install-script:
	rm -rf $(archive_dir)
	mkdir $(archive_dir)
	cp $(BIN) $(archive_dir)
	makeself $(archive_dir) $(file_name) $(label) $(startup_script)

.PHONY: clean
clean:
	rm -f $(BIN)
