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
#include "crypt_tagged_stream_impl.h"

#include <sodium.h>

namespace gr {
  namespace nacl {

    crypt_tagged_stream::sptr
    crypt_tagged_stream::make(std::string key, std::string nonce, const std::string& len_key)
    {
      return gnuradio::get_initial_sptr
        (new crypt_tagged_stream_impl(key, nonce, len_key));
    }

    /*
     * The private constructor
     */
    crypt_tagged_stream_impl::crypt_tagged_stream_impl(std::string key, std::string nonce, const std::string& len_key)
      : gr::tagged_stream_block("crypt_tagged_stream",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(1, 1, sizeof(uint8_t)), len_key)
    {
        d_key = new unsigned char[crypto_stream_KEYBYTES];
        d_nonce = new unsigned char[crypto_stream_NONCEBYTES];
    }

    /*
     * Our virtual destructor.
     */
    crypt_tagged_stream_impl::~crypt_tagged_stream_impl()
    {
        delete[] d_key;
        delete[] d_nonce;
    }

    int
    crypt_tagged_stream_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = ninput_items[0];
      return noutput_items ;
    }

    int
    crypt_tagged_stream_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        const uint8_t *in = (const uint8_t *) input_items[0];
        uint8_t *out = (uint8_t *) output_items[0];

        // Encrypt single input buffer of tagged stream size
        int status;
        if(ninput_items[0]!=0 && noutput_items>=ninput_items[0]){
            status = crypto_stream_xor(out, in, ninput_items[0], d_nonce, d_key);
        }
        
         // Tell runtime system how many output items we produced.
        if(status==0){
            return ninput_items[0];
        }
        else{
            return 0;
        }
    }

  } /* namespace nacl */
} /* namespace gr */

