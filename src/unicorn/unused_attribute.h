#ifndef UNICORN_UNUSED_ATTRIBUTE_H
#define UNICORN_UNUSED_ATTRIBUTE_H


#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif


#endif
