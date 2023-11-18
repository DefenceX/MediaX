#!/usr/bin/python3 
#
# Copyright (c) 2023, DefenceX PTY LTD
#
# This file is part of the VivoeX project developed by DefenceX.
#
# Licensed under the Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)
# License. See the LICENSE file in the project root for full license details.
#

import sys
sys.path.append('/usr/local/lib/python3/dist-packages/mediax')
import rtp_uncompressed_payloader

# Create a RTPVrawPayloader object

rtp = rtp_uncompressed_payloader.rtp_uncompressed_payloader()


