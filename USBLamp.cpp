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
				int result;
				CALL(usb_reset(handler))
				CALL(usb_detach_kernel_driver_np(handler, 0))
                CALL(usb_set_configuration(handler, 1))
                CALL(usb_claim_interface(handler, 0))
                //CALL(usb_claim_interface(handler, 1))
                return;
            }

        }
    }

    return;
    // todo keine lampe gefunden
}

void USBLamp::send() {
    int requesttype = 0x21;
    int request = 0x09;
    //int request = 0x18;
    int value = 0x0;
    int index = 0x0;
    //int index = 0x2;
    char *bytes = "ba7fb675";
    int size = 8;
    int timeout = 250;

	int result;
    CALL(usb_control_msg(handler, requesttype, request, value, index, bytes, size, timeout))
}

void USBLamp::sendInterrupt() {
	int ep = 0x80;
	char *bytes = (char *) malloc(sizeof(char)*8);
	int size = 8;
	int timeout = 250;

	int result;
	//CALL(usb_interrupt_read(handler, ep, bytes, size, timeout))
	CALL(usb_interrupt_write(handler, ep, bytes, size, timeout))
}

void USBLamp::close() {
	int result;
    CALL(usb_release_interface(handler, 0))
    usb_close(handler);
}

USBLamp::~USBLamp() {
}

