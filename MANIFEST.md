title: gr-nacl
brief: GNU Radio module for data encryption using NaCl library
tags:
  - encryption
author:
  - Stefan Wunsch <stefan.wunsch@student.kit.edu>
copyright_owner:
  - Stefan Wunsch
dependencies:
  - gnuradio 3.7.6.1
  - libsodium
repo: https://github.com/stwunsch/gr-nacl
---

The gr-nacl module for GNU Radio provides functionality from the NaCl crypto library implemented with the fork libsodium. This contains public-key and secret-key encryption. The implementation is based on encryption of messages, which are passed in GNU Radio via the message system. Furthermore, a byte stream encryption method via tagged streams is implemented.
