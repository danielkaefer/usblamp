#ifndef USBLAMP_H
#define	USBLAMP_H

#include <stdlib.h>
#include <stdio.h>
#include <usb.h>

#define ENDPOINT 0x81
#define ID_VENDOR 0x1d34
#define ID_PRODUCT 0x0004

#define CALL(X) printf("%s\n", #X); result = X; if(result < 0) { printf("ERROR Number: %d Description: %s\n", result, usb_strerror()); }

class USBLamp {
public:
    USBLamp();
    void open();
    bool isConnected();
    void send(); // parameter color, ...
    void sendInterrupt();
    void close();
    virtual ~USBLamp();
private:
    struct usb_device *device;
    struct usb_dev_handle *handler;

};

#endif	/* USBLAMP_H */

