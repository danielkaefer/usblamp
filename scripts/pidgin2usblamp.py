#!/usr/bin/env python

# Copyright (C) 2010 Daniel Kaefer
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

import dbus
import gobject
import subprocess
import time
import signal
from dbus.mainloop.glib import DBusGMainLoop

def handler(signum, fame):
    print("exit")
    raise SystemExit()
signal.signal(signal.SIGINT, handler)

def lamp(color):
	shell = "usblamp %s" % color
	subprocess.Popen([shell], shell=True, stdout=subprocess.PIPE, close_fds=True)

def send2usb(username, message):
	print "Nachricht von %s" % username
	lamp("red")
	time.sleep(0.5)
	lamp("off")
	time.sleep(0.2)
	lamp("red")
	time.sleep(0.5)
	lamp("off")
	time.sleep(0.2)
	lamp("red")
	time.sleep(0.5)
	lamp("white")

def receivedMessage(account, sender, message, conversation, flags):
    buddy = purple.PurpleFindBuddy(account, sender)
    alias = purple.PurpleBuddyGetAlias(buddy)
    send2usb(alias, message)

lamp("blue")
time.sleep(7.5)
lamp("off")

dbus.mainloop.glib.DBusGMainLoop(set_as_default=True)
bus = dbus.SessionBus()
bus.add_signal_receiver(receivedMessage,
                        dbus_interface="im.pidgin.purple.PurpleInterface",
                        signal_name="ReceivedImMsg")
obj = bus.get_object("im.pidgin.purple.PurpleService", "/im/pidgin/purple/PurpleObject")
purple = dbus.Interface(obj, "im.pidgin.purple.PurpleInterface")


loop = gobject.MainLoop()
loop.run()

