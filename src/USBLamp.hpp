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

#ifndef USBLAMP_H
#define	USBLAMP_H

#include <stdlib.h>
#include <stdio.h>
#include <usb.h>

#include "Color.hpp"

#define DEBUG 0
#define ENDPOINT 0x81
#define ID_VENDOR 0x1d34
#define ID_PRODUCT 0x0004

#if DEBUG
#define CALL(X) printf("%s\n", #X); result = X; if(result < 0) { printf("ERROR Number: %d Description: %s\n", result, usb_strerror()); }
#else
#define CALL(X) result = X; if(result < 0) { printf("ERROR Number: %d Description: %s\n", result, usb_strerror()); }
#endif

class USBLamp {
public:
    USBLamp();
    void open();
    bool isConnected();
	void init();
    void switchOff();
    void setColor(Color newColor);
	Color getColor();
	void fading(unsigned int delay, Color newColor);
    void close();
    virtual ~USBLamp();
private:
    void send(char *data, int size);
    struct usb_device *device;
    struct usb_dev_handle *handler;
	Color color;
};

#endif	/* USBLAMP_H */

