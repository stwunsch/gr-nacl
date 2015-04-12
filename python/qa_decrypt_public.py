#!/usr/bin/env python
# -*- coding: utf-8 -*-
# /* 
#  * Copyright 2015 Stefan Wunsch
#  * 
#  * This is free software; you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License as published by
#  * the Free Software Foundation; either version 3, or (at your option)
#  * any later version.
#  * 
#  * This software is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  * 
#  * You should have received a copy of the GNU General Public License
#  * along with this software; see the file COPYING.  If not, write to
#  * the Free Software Foundation, Inc., 51 Franklin Street,
#  * Boston, MA 02110-1301, USA.
#  */
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import nacl_swig as nacl
import pmt
from time import sleep

class qa_decrypt_public (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        data = [ord('a'),ord('b'),ord('c'),ord('d')]
        msg = pmt.list1(pmt.list2(pmt.string_to_symbol("msg_clear"),pmt.init_u8vector(len(data),data)))
        filename_sk_a = "secret.a"
        filename_pk_a = "public.a"
        nacl.generate_keypair(filename_sk_a,filename_pk_a)
        filename_sk_b = "secret.b"
        filename_pk_b = "public.b"
        nacl.generate_keypair(filename_sk_b,filename_pk_b)
        
        strobe = blocks.message_strobe(msg, 100)
        encrypt_public = nacl.encrypt_public(filename_pk_b,filename_sk_a)
        decrypt_public = nacl.decrypt_public(filename_pk_a,filename_sk_b)
        debug = blocks.message_debug()
        
        self.tb.msg_connect(strobe,"strobe",encrypt_public,"Msg clear")
        self.tb.msg_connect(encrypt_public,"Msg encrypted",decrypt_public,"Msg encrypted")
        self.tb.msg_connect(decrypt_public,"Msg decrypted",debug,"store")
        
        self.tb.start()
        sleep(0.15)
        self.tb.stop()
        self.tb.wait()
        
        # check results
        msg_out = debug.get_message(0)
        msg_symbol = pmt.symbol_to_string(pmt.nth(0,pmt.nth(0,msg_out)))
        msg_decrypted = pmt.u8vector_elements(pmt.nth(1,pmt.nth(0,msg_out)))
        print msg_symbol, msg_decrypted
        print "msg_clear", data
        
        for k in range(len(data)):
            self.assertEqual(data[k],msg_decrypted[k])

if __name__ == '__main__':
    gr_unittest.run(qa_decrypt_public)#, "qa_decrypt_public.xml")
