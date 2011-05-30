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
		if(strlen(color) != 7) {
			return Color();
		}
		std::string hex(color);
		unsigned int red = 0, green = 0, blue = 0;
		if ((sscanf(hex.substr(1,2).c_str(), "%X", &red) +
					sscanf(hex.substr(3,2).c_str(), "%X", &green) +
					sscanf(hex.substr(5,2).c_str(), "%X", &blue))!=3) {
			return Color();
		}
		if (red>maxval) red=maxval;
		if (green>maxval) green=maxval;
		if (blue>maxval) blue=maxval;
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
	std::cout << "Usage: usblamp color [color...]" << std::endl;
	std::cout << "where colors include:" << std::endl;
	std::cout << "	red" << std::endl;
	std::cout << "	blue" << std::endl;
	std::cout << "	green" << std::endl;
	std::cout << "	white" << std::endl;
	std::cout << "	magenta" << std::endl;
	std::cout << "	cyan" << std::endl;
	std::cout << "	yellow" << std::endl;
	std::cout << "	'#rrggbb' (hex)" << std::endl;
	std::cout << "	off" << std::endl;
	std::cout << "Website: https://github.com/daniel-git/usblamp" << std::endl;
}


int main(int argc, char** argv) {
	unsigned int delay = 250; // Milliseconds

	// Check root access
	if(geteuid() != 0) {
		std::cout << "Need root access" << std::endl;
		return 1;
	}

	if(argc < 2) {
        print_help();
        return 0;
    }

	USBLamp lamp = USBLamp();
	lamp.open();
	if (lamp.isConnected()) {
		lamp.init();
		bool nextArgDelay = false;
		for (int i=1; i<argc; ++i) {
			if (strlen(argv[i]) > 4 && argv[i][0] == '-' && argv[i][1] == 'd') {
				delay = atoi(&argv[i][2]);
				if (DEBUG) {
					std::cout << "Delay: " << delay << "ms" << std::endl;
				}
				continue;
			}
			if (strlen(argv[i]) == 2 && argv[i][0] == '-' && argv[i][1] == 'd') {
				nextArgDelay = true;	
				continue;
			}
			if (nextArgDelay) {
				delay = atoi(argv[i]);
				nextArgDelay = false;
				if (DEBUG) {
					std::cout << "Delay: " << delay << "ms" << std::endl;
				}
				continue;
			}
			Color color = getColor(argv[i], Color::maxval);
			lamp.setColor(color);
			if (i+1<argc) {
				Color nextColor = getColor(argv[i+1], Color::maxval);
				lamp.fading(delay, nextColor);
			}
		}
		lamp.close();
	} else {
		std::cout << "no lamp found" << std::endl;
	}

	return 0;
}
// vim: noai:ts=4:sw=4
