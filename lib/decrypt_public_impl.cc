/* -*- c++ -*- */
/* /* 
 *  * Copyright 2015 Stefan Wunsch
 *  * 
 *  * This is free software; you can redistribute it and/or modify
 *  * it under the terms of the GNU General Public License as published by
 *  * the Free Software Foundation; either version 3, or (at your option)
 *  * any later version.
 *  * 
 *  * This software is distributed in the hope that it will be useful,
 *  * but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  * GNU General Public License for more details.
 *  * 
 *  * You should have received a copy of the GNU General Public License
 *  * along with this software; see the file COPYING.  If not, write to
 *  * the Free Software Foundation, Inc., 51 Franklin Street,
 *  * Boston, MA 02110-1301, USA.
 *  */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "decrypt_public_impl.h"

#include <fstream>
#include <sodium.h>

namespace gr {
  namespace nacl {

    decrypt_public::sptr
    decrypt_public::make(std::string filename_pk, std::string filename_sk)
    {
      return gnuradio::get_initial_sptr
        (new decrypt_public_impl(filename_pk, filename_sk));
    }

    /*
     * The private constructor
     */
    decrypt_public_impl::decrypt_public_impl(std::string filename_pk, std::string filename_sk)
      : gr::block("decrypt_public",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
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
        set_msg_handler(d_port_id_in, boost::bind(&decrypt_public_impl::handle_msg, this, _1));
        
        // Register output message port
        d_port_id_out = pmt::mp("Msg out");
        message_port_register_out(d_port_id_out);
    }

    /*
     * Our virtual destructor.
     */
    decrypt_public_impl::~decrypt_public_impl()
    {
        delete[] d_pk;
        delete[] d_sk;
    }
    
    void
    decrypt_public_impl::handle_msg(pmt::pmt_t msg){
        std::cout << "// DEBUG DECRYPTION" << std::endl;
        
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
            unsigned char data_char[data.size()];
            unsigned char nonce_char[nonce.size()];
            for(int k=0; k<data.size(); k++) data_char[k] = (unsigned char)data[k];
            for(int k=0; k<nonce.size(); k++) nonce_char[k] = (unsigned char)nonce[k];
            size_t msg_len = sizeof(data_char)-crypto_box_MACBYTES;
            unsigned char msg_decrypted[msg_len];
            
            std::cout << "DEBUG ENCRYPTED MESSAGE 2: ";
            for(int k=0; k<sizeof(data_char); k++) std::cout << (int)data_char[k] << " ";
            std::cout << std::endl;
            
            std::cout << "DEBUG NONCE CHAR 2: ";
            for(int k=0; k<sizeof(nonce_char); k++) std::cout << (int)nonce_char[k] << " ";
            std::cout << std::endl;
            
            std::cout << "DEBUG NONCE VEC 2: ";
            for(int k=0; k<nonce.size(); k++) std::cout << (int)nonce[k] << " ";
            std::cout << std::endl;
            
            std::cout << "DEBUG PUBLIC KEY DECRYPTION: ";
            for(int k=0; k<sizeof(d_pk); k++) std::cout << (int)d_pk[k] << " ";
            std::cout << std::endl;
            
            std::cout << "DEBUG SECRET KEY DECRYPTION: ";
            for(int k=0; k<sizeof(d_sk); k++) std::cout << (int)d_sk[k] << " ";
            std::cout << std::endl;
            
            int msg_status = crypto_box_open_easy(msg_decrypted, data_char, sizeof(data_char), nonce_char, d_pk, d_sk);
            
            // check whether msg is successfully decrypted
            if(msg_status==0){
                // repack msg with symbol 'msg_decrypted'
                std::vector<uint8_t> msg_decrypted_vec; msg_decrypted_vec.resize(msg_len);
                for(int k=0; k<msg_len; k++) msg_decrypted_vec[k] = (uint8_t)msg_decrypted[k];
                
                std::cout << "DEBUG DECRYPTED MESSAGE: ";
                for(int k=0; k<msg_len; k++) std::cout << (int)msg_decrypted_vec[k] << " ";
                std::cout << std::endl;
                
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
    
  } /* namespace nacl */
} /* namespace gr */

