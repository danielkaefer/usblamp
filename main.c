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
    int c, i, a;

    /* ... */

    for (bus = busses; bus; bus = bus->next) {
        struct usb_device *dev;

        for (dev = bus->devices; dev; dev = dev->next) {
            /* Check if this device is a printer */

            printf("%04X:%04X\n", dev->descriptor.idVendor, dev->descriptor.idProduct);

            if(0) {
                return dev;
            }

            /* Opens a USB device */
            usb_dev_handle *udev;
            udev = usb_open(dev);
            if (udev) {
                if (dev->descriptor.iManufacturer) {
                    char string[256];
                    int ret = usb_get_string_simple(udev, dev->descriptor.iManufacturer, string, sizeof (string));
                    if (ret > 0) {
                        printf("    Manufacturer: %s\n", string);
                    }
                }
            }
            usb_close(udev);

            if (dev->descriptor.bDeviceClass == 7) {
                /* Open the device, claim the interface and do your processing */
            }

            /* Loop through all of the configurations */
            for (c = 0; c < dev->descriptor.bNumConfigurations; c++) {
                /* Loop through all of the interfaces */
                for (i = 0; i < dev->config[c].bNumInterfaces; i++) {
                    /* Loop through all of the alternate settings */
                    for (a = 0; a < dev->config[c].interface[i].num_altsetting; a++) {
                        /* Check if this interface is a printer */
                        if (dev->config[c].interface[i].altsetting[a].bInterfaceClass == 7) {
                            /* Open the device, set the alternate setting, claim the interface and do your processing */
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

/*
 *
 */
int main(int argc, char** argv) {
    printf("start\n");
    struct usb_device *display = findDisplay();
    if(display) {

    } else {
        printf("Keine Lampe gefunden");
    }
    printf("end\n");

    return 0;
}
