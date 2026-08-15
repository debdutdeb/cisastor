#ifndef __MACRO_H__
#define __MACRO_H__

#define cast(a, b) ((a)b)

#define null NULL

#define byte char

// intentionally left without formatting allowed, if need formatting, just log
// as is.
#define unreachable(str)                                                       \
  do {                                                                         \
    fprintf(stderr, "unrechable block hit, diagnostic message: %s\n", str);    \
    abort();                                                                   \
  } while (0)

#endif
