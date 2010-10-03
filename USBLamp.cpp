#include "USBLamp.h"


USBLamp::USBLamp() {
}

void USBLamp::open() {
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

            if (ID_VENDOR == dev->descriptor.idVendor && ID_PRODUCT == dev->descriptor.idProduct) {
                device = dev;
                handler = usb_open(device);
                int result = usb_claim_interface(handler, INTERFACE);
                if(result < 0) {
                    // todo error handling
                    printf("Fehler %d\n", result);
                }
                return;
            }

        }
    }

    return;
    // todo keine lampe gefunden
}
void USBLamp::send() {
    int requesttype = 0;
    int request = 0xA;
    int value = 0;
    int index = 0;
    char *bytes = "ba7fb675";
    int size = 8;
    int timeout = 50;

    int result = usb_control_msg(handler, requesttype, request, value, index, bytes, size, timeout);
    if(result < 0) {
        // todo error handling
        printf("Fehler %d\n", result);
    }
}

void USBLamp::close() {
    int result = usb_release_interface(handler, INTERFACE);
    if(result < 0) {
        // todo error handling
        printf("Fehler %d\n", result);
    }
    usb_close(handler);
}

USBLamp::~USBLamp() {
}

