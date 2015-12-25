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

import imaplib
import yaml
import time
import os
import sys
import usblamp

configFile = os.path.join(os.path.dirname(__file__), 'imap.conf')
try:
    with open(configFile) as f:
        conf = yaml.load(f)
except IOError, e:
	print(e)
	sys.exit(1);

while True:
	mailbox = imaplib.IMAP4_SSL(conf['host'])
	mailbox.login(conf['username'], conf['password'])
	mailbox.select(conf['mailbox'])
	typ, msgnums = mailbox.search(None,'UnSeen')
	print msgnums
	if msgnums[0]:
		usblamp.switchTo(conf['colour'])

	mailbox.close()
	mailbox.logout()

        try:
            time.sleep(conf['refresh-time-interval'])
        except (KeyboardInterrupt, SystemExit):
            usblamp.switchTo('#000')
            sys.exit()

