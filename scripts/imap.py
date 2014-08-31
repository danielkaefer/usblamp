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

configFile = '%s/imap.conf' % os.path.dirname(__file__)
if not os.path.exists(configFile):
	print 'Config file %r not found' % configFile
	sys.exit(1);

with open(configFile) as f:
    conf = yaml.load(f)

while True:
	mailbox = imaplib.IMAP4_SSL(conf['host'])
	mailbox.login(conf['username'], conf['password'])
	mailbox.select()
	typ, msgnums = mailbox.search(None,'UnSeen')
	print msgnums
	if msgnums[0]:
		usblamp.switchTo('blue')

	mailbox.close()
	mailbox.logout()

	time.sleep(conf['refresh-time-interval'])

