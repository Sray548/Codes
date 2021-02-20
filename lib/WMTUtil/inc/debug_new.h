/*
 * debug_new.h used for memory leak debug.
 */

#ifndef _DEBUG_NEW_H
#define _DEBUG_NEW_H

#define _DEBUG_NEW_ENABLE 0

#if _DEBUG_NEW_ENABLE


#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif  // __cplusplus

/* Prototypes */
int    check_leaks(int flag);   //flag: 0 print last changed, 1 print all, -1 only check the endmark
void*  debug_malloc(size_t size, const char* file, int line);
void*  debug_calloc(size_t size, const char* file, int line);
void*  debug_realloc(void* pointer, size_t size, const char* file, int line);
void   debug_free(void* pointer, const char* file, int line);
char * debug_strdup(const char * str, const char* file, int line);

#ifndef INLCUDE_FROM_DEBUG_NEW_CPP
    #define malloc(s) debug_malloc(s, __FILE__, __LINE__)
    #define calloc(s, n) debug_calloc((s)*(n), __FILE__, __LINE__)
    #define realloc(s,n) debug_realloc(s, n, __FILE__, __LINE__)
    #define free(p) debug_free(p, __FILE__, __LINE__)
    #define strdup(p) debug_strdup(p, __FILE__, __LINE__)
#endif

/* Control flags */
extern int  new_verbose_flag;	// default to 0(false): no verbose information
extern int  new_autocheck_flag;	// default to 1(true): call check_leaks() on exit

#ifdef __cplusplus
}
void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);
void operator delete(void* pointer, const char* file, int line);
void operator delete[](void* pointer, const char* file, int line);
 
#ifndef INLCUDE_FROM_DEBUG_NEW_CPP
    #define new DEBUG_NEW
    #define DEBUG_NEW new(__FILE__, __LINE__)
#endif
#endif  // __cplusplus

#else
    #define check_leaks(unused) 
#endif // _DEBUG_NEW_ENABLE

#endif // _DEBUG_NEW_H
