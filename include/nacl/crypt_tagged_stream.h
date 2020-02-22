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

#ifndef INCLUDED_NACL_CRYPT_TAGGED_STREAM_H
#define INCLUDED_NACL_CRYPT_TAGGED_STREAM_H

#include <nacl/api.h>
#include <gnuradio/tagged_stream_block.h>

namespace gr {
  namespace nacl {

    /*!
     * \brief <+description of block+>
     * \ingroup nacl
     *
     */
    class NACL_API crypt_tagged_stream : virtual public gr::tagged_stream_block
    {
     public:
      typedef boost::shared_ptr<crypt_tagged_stream> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of nacl::crypt_tagged_stream.
       *
       * To avoid accidental use of raw pointers, nacl::crypt_tagged_stream's
       * constructor is in a private implementation
       * class. nacl::crypt_tagged_stream::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string key, std::string nonce, bool rotate_nonce=false, const std::string& len_key="packet_len");
    };

  } // namespace nacl
} // namespace gr

#endif /* INCLUDED_NACL_CRYPT_TAGGED_STREAM_H */

