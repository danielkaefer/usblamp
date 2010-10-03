main: main.cpp USBLamp.cpp USBLamp.h
	g++ -Wall -o main main.cpp USBLamp.cpp USBLamp.h -lusb
clean:
	rm main
