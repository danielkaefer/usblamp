/*
 * Copyright (C) 2010-2011 Daniel Kaefer and other contributors see
 * <https://github.com/daniel-git/usblamp/contributors/> for more details
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <usb.h>
#include <string.h>
#include <iostream>
#include <string>
#include <unistd.h>

#include "USBLamp.hpp"
#include "Color.hpp"

//using namespace std;

Color getColor(char* color, unsigned char maxval) {
	if(((color[0]) == '#')  || ((color[0]) == '_')) {
		if(strlen(color) != 4 && strlen(color) != 7) {
			return Color();
		}
		std::string hex(color);
		unsigned int red = 0, green = 0, blue = 0;
		// Try to parse the color as a shorthand 3-character color (e.g. '#FFF')
		if(strlen(color) == 4) {
			if(sscanf(hex.substr(1,1).c_str(), "%X", &red) +
					sscanf(hex.substr(2,1).c_str(), "%X", &green) +
					sscanf(hex.substr(3,1).c_str(), "%X", &blue)==3) {
				red = red << 4;
				green = green << 4;
				blue = blue << 4;
			} else {
				return Color();
			}
		}
		// Try to parse the 6-character color (e.g. '#FFFFFF')
		if(strlen(color) == 7 &&
				(sscanf(hex.substr(1,2).c_str(), "%X", &red) +
				sscanf(hex.substr(3,2).c_str(), "%X", &green) +
				sscanf(hex.substr(5,2).c_str(), "%X", &blue))!=3) {
			return Color();
		}

		red = std::max((unsigned int) 0, std::min((unsigned int) 255, red*maxval/255));
		green = std::max((unsigned int) 0, std::min((unsigned int) 255, green*maxval/255));
		blue = std::max((unsigned int) 0, std::min((unsigned int) 255, blue*maxval/255));
		return Color(red, green, blue);
	} else if(strcmp(color, "red") == 0) {
		return Color(maxval,0,0);
	} else if(strcmp(color, "green") == 0) {
		return Color(0,maxval,0);
	} else if(strcmp(color, "blue") == 0) {
		return Color(0,0,maxval);
	} else if(strcmp(color, "white") == 0) {
		return Color(maxval,maxval,maxval);
	} else if(strcmp(color, "magenta") == 0) {
		return Color(maxval,0,maxval);
	} else if(strcmp(color, "cyan") == 0) {
		return Color(0,maxval,maxval);
	} else if(strcmp(color, "yellow") == 0) {
		return Color(maxval,maxval,0);
	} else {
		// default set off
		return Color(0,0,0);
	}
}

void print_help() {
	std::cout << "Usage: usblamp [-p <port>] [-d <delay>] color [color...]" << std::endl;
	std::cout << "   -d <delay> will set fade delay between colors: default is 250ms" << std::endl;
	std::cout << "   valid colors: [red blue green white magenta cyan yellow off], #rrggbb (hex) or #rgb (hex)" << std::endl << std::endl;
	std::cout << "   -p <port> will listen on the specified UDP socket. Datagrams of >= 3 bytes" << std::endl;
	std::cout << "   will set color using bytes[0..2]=[red, green, blue], eg. [0 0xff 0xff]=cyan." << std::endl << std::endl;
	std::cout << "   The previously set color will be sent as a reply." << std::endl << std::endl;
	std::cout << "Website: https://github.com/daniel-git/usblamp" << std::endl;
}

void listen(USBLamp lamp, int port) {
	struct sockaddr_in server;
	struct sockaddr_in from;
	unsigned char buf[1024];

	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		std::cout << "Can't open socket " << std::endl;
		exit(1);
	}
	socklen_t length = sizeof(server);
	memset(&server, 0, (int)length);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	if (bind(sock, (struct sockaddr *)&server, length) < 0) {
		std::cout << "Can't bind to port " << port << std::endl;
		exit(1);
	}
	int n;
	int maxval = Color::maxval;
	do {
		n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *)&from, &length);
		if (n > 2) {
			int red = std::max(0, std::min(255, buf[0]*maxval/255));
			int green = std::max(0, std::min(255, buf[1]*maxval/255));
			int blue = std::max(0, std::min(255, buf[2]*maxval/255));
			Color color = Color(red, green, blue);
			Color lastcolor = lamp.getColor();
			lamp.setColor(color);
			buf[0] = lastcolor.red*255/maxval;
			buf[1] = lastcolor.green*255/maxval;
			buf[2] = lastcolor.blue*255/maxval;
			n = sendto(sock, buf, 3, 0, (struct sockaddr *)&from, length);
		}
	} while (n >= 0);
}

int main(int argc, char** argv) {
	unsigned int delay = 250; // Milliseconds
	unsigned int port = 0;    // UDP datagram port

	// Check root access
	if(geteuid() != 0) {
		std::cout << "Need root access" << std::endl;
		return 1;
	}

	if(argc < 2 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        print_help();
        return 0;
    }

	USBLamp lamp = USBLamp();
	lamp.open();
	if (lamp.isConnected()) {
		lamp.init();
		lamp.switchOff();
		for (int i=1; i<argc; ++i) {
			if (strlen(argv[i]) >= 2 && argv[i][0] == '-') {
				char op = argv[i][1];
				char *argval = strlen(argv[i]) == 2 ? argv[++i] : &argv[i][2];
				if (op == 'd') {
					delay = atoi(argval);
					if (DEBUG) {
						std::cout << "Delay: " << delay << "ms" << std::endl;
					}
				} else if (op =='p') {
				    port = atoi(argval);
					if (DEBUG) {
						std::cout << "Listening on UDP port " << port << std::endl;
					}
					i = argc;
				} else {
				    print_help();
				}
			} else {
				Color color = getColor(argv[i], Color::maxval);
				lamp.setColor(color);
				if (i+1<argc) {
					Color nextColor = getColor(argv[i+1], Color::maxval);
					lamp.fading(delay, nextColor);
				}
			}
		}
		if (port != 0) {
		    listen(lamp, port);
		}
		lamp.close();
	} else {
		std::cout << "no lamp found" << std::endl;
	}

	return 0;
}
// vim: noai:ts=4:sw=4
