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

    /* ... */

    for (bus = busses; bus; bus = bus->next) {
        struct usb_device *dev;

        for (dev = bus->devices; dev; dev = dev->next) {
            /* Check if this device is a printer */

            //printf("%04X:%04X\n", dev->descriptor.idVendor, dev->descriptor.idProduct);

            if (ID_VENDOR == dev->descriptor.idVendor && ID_PRODUCT == dev->descriptor.idProduct) {
                device = dev;
                usb_set_debug(254);
                handler = usb_open(device);
                int result;
                //CALL(usb_reset(handler))
                CALL(usb_detach_kernel_driver_np(handler, 0))
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
    int timeout = 10;

    int result;
    CALL(usb_control_msg(handler, requesttype, request, value, index, bytes, size, timeout))
    printf("%d  bytes written/read\n", result);
    if (size != result) {
        printf("Error: color???\n");
    }
}

void USBLamp::setColor(Color color) {
    printf("Set color %d,%d,%d",color.getRed(), color.getGreen(), color.getBlue());
    char *data = (char *) malloc(sizeof (char) *8);
    data[0] = color.getRed();
    data[1] = color.getGreen();
    data[2] = color.getBlue();
    data[3] = 0x00;
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0x00;
    data[7] = 0x05;
    int size = 8;

    send(data, size);
}

void USBLamp::switchOff() {
    Color c = Color(0,0,0);
    setColor(c);
}

bool USBLamp::isConnected() {
    return handler != NULL;
}

/*void USBLamp::sendInterrupt() {
    //int ep = 0x81;
    char *bytes = (char *) malloc(sizeof (char) *8);
    bytes[0] = 'a';
    bytes[1] = 'b';
    bytes[2] = 'c';
    bytes[3] = 'd';
    bytes[4] = 'e';
    bytes[5] = 'f';
    bytes[6] = 'g';
    bytes[7] = 'h';
    int size = 8;
    int timeout = 250;

    int result;
    //CALL(usb_interrupt_read(handler, ENDPOINT, bytes, size, timeout))
    CALL(usb_interrupt_write(handler, ENDPOINT, bytes, size, timeout))
}*/

void USBLamp::close() {
    int result;
    //CALL(usb_reset(handler)) // off
    CALL(usb_release_interface(handler, 0))
    CALL(usb_close(handler))
}

USBLamp::~USBLamp() {
}

