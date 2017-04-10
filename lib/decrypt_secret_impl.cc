/* -*- c++ -*- */
/* Copyright 2015 Stefan Wunsch
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
#include "decrypt_secret_impl.h"

#include <fstream>
#include "sodium.h"

namespace gr {
  namespace nacl {

    decrypt_secret::sptr
    decrypt_secret::make(std::string filename_key)
    {
      return gnuradio::get_initial_sptr
        (new decrypt_secret_impl(filename_key));
    }

    /*
     * The private constructor
     */
    decrypt_secret_impl::decrypt_secret_impl(std::string filename_key)
      : gr::block("decrypt_secret",
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
        d_port_id_in = pmt::mp("Msg encrypted");
        message_port_register_in(d_port_id_in);
        set_msg_handler(d_port_id_in, boost::bind(&decrypt_secret_impl::handle_msg, this, _1));
        
        // Register output message port
        d_port_id_out = pmt::mp("Msg decrypted");
        message_port_register_out(d_port_id_out);
    }
    
    void
    decrypt_secret_impl::handle_msg(pmt::pmt_t msg){
        size_t msg_size = pmt::length(msg);
        
        // check for encrypted message tagged with symbol 'msg_encrypted' and nonce tagged with 'nonce'
        std::vector<uint8_t> data, nonce;
        bool msg_encrypted_found = false;
        bool nonce_found = false;
        for(int k=0; k<msg_size; k++){
            if(pmt::symbol_to_string(pmt::nth(0,pmt::nth(k,msg)))=="msg_encrypted"){
                if(pmt::is_u8vector(pmt::nth(1,pmt::nth(k,msg)))){
                    data = pmt::u8vector_elements(pmt::nth(1,pmt::nth(k,msg)));
                    msg_encrypted_found = true;
                }
            }
            if(pmt::symbol_to_string(pmt::nth(0,pmt::nth(k,msg)))=="nonce"){
                if(pmt::is_u8vector(pmt::nth(1,pmt::nth(k,msg)))){
                    nonce = pmt::u8vector_elements(pmt::nth(1,pmt::nth(k,msg)));
                    nonce_found = true;
                }
            }
        }
        
        // encrypt data
        if(msg_encrypted_found&&nonce_found){
            // decrypt message
            __GR_VLA(unsigned char, data_char, data.size());
            __GR_VLA(unsigned char, nonce_char, nonce.size());
            size_t data_char_sz = (sizeof(unsigned char) * data.size());
            for(int k=0; k<data.size(); k++) data_char[k] = (unsigned char)data[k];
            for(int k=0; k<nonce.size(); k++) nonce_char[k] = (unsigned char)nonce[k];
            size_t msg_len = data_char_sz -crypto_secretbox_MACBYTES;
            __GR_VLA(unsigned char, msg_decrypted, msg_len);
            
            int msg_status = crypto_secretbox_open_easy(msg_decrypted, data_char, data_char_sz, nonce_char, d_key);
            
            // check whether msg is successfully decrypted
            if(msg_status==0){
                // repack msg with symbol 'msg_decrypted'
                std::vector<uint8_t> msg_decrypted_vec; msg_decrypted_vec.resize(msg_len);
                for(int k=0; k<msg_len; k++) msg_decrypted_vec[k] = (uint8_t)msg_decrypted[k];
                
                pmt::pmt_t msg_out = pmt::list2(pmt::string_to_symbol("msg_decrypted"),pmt::init_u8vector(msg_decrypted_vec.size(),msg_decrypted_vec));
                
                // publish msg
                message_port_pub(d_port_id_out,pmt::list1(msg_out));
            }
            else{
                std::cout << "Failed to decrypt message." << std::endl;
                std::cout << "Nonce found: " << nonce_found << std::endl;
                std::cout << "Encrypted message found: " << msg_encrypted_found << std::endl;
                std::cout << "Message decryption status: " << msg_status << std::endl;
            }
        }
    }

    /*
     * Our virtual destructor.
     */
    decrypt_secret_impl::~decrypt_secret_impl()
    {
        delete[] d_key;
    }

  } /* namespace nacl */
} /* namespace gr */

