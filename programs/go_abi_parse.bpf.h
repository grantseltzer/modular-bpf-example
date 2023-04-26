/*
    Definitions of constants representing Go types, and their sizes/alignments.
    These are meant to be used in bpf code to parse the cookiefilter set in
    userspace (seet pkg/cookiefilter). Once parsed, bpf code can then accurately
    parse registers and the stack to send requested information over the ringbuffer
    back to userspace.
*/

#include<stdint.h>

#define BOOL 1
#define UINT8 2
#define INT8 3 
#define UINT16 4
#define INT16 5
#define UINT32 6
#define INT32 7
#define UINT64 8
#define INT64 9
#define FLOAT32 10
#define FLOAT64 11
#define COMPLEX64 12
#define COMPLEX128 13
#define POINTER 14
#define STRING 15

#define BOOL_SIZE_64 1
#define BOOL_ALIGN_64 1
#define UINT8_SIZE_64 1
#define UINT8_ALIGN_64 1
#define INT8_SIZE_64 1
#define INT8_ALIGN_64 1

#define UINT16_SIZE_64 2
#define UINT16_ALIGN_64 2
#define INT16_SIZE_64 2
#define INT16_ALIGN_64 2

#define UINT32_SIZE_64 4
#define UINT32_ALIGN_64 4
#define INT32_SIZE_64 4
#define INT32_ALIGN_64 4

#define UINT64_SIZE_64 8
#define UINT64_ALIGN_64 8
#define INT64_SIZE_64 8
#define INT64_ALIGN_64 8

#define UINT_SIZE_64 8
#define UINT_ALIGN_64 8
#define INT_SIZE_64 8
#define INT_ALIGN_64 8

#define FLOAT32_SIZE_64 4
#define FLOAT32_ALIGN_64 4

#define FLOAT64_SIZE_64 8
#define FLOAT64_ALIGN_64 8

#define COMPLEX64_SIZE_64 8
#define COMPLEX64_ALIGN_64 4

#define COMPLEX128_SIZE_64 16
#define COMPLEX128_ALIGN_64 8

#define POINTER_SIZE_64 8
#define POINTER_ALIGN_64 8

/////

#define NUM_RETURNS_INDEX  1
#define NUM_PARAMS_INDEX 2

////

////

#if defined(__TARGET_ARCH_x86)
#else


void parse_cookie_filter(struct pt_regs *ctx, char *cookiefilter) {
   
    // Length of integer and floating point register sequences on arm64
    const uint32_t NI = 1;
    const uint32_t NFP = 1;

    // Index of next integer and floating-pointer registers
    uint32_t I = 0;
    uint32_t FP = 0;

    uint8_t NumReturns = 0;
    uint8_t NumParams = 0; 

    char* tmp = cookiefilter;
    bool parsing = true;
    uint32_t cookiefilter_index = 0;
    while (parsing) {
        // null check
        if (!tmp) {
            parsing = false;
            break;
        }
        // end of filter check
        if (*tmp == '\0') {
            parsing = false;
            break;
        }

        if (cookiefilter_index == NUM_RETURNS_INDEX) {
            NumReturns = (uint32_t)*tmp;
        }

        if (cookiefilter_index == NUM_PARAMS_INDEX) {
            NumParams = (uint32_t)*tmp;
        }

        /*
            TODO: parse params and returns
        */


        cookiefilter_index++; // can also subtract `tmp` from `cookiefilter`
        tmp++;
    }
}

#endif
