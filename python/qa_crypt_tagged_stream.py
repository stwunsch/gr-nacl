#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2020 Stefan Wunsch.
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

class qa_crypt_tagged_stream(gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        data = [ord('t'),ord('e'),ord('s'),ord('t'),ord('#')]*8
        packet_len = len(data)/8
        key = "12345678123456781234567812345678"
        nonce = "abcdefghabcdefghabcdefgh"
        rotate_nonce = True

        src = blocks.vector_source_b(data)
        s2ts = blocks.stream_to_tagged_stream(1,1,packet_len,"packet_len")
        encrypt = nacl.crypt_tagged_stream(key,nonce,rotate_nonce)
        decrypt = nacl.crypt_tagged_stream(key,nonce,rotate_nonce)
        sink_encrypt = blocks.vector_sink_b()
        sink_decrypt = blocks.vector_sink_b()

        self.tb.connect(src,s2ts,encrypt,decrypt,sink_decrypt)
        self.tb.connect(encrypt,sink_encrypt)

        self.tb.run ()
        # print(data)
        print("INPUT DATA")
        for k in range(len(data)):
            print(chr(data[k]))
        print
        print("INPUT DATA ENCRYPTED")
        for k in range(len(sink_encrypt.data())):
            print(chr(sink_encrypt.data()[k]))
        print
        print("OUTPUT DATA (EQUALS INPUT DATA ENCRYPTED AND DECRYPTED)")
        for k in range(len(sink_decrypt.data())):
            print(chr(sink_decrypt.data()[k]))
        print

        # check data
        data_out =  sink_decrypt.data()
        for k in range(len(data)):
            self.assertEqual(data[k],data_out[k])

if __name__ == '__main__':
    gr_unittest.run(qa_crypt_tagged_stream)
