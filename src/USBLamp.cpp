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

#include "USBLamp.hpp"

USBLamp::USBLamp() {
}

void USBLamp::open() {
    struct usb_bus *busses;

    usb_init();
    usb_find_busses();
    usb_find_devices();

    busses = usb_get_busses();

    struct usb_bus *bus;

    for (bus = busses; bus; bus = bus->next) {
        struct usb_device *dev;

        for (dev = bus->devices; dev; dev = dev->next) {
            //printf("%04X:%04X\n", dev->descriptor.idVendor, dev->descriptor.idProduct);

            if (ID_VENDOR == dev->descriptor.idVendor && ID_PRODUCT == dev->descriptor.idProduct) {
                device = dev;
                //usb_set_debug(254);
                handler = usb_open(device);
                int result;
                //CALL(usb_reset(handler))
                //CALL(usb_detach_kernel_driver_np(handler, 0))
                usb_detach_kernel_driver_np(handler, 0);
                //CALL(usb_set_configuration(handler, 1))
                CALL(usb_claim_interface(handler, 0))
                //CALL(usb_claim_interface(handler, 1))
                return;
            }

        }
    }

    return;
}

void USBLamp::send(char *bytes, int size) {
    int requesttype = USB_TYPE_CLASS | USB_RECIP_INTERFACE;
    int request = 0x09;
    int value = 0x200;
    int index = 0x00;
    int timeout = 100;

    int result;
    CALL(usb_control_msg(handler, requesttype, request, value, index, bytes, size, timeout))
    if (size != result) {
        printf("Error: color???\n");
    }
}

void USBLamp::init() {
    int size = 8;
    char data[8];

    data = { 0x1f, 0x02, 0x00, 0x2e, 0x00, 0x00, 0x2b, 0x03 };
    send(data, size);

    data = { 0x00, 0x02, 0x00, 0x2e, 0x00, 0x00, 0x2b, 0x04 };
    send(data, size);

    data = { 0x00, 0x02, 0x00, 0x2e, 0x00, 0x00, 0x2b, 0x05 };
    send(data, size);
}

void USBLamp::setColor(char red, char green, char blue) {
    if(DEBUG) {
        printf("Set color %d,%d,%d\n",red, green, blue);
    }
    char data[] = {red, green, blue, 0x00, 0x00, 0x00, 0x00, 0x05};

    send(data, 8);
}

void USBLamp::switchOff() {
    setColor(0,0,0);
}

bool USBLamp::isConnected() {
    return handler != NULL;
}

void USBLamp::close() {
    int result;
    //CALL(usb_reset(handler)) // off
    CALL(usb_release_interface(handler, 0))
    CALL(usb_close(handler))
}

USBLamp::~USBLamp() {
}

