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

#ifndef INCLUDED_NACL_CRYPT_TAGGED_STREAM_IMPL_H
#define INCLUDED_NACL_CRYPT_TAGGED_STREAM_IMPL_H

#include <nacl/crypt_tagged_stream.h>

namespace gr {
  namespace nacl {

    class crypt_tagged_stream_impl : public crypt_tagged_stream
    {
     private:
      // Nothing to declare in this block.

     protected:
      int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
      crypt_tagged_stream_impl(std::string key, std::string nonce, bool rotate_nonce, const std::string& len_key);
      ~crypt_tagged_stream_impl();

      uint8_t* d_key;
      uint8_t* d_nonce;
      bool d_rotate_nonce;

      // Where all the action really happens
      int work(int noutput_items,
            gr_vector_int &ninput_items,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items);
    };

  } // namespace nacl
} // namespace gr

#endif /* INCLUDED_NACL_CRYPT_TAGGED_STREAM_IMPL_H */

