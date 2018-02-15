#ifndef INCLUDED_DLIB
#define INCLUDED_DLIB

#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <cctype>

//debug functions

namespace dlib{
    void hexdump(uint8_t *p, int count);
}

#endif
