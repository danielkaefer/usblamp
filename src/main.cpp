/*
 * Copyright (C) 2010  Daniel Kaefer
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <usb.h>
#include <string.h>
#include <iostream>
#include <string>

#include "USBLamp.hpp"

//using namespace std;

struct Color {
	unsigned char red, green, blue;
	Color() : red(0), green(0), blue(0) { }
	Color(unsigned char r, unsigned char g, unsigned char b) : red(r), green(g), blue(b) { }
};


void setColor(Color color) {
	USBLamp lamp = USBLamp();
	lamp.open();
	if (lamp.isConnected()) {
		lamp.init();
		lamp.setColor(color.red, color.green, color.blue);
		lamp.close();
	} else {
		printf("no lamp found\n");
	}

}

Color getColor(char* color, unsigned char maxval) {
	if((color[0]) == '#') {
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
	} else {
		// default set off
		return Color(0,0,0);
	}
}


int main(int argc, char** argv) {
	unsigned char maxval = 0x40; // Brightest value
	unsigned int delay = 250; // Milliseconds

	if(geteuid() != 0) {
		std::cout << "Need root access" << std::endl;
		return 1;
	}

	if(argc > 1) {
		for (int i=1; i<argc; ++i) {
			Color color = getColor(argv[i], maxval);
			setColor(color);
			if (i+1<argc) {
				Color nextColor = getColor(argv[i+1], maxval);
				// Do fading
				for (int j=0; j<maxval; ++j) {
					usleep(delay*1000/maxval+1);
					Color c;
					c.red = color.red + (nextColor.red-color.red)*j/maxval;
					c.green = color.green + (nextColor.green-color.green)*j/maxval;
					c.blue = color.blue + (nextColor.blue-color.blue)*j/maxval;
					setColor(c);
				}
			}
		}
	} else {
		std::cout << "Usage: usblamp color [color...]" << std::endl;
		std::cout << "where colors include:" << std::endl;
		std::cout << "	red" << std::endl;
		std::cout << "	blue" << std::endl;
		std::cout << "	green" << std::endl;
		std::cout << "	white" << std::endl;
		std::cout << "	magenta" << std::endl;
		std::cout << "	cyan" << std::endl;
		std::cout << "	#rrggbb (hex)" << std::endl;
		std::cout << "	off" << std::endl;
		std::cout << "Website: https://github.com/daniel-git/usblamp" << std::endl;
	}

	return 0;
}
// vim: noai:ts=4:sw=4
