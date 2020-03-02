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

#ifndef INCLUDED_NACL_GENERATE_KEYPAIR_IMPL_H
#define INCLUDED_NACL_GENERATE_KEYPAIR_IMPL_H

#include <nacl/generate_keypair.h>

namespace gr {
namespace nacl {

class generate_keypair_impl : public generate_keypair
{
private:
    // Nothing to declare in this block.

public:
    generate_keypair_impl(std::string filename_sk, std::string filename_pk);
    ~generate_keypair_impl();
};

} // namespace nacl
} // namespace gr

#endif /* INCLUDED_NACL_GENERATE_KEYPAIR_IMPL_H */
