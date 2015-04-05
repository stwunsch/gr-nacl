/* -*- c++ -*- */

#define NACL_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "nacl_swig_doc.i"

%{
#include "nacl/encrypt_public.h"
%}


%include "nacl/encrypt_public.h"
GR_SWIG_BLOCK_MAGIC2(nacl, encrypt_public);
