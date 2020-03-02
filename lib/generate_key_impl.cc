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

#include "generate_key_impl.h"
#include <gnuradio/io_signature.h>

#include <sodium.h>
#include <fstream>

namespace gr {
namespace nacl {

generate_key::sptr generate_key::make(std::string filename_key)
{
    return gnuradio::get_initial_sptr(new generate_key_impl(filename_key));
}

/*
 * The private constructor
 */
generate_key_impl::generate_key_impl(std::string filename_key)
    : gr::block("generate_key",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    unsigned char* key = new unsigned char[crypto_secretbox_KEYBYTES];

    // generate keypair
    randombytes_buf(key, sizeof(key));
    std::cout << "Key " << filename_key << " generated successfully." << std::endl;

    // save keys to files
    std::ofstream file_key(filename_key.c_str());
    for (int k = 0; k < crypto_secretbox_KEYBYTES; k++)
        file_key << key[k];
    file_key.close();

    std::cout << "Key " << filename_key << " saved to file." << std::endl;

    // clean-up
    delete[] key;
}

/*
 * Our virtual destructor.
 */
generate_key_impl::~generate_key_impl() {}

} /* namespace nacl */
} /* namespace gr */
