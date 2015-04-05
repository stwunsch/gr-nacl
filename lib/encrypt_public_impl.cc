/* -*- c++ -*- */
/* 
 * Copyright 2015 Stefan Wunsch
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

#include <fstream>
#include "crypto_box.h"

namespace gr {
  namespace nacl {

    encrypt_public::sptr
    encrypt_public::make(std::string filename_pk, std::string filename_sk)
    {
      return gnuradio::get_initial_sptr
        (new encrypt_public_impl(filename_pk, filename_sk));
    }

    /*
     * The private constructor
     */
    encrypt_public_impl::encrypt_public_impl(std::string filename_pk, std::string filename_sk)
      : gr::block("encrypt_public",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        d_filename_pk = filename_pk;
        d_filename_sk = filename_sk;
        d_pk = new unsigned char[crypto_box_PUBLICKEYBYTES];
        d_sk = new unsigned char[crypto_box_SECRETKEYBYTES];
        
        // Load keys from files
        char c;
        
        std::ifstream file_sk(filename_sk.c_str());
        if(not(file_sk.is_open())) throw std::runtime_error("Secret-key file not found.");
        for(int k=0; k<crypto_box_SECRETKEYBYTES; k++){
            file_sk.get(c);
            d_sk[k] = c;
        }
        file_sk.close();
        
        std::ifstream file_pk(filename_pk.c_str());
        if(not(file_pk.is_open())) throw std::runtime_error("Public-key file not found.");
        for(int k=0; k<crypto_box_PUBLICKEYBYTES; k++){
            file_pk.get(c);
            d_pk[k] = c;
        }
        file_pk.close();
        
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
        
    }

    /*
     * Our virtual destructor.
     */
    encrypt_public_impl::~encrypt_public_impl()
    {
        delete[] d_pk;
        delete[] d_sk;
    }

  } /* namespace nacl */
} /* namespace gr */

