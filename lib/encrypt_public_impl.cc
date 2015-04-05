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
        
        // Check sizes of secret-key and public-key
        if(d_pk.size()!=crypto_box_PUBLICKEYBYTES){
            std::cout << "Size private-key: " << d_pk.size()*sizeof(char) << std::endl;
            std::cout << "Needed private-key size: " << crypto_box_PUBLICKEYBYTES << std::endl;
            throw std::runtime_error("Size private-key is wrong.");
        }
        if(d_sk.size()!=crypto_box_SECRETKEYBYTES){
            std::cout << "Size secret-key: " << d_sk.size()*sizeof(char) << std::endl;
            std::cout << "Needed secret-key size: " << crypto_box_PUBLICKEYBYTES << std::endl;
            throw std::runtime_error("Size secret-key is wrong.");
        }
        
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
        
        unsigned char *pk=new unsigned char[d_pk.length()+1];
        strcpy((char *)pk,d_pk.c_str());
        unsigned char *sk=new unsigned char[d_sk.length()+1];
        strcpy((char *)sk,d_sk.c_str());
        
        crypto_box_keypair(pk,sk);
        std::cout << "PRIVATE: " << sk << std::endl;
        std::cout << "PUBLIC: " << pk << std::endl;
    }

    /*
     * Our virtual destructor.
     */
    encrypt_public_impl::~encrypt_public_impl()
    {
    }

  } /* namespace nacl */
} /* namespace gr */

