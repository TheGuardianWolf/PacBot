#ifndef VOIDTYPES_H
#define VOIDTYPES_H

#if sizeof(void*) == sizeof(unsigned int)
typedef int void_t;
typedef unsigned int uvoid_t;
#elif sizeof(void*) == sizeof(unsigned long)
typedef long void_t;
typedef unsigned long uvoid_t;
#elif sizeof(void*) == sizeof(unsigned char)
typedef char void_t;
typedef unsigned char uvoid_t;
#endif

#if sizeof(void*) == sizeof(float)
typedef float fvoid_t;
#elif sizeof(void*) == sizeof(double)
typedef double fvoid_t;
#endif

#endif /* VOIDTYPES_H */
