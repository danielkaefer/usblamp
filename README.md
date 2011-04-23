WebMail Notifier Linux driver (Dream Cheeky)
============================================

Installation
------------
dependency libusb++-dev
    wget https://github.com/daniel-git/usblamp/tarball/master --no-check-certificate
    tar xzf master
    cd daniel-git-usblamp*/src
    make
    sudo make install

Usage
-----

 * Switch the red light on `sudo usblamp red`
 * Switch to green light on (hexcode) `sudo usblamp #00FF00`
 * Turn the lamp off `sudo usblamp off`

License
-------

GNU GENERAL PUBLIC LICENSE 
