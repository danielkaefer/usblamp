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

void setColor(char red, char green, char blue) {
	USBLamp lamp = USBLamp();
	lamp.open();
	if (lamp.isConnected()) {
		lamp.init();
		lamp.setColor(red, green, blue);
		lamp.close();
	} else {
		printf("no lamp found\n");
	}

}

void parseColor(char* hexcode) {
	if(strlen(hexcode) != 7) {
		return;
	}

	for(int i=1; i<7; ++i) {
		if(hexcode[i] < '0' && hexcode[i] > '9') {
			return;
		}
	}

	std::string hex(hexcode);

	unsigned int red;
	unsigned int green;
	unsigned int blue;
	sscanf(hex.substr(1,2).c_str(), "%X", &red);
	sscanf(hex.substr(3,2).c_str(), "%X", &green);
	sscanf(hex.substr(5,2).c_str(), "%X", &blue);
	setColor(red, green, blue);
}

int main(int argc, char** argv) {

	if(geteuid() != 0) {
		std::cout << "Need root access" << std::endl;
		return 1;
	}


	if(argc == 2) {
		if((argv[1][0]) == '#') {
			parseColor(argv[1]);
		} else if(strcmp(argv[1], "red") == 0) {
			setColor(255,0,0);
		} else if(strcmp(argv[1], "green") == 0) {
			setColor(0,255,0);
		} else if(strcmp(argv[1], "blue") == 0) {
			setColor(0,0,255);
		} else if(strcmp(argv[1], "white") == 0) {
			setColor(255,255,255);
		} else if(strcmp(argv[1], "magenta") == 0) {
			setColor(255,0,255);
		} else if(strcmp(argv[1], "cyan") == 0) {
			setColor(0,255,255);
		} else {
			// default set off
			setColor(0,0,0);
		}
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
