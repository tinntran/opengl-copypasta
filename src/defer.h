#ifndef DEFER_H_
#define DEFER_H_

#include <stdio.h>

#define return_defer(res) do { result = res; goto defer; } while(0)

#define MLOG(cat, fmt, ...) do { \
  printf("{%s} ", cat); \
  printf(fmt, __VA_ARGS__); \
} while(0)

#endif //DEFER_H_
