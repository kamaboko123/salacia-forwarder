#ifndef INCLUDED_DLIB
#define INCLUDED_DLIB

#include <stdio.h>
#include <stdlib.h>

#include <cctype>
#include <cstdint>

#include "RouteTable.hpp"

// debug functions

namespace dlib {
    void hexdump(uint8_t *p, int count);
    void dump_route_table(RouteTable &rtb);
}  // namespace dlib
#endif
