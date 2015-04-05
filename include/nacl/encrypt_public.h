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


#ifndef INCLUDED_NACL_ENCRYPT_PUBLIC_H
#define INCLUDED_NACL_ENCRYPT_PUBLIC_H

#include <nacl/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace nacl {

    /*!
     * \brief <+description of block+>
     * \ingroup nacl
     *
     */
    class NACL_API encrypt_public : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<encrypt_public> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of nacl::encrypt_public.
       *
       * To avoid accidental use of raw pointers, nacl::encrypt_public's
       * constructor is in a private implementation
       * class. nacl::encrypt_public::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string filename_pk, std::string filename_sk);
    };

  } // namespace nacl
} // namespace gr

#endif /* INCLUDED_NACL_ENCRYPT_PUBLIC_H */

