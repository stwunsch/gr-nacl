/* -*- c++ -*- */
/*
 * Copyright 2020 Stefan Wunsch.
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
#include "encrypt_secret_impl.h"

#include <fstream>
#include "sodium.h"

namespace gr {
  namespace nacl {

    encrypt_secret::sptr
    encrypt_secret::make(std::string filename_key)
    {
      return gnuradio::get_initial_sptr
        (new encrypt_secret_impl(filename_key));
    }

    /*
     * The private constructor
     */
    encrypt_secret_impl::encrypt_secret_impl(std::string filename_key)
      : gr::block("encrypt_secret",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
        d_filename_key = filename_key;
        d_key = new unsigned char[crypto_secretbox_KEYBYTES];

        // Load key from file
        char c;

        std::ifstream file_key(filename_key.c_str());
        if(!(file_key.is_open())) throw std::runtime_error("Key file not found.");
        for(int k=0; k<crypto_secretbox_KEYBYTES; k++){
            file_key.get(c);
            d_key[k] = c;
        }
        file_key.close();

        // Register input message port
        d_port_id_in = pmt::mp("Msg clear");
        message_port_register_in(d_port_id_in);
        set_msg_handler(d_port_id_in, boost::bind(&encrypt_secret_impl::handle_msg, this, _1));

        // Register output message port
        d_port_id_out = pmt::mp("Msg encrypted");
        message_port_register_out(d_port_id_out);
    }

    void
    encrypt_secret_impl::handle_msg(pmt::pmt_t msg){
        size_t msg_size = pmt::length(msg);

        // check for unencrypted (clear) message tagged with symbol 'msg_clear'
        std::vector<uint8_t> data;
        for(int k=0; k<msg_size; k++){
            if(pmt::symbol_to_string(pmt::nth(0,pmt::nth(k,msg)))=="msg_clear"){
                if(pmt::is_u8vector(pmt::nth(1,pmt::nth(k,msg)))){
                    data = pmt::u8vector_elements(pmt::nth(1,pmt::nth(k,msg)));
                }
            }
        }

        // encrypt data
        if(data.size()!=0){
            // generate nonce
            unsigned char nonce[crypto_secretbox_NONCEBYTES];
            randombytes_buf(nonce, sizeof(nonce));

            // encrypt message
            __GR_VLA(unsigned char, data_char, data.size());
            size_t data_char_sz = (sizeof(unsigned char) * data.size());
            for(int k=0; k<data.size(); k++) data_char[k] = (unsigned char)data[k];
            size_t ciphertext_len = crypto_secretbox_MACBYTES + data_char_sz;
            __GR_VLA(unsigned char, ciphertext, ciphertext_len);
            crypto_secretbox_easy(ciphertext, data_char, data_char_sz, nonce, d_key);

            // repack msg with symbol 'msg_encrypted' and nonce with symbol 'nonce'
            std::vector<uint8_t> msg_encrypted; msg_encrypted.resize(ciphertext_len);
            for(int k=0; k<ciphertext_len; k++) msg_encrypted[k] = (uint8_t)ciphertext[k];

            std::vector<uint8_t> nonce_vec; nonce_vec.resize(crypto_secretbox_NONCEBYTES);
            for(int k=0; k<crypto_secretbox_NONCEBYTES; k++) nonce_vec[k] = (uint8_t)nonce[k];

            pmt::pmt_t msg_out_nonce = pmt::list2(pmt::string_to_symbol("nonce"),pmt::init_u8vector(nonce_vec.size(),nonce_vec));
            pmt::pmt_t msg_out_data = pmt::list2(pmt::string_to_symbol("msg_encrypted"),pmt::init_u8vector(msg_encrypted.size(),msg_encrypted));

            // publish msg
            message_port_pub(d_port_id_out,pmt::list2(msg_out_nonce,msg_out_data));
        }
    }

    /*
     * Our virtual destructor.
     */
    encrypt_secret_impl::~encrypt_secret_impl()
    {
        delete[] d_key;
    }

  } /* namespace nacl */
} /* namespace gr */

