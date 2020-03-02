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

#include "crypt_tagged_stream_impl.h"
#include <gnuradio/io_signature.h>

#include <sodium.h>

namespace gr {
namespace nacl {

crypt_tagged_stream::sptr crypt_tagged_stream::make(std::string key,
                                                    std::string nonce,
                                                    bool rotate_nonce,
                                                    const std::string& len_key)
{
    return gnuradio::get_initial_sptr(
        new crypt_tagged_stream_impl(key, nonce, rotate_nonce, len_key));
}

/*
 * The private constructor
 */
crypt_tagged_stream_impl::crypt_tagged_stream_impl(std::string key,
                                                   std::string nonce,
                                                   bool rotate_nonce,
                                                   const std::string& len_key)
    : gr::tagged_stream_block("crypt_tagged_stream",
                              gr::io_signature::make(1, 1, sizeof(uint8_t)),
                              gr::io_signature::make(1, 1, sizeof(uint8_t)),
                              len_key)
{
    d_key = new uint8_t[crypto_stream_KEYBYTES];
    d_nonce = new uint8_t[crypto_stream_NONCEBYTES];
    d_rotate_nonce = rotate_nonce;

    // Error-handling nonce and key sizes
    if (key.size() != crypto_stream_KEYBYTES) {
        std::cout << "Key size (needed/given): " << crypto_stream_KEYBYTES << "/"
                  << key.size() << std::endl;
        throw std::runtime_error("Wrong key size.");
    }
    if (nonce.size() != crypto_stream_NONCEBYTES) {
        std::cout << "Nonce size (needed/given): " << crypto_stream_NONCEBYTES << "/"
                  << nonce.size() << std::endl;
        throw std::runtime_error("Wrong nonce size.");
    }

    // Copy nonce and key from string to unsigend char array
    for (int k = 0; k < crypto_stream_KEYBYTES; k++)
        d_key[k] = (uint8_t)key[k];
    for (int k = 0; k < crypto_stream_NONCEBYTES; k++)
        d_nonce[k] = (uint8_t)nonce[k];
}

/*
 * Our virtual destructor.
 */
crypt_tagged_stream_impl::~crypt_tagged_stream_impl()
{
    delete[] d_key;
    delete[] d_nonce;
}

int crypt_tagged_stream_impl::calculate_output_stream_length(
    const gr_vector_int& ninput_items)
{
    int noutput_items = ninput_items[0];
    return noutput_items;
}

int crypt_tagged_stream_impl::work(int noutput_items,
                                   gr_vector_int& ninput_items,
                                   gr_vector_const_void_star& input_items,
                                   gr_vector_void_star& output_items)
{
    const uint8_t* in = (const uint8_t*)input_items[0];
    uint8_t* out = (uint8_t*)output_items[0];

    // Encrypt single input buffer of tagged stream size
    int status;
    if (ninput_items[0] != 0 && noutput_items >= ninput_items[0]) {
        status = crypto_stream_xor(out, in, ninput_items[0], d_nonce, d_key);
    }

    // Tell runtime system how many output items we produced.
    // FIXME: Status might be a garbage value depending on l. 94
    if (status == 0) {
        if (d_rotate_nonce) {
            // Store first bit
            d_nonce[0] = 0b00000000;
            unsigned char store_bit = 0b10000000 & d_nonce[0]; // FIXME: not final

            // Shift left char array and add stored bit at the end // FIXME: check this
            // implementation!
            for (int k = 0; k < crypto_stream_NONCEBYTES; k++)
                d_nonce[k] = d_nonce[k] << 1;
            if (store_bit == 0)
                d_nonce[crypto_stream_NONCEBYTES - 1] =
                    d_nonce[crypto_stream_NONCEBYTES - 1] & 0b00000000;
            else
                d_nonce[crypto_stream_NONCEBYTES - 1] =
                    d_nonce[crypto_stream_NONCEBYTES - 1] | 0b00000001;
        }

        return ninput_items[0];
    } else {
        return 0;
    }
}

} /* namespace nacl */
} /* namespace gr */
