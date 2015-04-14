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
#include "generate_keypair_impl.h"

#include <fstream>
#include "crypto_box.h"

namespace gr {
  namespace nacl {

    generate_keypair::sptr
    generate_keypair::make(std::string filename_sk, std::string filename_pk)
    {
      return gnuradio::get_initial_sptr
        (new generate_keypair_impl(filename_sk, filename_pk));
    }

    /*
     * The private constructor
     */
    generate_keypair_impl::generate_keypair_impl(std::string filename_sk, std::string filename_pk)
      : gr::block("generate_keypair",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
        unsigned char *pk = new unsigned char[crypto_box_PUBLICKEYBYTES];
        unsigned char *sk = new unsigned char[crypto_box_SECRETKEYBYTES];
        
        // generate keypair
        crypto_box_keypair(pk,sk);
        std::cout << "Keypair [" << filename_sk << ", " << filename_pk << "] generated successfully." << std::endl;
        
        // save keys to files
        std::ofstream file_sk(filename_sk.c_str());
        for(int k=0;k<crypto_box_SECRETKEYBYTES; k++) file_sk << sk[k];
        file_sk.close();
        
        std::ofstream file_pk(filename_pk.c_str());
        for(int k=0;k<crypto_box_PUBLICKEYBYTES; k++) file_pk << pk[k];
        file_pk.close();
        
        std::cout << "Keypair [" << filename_sk << ", " << filename_pk << "] saved to file." << std::endl;
        
        // clean-up
        delete[] pk;
        delete[] sk;
    }

    /*
     * Our virtual destructor.
     */
    generate_keypair_impl::~generate_keypair_impl()
    {
    }

  } /* namespace nacl */
} /* namespace gr */

