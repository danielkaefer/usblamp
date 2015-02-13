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
#include <string.h>
#include <iostream>
#include <string>
#include <unistd.h>

#include "USBLamp.hpp"
#include "Color.hpp"

// using namespace std;

Color getColor(const char* param, unsigned char maxval) {
  Color ret = Color();
  if (param) {
    char *p1,*color = (char*) malloc(strlen(param)+1);
    strcpy(color,param);
    if ((p1 = strchr(color,','))) *p1 = 0;
    if(((color[0]) == '#')  || ((color[0]) == '_')) {
      if(strlen(color) == 4 || strlen(color) == 7) {
        char flag = 0;
        std::string hex(color);
        unsigned int red = 0, green = 0, blue = 0;
        if (strlen(color) == 4) {
// Try to parse the color as a shorthand 3-character color (e.g. '#FFF')
          if (sscanf(hex.substr(1,1).c_str(), "%X", &red) +
              sscanf(hex.substr(2,1).c_str(), "%X", &green) +
              sscanf(hex.substr(3,1).c_str(), "%X", &blue) == 3) {
            red = red << 4;
            green = green << 4;
            blue = blue << 4;
            flag++;
          }
        } else if (strlen(color) == 7) {
// Try to parse the 6-character color (e.g. '#FFFFFF')
          if (sscanf(hex.substr(1,2).c_str(), "%X", &red) +
              sscanf(hex.substr(3,2).c_str(), "%X", &green) +
              sscanf(hex.substr(5,2).c_str(), "%X", &blue) == 3) {
            flag++;
          }  
        }
        if (flag) {
          red = std::max((unsigned int) 0, std::min((unsigned int) 255, red*maxval/255));
          green = std::max((unsigned int) 0, std::min((unsigned int) 255, green*maxval/255));
          blue = std::max((unsigned int) 0, std::min((unsigned int) 255, blue*maxval/255));
          ret =  Color(red, green, blue);
        }
      }
    } else if (!strcmp(color, "red")) {
      ret = Color(maxval,0,0);
    } else if (!strcmp(color, "green")) {
      ret = Color(0,maxval,0);
    } else if (!strcmp(color, "blue")) {
      ret = Color(0,0,maxval);
    } else if (!strcmp(color, "white")) {
      ret = Color(maxval,maxval,maxval);
    } else if (!strcmp(color, "magenta")) {
      ret = Color(maxval,0,maxval);
    } else if (!strcmp(color, "cyan")) {
      ret = Color(0,maxval,maxval);
    } else if (!strcmp(color, "yellow")) {
      ret = Color(maxval,maxval,0);
    } else {                            // default set off
      ret = Color(0,0,0);
    }
    free(color);
  }
  return ret;
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

void print_help() {
	std::cout << "Usage: usblamp [-s] [-r <num>[,<delay>]] [-p <port>] [-d <delay>] color[,<delay>] [color...]" << std::endl;
	std::cout << "   valid colors: [red blue green white magenta cyan yellow off], #rrggbb (hex) or #rgb (hex)" << std::endl;
	std::cout << "   an optional parameter color[,<delay>] will overwrite the -d option for that color" << std::endl << std::endl;
	std::cout << "   -s will switch between colors and disable fade" << std::endl << std::endl;
	std::cout << "   -r <num>[,<delay>] will repeat the color sequence <num> times (default is 0)" << std::endl;
	std::cout << "      with an optional <delay> (default is 0ms) between sequences" << std::endl << std::endl;
	std::cout << "   -d <delay> will set switch/fade delay between colors (default is 250ms)" << std::endl << std::endl;
	std::cout << "   -p <port> will listen on the specified UDP socket. Datagrams of >= 3 bytes" << std::endl;
	std::cout << "      will set color using bytes[0..2]=[red, green, blue], eg. [0 0xff 0xff]=cyan." << std::endl << std::endl;
//	std::cout << "   The previously set color will be sent as a reply." << std::endl << std::endl;
	std::cout << "original website: https://github.com/daniel-git/usblamp" << std::endl << std::endl;
	std::cout << "Version 1.0" << std::endl;
	std::cout << "  supported USB lamps: 0x1d34:0x0004, 0x1d34:0x000a, 0x1294:0x1320" << std::endl;
}

int main(int argc, char** argv) {
    unsigned int delay = 250; // Milliseconds
    unsigned int port = 0;    // UDP datagram port
    unsigned short repeat = 0;
    unsigned int repeat_wait = 0;
    char opt_help = 0;
    char opt_switch = 0;
    int opt;

// Check root access
    if (geteuid() != 0) {
	std::cout << "Need root access" << std::endl;
	return 1;
    }

    while ((opt = getopt(argc, argv, "hsr:d:p:")) != -1) {  // for each option...
        char *p1;
        switch ( opt ) {
            case 'h':
                opt_help++;
                break;
            case 's':
                opt_switch++;
                break;
            case 'r':
                repeat = atoi(optarg);
                if ((p1 = strchr(optarg,','))) {
                  repeat_wait = atoi(p1+1);
                }
                break;
            case 'd':
                delay = atoi(optarg);
		if (DEBUG) {
		    std::cout << "Delay: " << delay << "ms" << std::endl;
		}
                break;
            case 'p':
                port = atoi(optarg);
                if (DEBUG) {
		    std::cout << "Listening on UDP port " << port << std::endl;
		}
                break;
            case '?':  // unknown option...
                opt_help++;
                break;
        }
    }
    repeat++;
    optind--;
    argc -= optind;
    argv += optind;
    if ((!port && argc < 2) || opt_help) {
        print_help();
        return 0;
    }

    USBLamp lamp = USBLamp();
    lamp.open();
    if (lamp.isConnected()) {
        unsigned int wait = delay;
        char first = 1;
        lamp.init();
        lamp.switchOff();
        while (repeat--) {
          for (int i=1; i<argc; i++) {
              char *p1 = argv[i];
              Color color = getColor(p1, Color::maxval);
              if (first) {
                  lamp.setColor(color);
                  first = 0;
              } else {
                  if (opt_switch) {
                    usleep(wait*1000);
                    lamp.setColor(color);
                  } else {
                    lamp.fading(wait,color);
                  }  
              }
              if ((p1 = strchr(p1,','))) wait = atoi(p1+1);
              else wait = delay;
          }
          if (repeat && repeat_wait) usleep(repeat_wait * 1000);
        }
        if (port) listen(lamp, port);
        lamp.close();
    } else {
        std::cout << "no lamp found" << std::endl;
    }
    return 0;
}
// vim: noai:ts=4:sw=4
