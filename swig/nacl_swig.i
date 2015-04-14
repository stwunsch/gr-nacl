/* -*- c++ -*- */

#define NACL_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "nacl_swig_doc.i"

%{
#include "nacl/encrypt_public.h"
#include "nacl/generate_keypair.h"
#include "nacl/decrypt_public.h"
#include "nacl/generate_key.h"
%}


%include "nacl/encrypt_public.h"
GR_SWIG_BLOCK_MAGIC2(nacl, encrypt_public);
%include "nacl/generate_keypair.h"
GR_SWIG_BLOCK_MAGIC2(nacl, generate_keypair);
%include "nacl/decrypt_public.h"
GR_SWIG_BLOCK_MAGIC2(nacl, decrypt_public);
%include "nacl/generate_key.h"
GR_SWIG_BLOCK_MAGIC2(nacl, generate_key);
