/* -*- c++ -*- */

#define NACL_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "nacl_swig_doc.i"

%{
#include "nacl/generate_keypair.h"
%}

%include "nacl/generate_keypair.h"
GR_SWIG_BLOCK_MAGIC2(nacl, generate_keypair);
