/* 
 * File:   main.c
 * Author: daniel
 *
 * Created on 20. Juni 2010, 19:57
 */

#include <stdlib.h>
#include <stdio.h>
#include <usb.h>

//using namespace std;

struct usb_device *findDisplay() {
    struct usb_bus *busses;

    usb_init();
    usb_find_busses();
    usb_find_devices();

    busses = usb_get_busses();

    struct usb_bus *bus;

    /* ... */

    for (bus = busses; bus; bus = bus->next) {
        struct usb_device *dev;

        for (dev = bus->devices; dev; dev = dev->next) {
            /* Check if this device is a printer */

            //printf("%04X:%04X\n", dev->descriptor.idVendor, dev->descriptor.idProduct);

            if (0x1d34 == dev->descriptor.idVendor && 0x0004 == dev->descriptor.idProduct) {
                return dev;
            }

        }
    }

    return NULL;
}

void send(usb_dev_handle *handler) {
    int interface = 0;
    int result;

    result = usb_claim_interface(handler,interface);
    if(result < 0) {
        // todo error handling
        printf("Fehler %d\n", result);
    }

    int requesttype = 0;
    int request = 0xA;
    int value = 0;
    int index = 0;
    char *bytes = "ba7fb675";
    int size = 8;
    int timeout = 100;

    result = usb_control_msg(handler, requesttype, request, value, index, bytes, size, timeout);
    if(result < 0) {
        // todo error handling
        printf("Fehler %d\n", result);
    }

    result = usb_release_interface(handler, interface);
    if(result < 0) {
        // todo error handling
        printf("Fehler %d\n", result);
    }
}

/*
 *
 */
int main(int argc, char** argv) {
    printf("start\n");
    struct usb_device *display = findDisplay();
    if (display) {

        printf("Lampe gefunden\n");
        usb_dev_handle *handler = usb_open(display);

        send(handler);

        usb_close(handler);
    } else {
        printf("Keine Lampe gefunden");
    }
    printf("end\n");

    return 0;
}
