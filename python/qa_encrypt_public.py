#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2015 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import nacl_swig as nacl
import pmt
from time import sleep

class qa_encrypt_public (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        msg = pmt.list2(pmt.string_to_symbol("msg_clear"),pmt.string_to_symbol("abcdefgh"))
        
        strobe = blocks.message_strobe(msg, 100)
        encrypt_public = nacl.encrypt_public("abcdabcdabcdabcdabcdabcdabcdabcd","defgdefgdefgdefgdefgdefgdefgdefg")
        debug = blocks.message_debug()
        
        self.tb.msg_connect(strobe,"strobe",encrypt_public,"Msg in")
        self.tb.msg_connect(encrypt_public,"Msg out",debug,"store")
        
        self.tb.start()
        sleep(0.15)
        self.tb.stop()
        self.tb.wait()
        
        # check results


if __name__ == '__main__':
    gr_unittest.run(qa_encrypt_public, "qa_encrypt_public.xml")
