/* -*- c++ -*- */
/* 
 * Copyright 2015 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "encrypt_public_impl.h"

#include "crypto_box.h"

namespace gr {
  namespace nacl {

    encrypt_public::sptr
    encrypt_public::make(std::string pk, std::string sk)
    {
      return gnuradio::get_initial_sptr
        (new encrypt_public_impl(pk, sk));
    }

    /*
     * The private constructor
     */
    encrypt_public_impl::encrypt_public_impl(std::string pk, std::string sk)
      : gr::block("encrypt_public",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        d_pk = pk;
        d_sk = sk;
        
        // Register input message port
        d_port_id_in = pmt::mp("Msg in");
        message_port_register_in(d_port_id_in);
        set_msg_handler(d_port_id_in, boost::bind(&encrypt_public_impl::handle_msg, this, _1));
        
        // Register output message port
        d_port_id_out = pmt::mp("Msg out");
        message_port_register_out(d_port_id_out);
    }

    void
    encrypt_public_impl::handle_msg(pmt::pmt_t msg){
        std::cout << "encrypt_public: GOT MSG" << std::endl;
        
        std::string msg_enc;
        std::string nonce;
        
        d_pk = crypto_box_keypair(&d_sk);
        std::cout << "PRIVATE: " << d_sk << std::endl;
        std::cout << "PUBLIC: " << d_pk << std::endl;
    }

    /*
     * Our virtual destructor.
     */
    encrypt_public_impl::~encrypt_public_impl()
    {
    }

  } /* namespace nacl */
} /* namespace gr */

