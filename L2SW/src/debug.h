#ifndef DEBUG_H
#define DEBUG_H

//#define _NO_DEBUG

#ifdef _NO_DEBUG
#define _printf(fmt, ...)
#define _hexdump(p, c)
#define _dump_mac_table(tbl)

#else
#include <stdio.h>

#define _printf(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)
#define _hexdump(p, c) hexdump(p, c)
#define _dump_mac_table(tbl) dump_mac_table(tbl)


#endif
#endif
