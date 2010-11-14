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

int main(int argc, char** argv) {

    if(argc == 2) {
        if(strcmp(argv[1], "red") == 0) {
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
        printf("usage\n");
    }

    return 0;
}
