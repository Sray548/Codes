/*
 * Copyright 2014 WonderMedia Technologies, Inc. All Rights Reserved.
 *
 * This PROPRIETARY SOFTWARE is the property of WonderMedia Technologies, Inc.
 * and may contain trade secrets and/or other confidential information of
 * WonderMedia Technologies, Inc. This file shall not be disclosed to any third party,
 * in whole or in part, without prior written consent of WonderMedia.
 *
 * THIS PROPRIETARY SOFTWARE AND ANY RELATED DOCUMENTATION ARE PROVIDED AS IS,
 * WITH ALL FAULTS, AND WITHOUT WARRANTY OF ANY KIND EITHER EXPRESS OR IMPLIED,
 * AND WonderMedia TECHNOLOGIES, INC. DISCLAIMS ALL EXPRESS OR IMPLIED WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 */

#ifndef WMT_WASSERT_H_INCLUDED
#define WMT_WASSERT_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "WLogger.h"

#ifdef ANDROID
    #define WASSERT_PRINTF(...)  wlogOutput(NULL, WLOGLEVEL_ERROR, __VA_ARGS__)
#else
    #define WASSERT_PRINTF  printf
#endif


/**
 * A C++ based smart assert. 
 * Usage: WASSERT(a == 2)(a); 
 *   if  a != 2, not just report the line and text, it also prints a's value.
 */
class WASSERT
{
public:
    WASSERT(const char * e) : WASSERT_A(*this), WASSERT_B(*this){
        expr = e;
    }

    ~WASSERT() {
        PANIC("WAssert Error");
        ::abort();
    }

    WASSERT& WASSERT_A;
    WASSERT& WASSERT_B;
    const char * expr;
    
    WASSERT& printContext(const char * file, int line) {
        WASSERT_PRINTF("\n\n\n\n\n*****************************************************");
        WASSERT_PRINTF("VIA Assertion failed in %s:%d\n", file, line);
        WASSERT_PRINTF("Expression: %s\n", expr);
        return *this;
    }

    WASSERT& printName(const char* eval) {
        WASSERT_PRINTF("   Values: %s = ", eval);
        return *this;
    }

    template<class T>
    WASSERT& printVal(const T& v) {
        *this << v;
        return *this;
    }

    WASSERT& msg(const char* msg) {
        WASSERT_PRINTF("   %s\n", msg);
        return *this;
    }
};


#define WASSERT_A(x) WASSERT_OP(x,B)
#define WASSERT_B(x) WASSERT_OP(x,A)
#define WASSERT_OP(x,next) WASSERT_A.printName(#x).printVal(x).WASSERT_##next


#define WASSERT(expr) if((expr));else WASSERT(#expr).printContext(__FILE__,__LINE__).WASSERT_A

inline WASSERT& operator<<(WASSERT& a, char v) {
    WASSERT_PRINTF("%c(c)\n", v);
    return a;
}

inline WASSERT& operator<<(WASSERT& a, unsigned char v) {
    WASSERT_PRINTF("0x%X(uc)\n", v);
    return a;
}


inline WASSERT& operator<<(WASSERT& a, short v) {
    WASSERT_PRINTF("%d(s)\n", v);
    return a;
}


inline WASSERT& operator<<(WASSERT& a, unsigned short v) {
    WASSERT_PRINTF("0x%X(us)\n", v);
    return a;
}

inline WASSERT& operator<<(WASSERT& a, int v) {
    WASSERT_PRINTF("%d(i)\n", v);
    return a;
}

inline WASSERT& operator<<(WASSERT& a, unsigned int v) {
    WASSERT_PRINTF("0x%X(ui)\n", v);
    return a;
}


inline WASSERT& operator<<(WASSERT& a, long	v) {
    WASSERT_PRINTF("%ld(l)\n", v);
    return a;
}

inline WASSERT& operator<<(WASSERT& a, unsigned long v) {
    WASSERT_PRINTF("%ld(ul)\n", v);
    return a;
}

inline WASSERT& operator<<(WASSERT& a, bool v) {
    WASSERT_PRINTF("%s(b)\n", v ? "true" : "false");
    return a;
}

inline WASSERT& operator<<(WASSERT& a, const void * v) {
    WASSERT_PRINTF("%p(p)\n", v);
    return a;
}

inline WASSERT& operator<<(WASSERT& a, const char * v) {
    WASSERT_PRINTF("%s(s)\n", v);
    return a;
}

inline WASSERT& operator<<(WASSERT& a, long long  v) {
    WASSERT_PRINTF("%lld(lld)\n", v);
    return a;
}




#endif  /* WMT_WASSERT_H_INCLUDED */




