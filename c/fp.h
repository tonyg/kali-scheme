#if defined(__alpha) || defined(__i386)
#define LITTLE_ENDIAN_IEEE_DOUBLE
#elif !(defined(sparc) || defined(__sgi) || defined(_IBMR2) || defined(hpux))
unknown machine type
#endif

#ifdef __alpha
#define UNSIGNED64 unsigned long
#else
#define UNSIGNED64 unsigned long long
#endif

#if defined(sparc) || defined(hpux)
#define PROTO(x) ()
#else
#define PROTO(x) x
#endif

#ifndef U32
#define U32 unsigned int
#endif

/* exponent stored + 1024, hidden bit to left of decimal point */
#define bias 1023
#define bitstoright 52
#define m1mask 0xf
#define hidden_bit 0x100000
#ifdef LITTLE_ENDIAN_IEEE_DOUBLE
struct dblflt {
    unsigned int m4: 16;
    unsigned int m3: 16;
    unsigned int m2: 16;
    unsigned int m1: 4;
    unsigned int e: 11;
    unsigned int s: 1;
};
#else
/* Big Endian IEEE Double Floats */
struct dblflt {
    unsigned int s: 1;
    unsigned int e: 11;
    unsigned int m1: 4;
    unsigned int m2: 16;
    unsigned int m3: 16;
    unsigned int m4: 16;
};
#endif
#define float_radix 2.147483648e9
