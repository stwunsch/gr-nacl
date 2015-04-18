gr-nacl
========
GNU Radio module for data encryption using NaCl library  

**Features**  
The gr-nacl module for GNU Radio provides functionality from the NaCl library implemented with the fork libsodium. This contains public-key and secret-key encryption. The difference is explained, e.g., on Wikipedia [0]. The implementation is based on encryption of messages, which are passed in GNU Radio via the message system. Check out the GNU Radio documentation for further information [1]. Furthermore, a byte stream encryption method via tagged streams is implemented.

The functionality can be tested with the example flowgraphs for GNU Radio Companion at the subfolder examples/ or directly with the provided test-cases for ctest.

**Install guide**  
Change to any folder in your home directory and enter following commands in your terminal.

`git clone https://github.com/stwunsch/gr-nacl.git` // clone this repository  
`cd gr-nacl/`  
`mkdir build` // make build folder  
`cd build/`  
`cmake ../` // build makefiles  
`make` // build toolbox  
`ctest` // run tests, check if all have passed, the option -V provides an extended output  
`sudo make install` // install toolbox

**Development platform**  
GNU Radio 3.7.6.1  

**Dependencies**  
The NaCl (pronounced 'salt') crypto library [2] by Daniel J. Bernstein, Tanja Lange and Peter Schwabe has a well maintained fork called 'libsodium' [3]. Follow the instructions to build and install it.

`git clone https://github.com/jedisct1/libsodium.git` // clone libsodium  
`cd libsodium`  
`./autogen.sh` // build libsodium  
`./configure`  
`make`  
`sudo make install` // install libsodium

**Contact**  
Stefan Wunsch  
stefan.wunsch[at]student.kit.edu

**Links**  
[0] https://en.wikipedia.org/wiki/Public-key_cryptography, https://en.wikipedia.org/wiki/Symmetric-key_algorithm  
[1] http://gnuradio.org/doc/doxygen/, http://gnuradio.org/doc/doxygen/page_msg_passing.html, http://gnuradio.org/doc/doxygen/page_tagged_stream_blocks.html  
[2] http://nacl.cr.yp.to/  
[3] http://doc.libsodium.org/, https://github.com/jedisct1/libsodium
