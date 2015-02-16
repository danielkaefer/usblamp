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

#include <usb.h>
#include "USBLamp.hpp"

static unsigned const char riso_kagaku_tbl[] = {
/* R+2G+4B -> riso kagaku color index */
    0, /* [0] black   */
    2, /* [1] red     */
    1, /* [2] green   */
    5, /* [3] yellow  */
    3, /* [4] blue    */
    6, /* [5] magenta */
    4, /* [6] cyan    */
    7  /* [7] white   */
};
#define RISO_KAGAKU_IX(r,g,b) riso_kagaku_tbl[((r)?1:0)+((g)?2:0)+((b)?4:0)]

USBLamp::USBLamp() {
  handler = NULL;
  led_type = 0;
}

void USBLamp::open() {
    struct usb_bus *busses;

    usb_init();
    usb_find_busses();
    usb_find_devices();

    busses = usb_get_busses();

    led_type = 0;

    struct usb_bus *bus;

    for (bus = busses; bus; bus = bus->next) {
        struct usb_device *dev;

        for (dev = bus->devices; dev; dev = dev->next) {
//printf("%04X:%04X\n", dev->descriptor.idVendor, dev->descriptor.idProduct);

            if (ID_VENDOR == dev->descriptor.idVendor && (ID_PRODUCT_OLD == dev->descriptor.idProduct || ID_PRODUCT_NEW == dev->descriptor.idProduct)) led_type = 1;
            else if (ID_VENDOR_2 == dev->descriptor.idVendor && ID_PRODUCT_2 == dev->descriptor.idProduct) led_type = 2;

            if (led_type) {
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
  int result = 0;
  int timeout = 1000;    // ms

  if (led_type == 1) {
    int requesttype = USB_TYPE_CLASS | USB_RECIP_INTERFACE;
    int request = 0x09;
    int value = 0x200;
    int index = 0x00;

    if (DEBUG) {
      printf("send(%d) %02X %02X %02X %02X %02X %02X %02X %02X\n",size,bytes[0],bytes[1],bytes[2],bytes[3],bytes[4],bytes[5],bytes[6],bytes[7]);
    }
    CALL(usb_control_msg(handler, requesttype, request, value, index, bytes, size, timeout))
  } else if (led_type == 2) {
    int ep = 0x02;

    if (DEBUG) {
      printf("send(%d) %02X %02X %02X %02X %02X\n",size,bytes[0],bytes[1],bytes[2],bytes[3],bytes[4]);
    }
    CALL(usb_interrupt_write(handler, ep, bytes, size, timeout))
  }
  if (size != result) {
    printf("Error: %d:%d color?\n",size,result);
  }
}

void USBLamp::init() {
  if (led_type == 1) {
    char data1[8] = { 0x1f, 0x02, 0x00, 0x2e, 0x00, 0x00, 0x2b, 0x03 };
    send(data1, sizeof(data1));

    char data2[8] = { 0x00, 0x02, 0x00, 0x2e, 0x00, 0x00, 0x2b, 0x04 };
    send(data2, sizeof(data2));

    char data3[8] = { 0x00, 0x02, 0x00, 0x2e, 0x00, 0x00, 0x2b, 0x05 };
    send(data3, sizeof(data3));
  }
}

Color USBLamp::getColor() {
  return color;
}

void USBLamp::setColor(Color newColor) {
  color = newColor;

  if(DEBUG) {
    printf("Set color %d,%d,%d\n",color.red, color.green, color.blue);
  }

  if (led_type == 1) {
    char data[] = {color.red, color.green, color.blue, 0x00, 0x00, 0x00, 0x00, 0x05};
    send(data, 8);
  } else if (led_type == 2) {
    char data[] = {RISO_KAGAKU_IX(color.red,color.green,color.blue), 0x00, 0x00, 0x00, 0x00};
    send(data, 5);
  }
}

void USBLamp::switchOff() {
  setColor(Color(0,0,0));
}

bool USBLamp::isConnected() {
  return handler != NULL;
}

void USBLamp::fading(unsigned int delay, Color newColor) {
  Color c;
  if (led_type == 1) {
// Do fading
    for (int j=0; j<color.maxval; ++j) {
      usleep(delay*1000/color.maxval+1);
      c.red = color.red + (newColor.red-color.red)*j/color.maxval;
      c.green = color.green + (newColor.green-color.green)*j/color.maxval;
      c.blue = color.blue + (newColor.blue-color.blue)*j/color.maxval;
      setColor(c);
    }
  } else {
    usleep(delay*1000);
    c.red = newColor.red;
    c.green = newColor.green;
    c.blue = newColor.blue;
    setColor(c);
  }
}

void USBLamp::close() {
  int result;
//CALL(usb_reset(handler)) // off
  CALL(usb_release_interface(handler, 0))
  CALL(usb_close(handler))
  led_type = 0;
}

USBLamp::~USBLamp() {
}
