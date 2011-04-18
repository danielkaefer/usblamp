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

Color getColor(char* color) {
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
		return Color(red, green, blue);
	} else if(strcmp(color, "red") == 0) {
		return Color(255,0,0);
	} else if(strcmp(color, "green") == 0) {
		return Color(0,255,0);
	} else if(strcmp(color, "blue") == 0) {
		return Color(0,0,255);
	} else if(strcmp(color, "white") == 0) {
		return Color(255,255,255);
	} else if(strcmp(color, "magenta") == 0) {
		return Color(255,0,255);
	} else if(strcmp(color, "cyan") == 0) {
		return Color(0,255,255);
	} else {
		// default set off
		return Color(0,0,0);
	}
}

int main(int argc, char** argv) {

	if(geteuid() != 0) {
		std::cout << "Need root access" << std::endl;
		return 1;
	}


	if(argc == 2) {
		setColor(getColor(argv[1]));
	} else {
		std::cout << "Usage: usblamp color" << std::endl;
		std::cout << "   or usblamp off" << std::endl;
		std::cout << "where colors include:" << std::endl;
		std::cout << "	red" << std::endl;
		std::cout << "	blue" << std::endl;
		std::cout << "	green" << std::endl;
		std::cout << "	white" << std::endl;
		std::cout << "	magenta" << std::endl;
		std::cout << "	cyan" << std::endl;
		std::cout << "Website: https://github.com/daniel-git/usblamp" << std::endl;
	}

	return 0;
}
// vim: noai:ts=4:sw=4
