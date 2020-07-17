#include "dlib.hpp"

/**
 * dump byte string
 */
void dlib::hexdump(uint8_t *p, int count) {
    int i, j;

    for (i = 0; i < count; i += 16) {
        printf("%04x : ", i);
        for (j = 0; j < 16 && i + j < count; j++) printf("%2.2x ", p[i + j]);
        for (; j < 16; j++) {
            printf("   ");
        }
        printf(": ");
        for (j = 0; j < 16 && i + j < count; j++) {
            char c = toascii(p[i + j]);
            printf("%c", isalnum(c) ? c : '.');
        }
        printf("\n");
    }
}

void dump_route_table(RouteTable &rtb) {
    // TODO: implement
}