/* 
 * File:   main.cpp
 * Author: daniel
 *
 * Created on 19. Juni 2010, 12:42
 */

#include <stdio.h>
#include <usb.h>

//using namespace std;

void findDisplay() {
    struct usb_bus *busses;

    usb_init();
    usb_find_busses();
    usb_find_devices();

    busses = usb_get_busses();

}

/*
 * 
 */
int main(int argc, char** argv) {
    printf("start");
    findDisplay();
    printf("end");

    return 0;
}

