#ifndef VOIDTYPES_H
#define VOIDTYPES_H

#include <limits.h>

#if sizeof(void*) == sizeof(unsigned int)
typedef int void_t;
typedef unsigned int uvoid_t;
#define VOID_T_MAX INT_MAX
#define UVOID_T_MAX UINT_MAX
#elif sizeof(void*) == sizeof(unsigned short)
typedef short void_t;
typedef unsigned short uvoid_t;
#define VOID_T_MAX SHRT_MAX
#define UVOID_T_MAX USHRT_MAX
#elif sizeof(void*) == sizeof(unsigned long)
typedef long void_t;
typedef unsigned long uvoid_t;
#define VOID_T_MAX LONG_MAX
#define UVOID_T_MAX ULONG_MAX
#elif sizeof(void*) == sizeof(unsigned char)
typedef char void_t;
typedef unsigned char uvoid_t;
#define VOID_T_MAX CHAR_MAX
#define UVOID_T_MAX UCHAR_MAX
#endif

#if sizeof(void*) == sizeof(float)
typedef float fvoid_t;
#elif sizeof(void*) == sizeof(double)
typedef double fvoid_t;
#endif

#endif /* VOIDTYPES_H */
