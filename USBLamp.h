#ifndef USBLAMP_H
#define	USBLAMP_H

#include <stdlib.h>
#include <stdio.h>
#include <usb.h>

#define INTERFACE 1
#define ID_VENDOR 0x1d34
#define ID_PRODUCT 0x0004

class USBLamp {
public:
    USBLamp();
    void open();
    void send(); // parameter color, ...
    void close();
    virtual ~USBLamp();
private:
    struct usb_device *device;
    struct usb_dev_handle *handler;

};

#endif	/* USBLAMP_H */

