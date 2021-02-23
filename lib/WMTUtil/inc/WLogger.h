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

#ifndef WMT_WLOGGER_HPP_INCLUDED
#define WMT_WLOGGER_HPP_INCLUDED



#include <stdarg.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * Usage: just like ALOGX in android, we choose WLOGX, the first arg is a wlogger object.
  
  The reason to use WLOGX is we want to control the log level at runtime, not just compile time.
  Each WLogger object has a fixed name and a level, you can control its log level seperately.
  
  Get a WLogger instance by its name:
        static WLogger * tag1Log = wlogGet("tag1");
  
  Set the log level by
        tag1Log->level = WLOGLEVEL_WARN; //only WLOGW and WLOGE can output
        or
        call wlogSetSetting(...) function to change it.
  for example in your main() function, call
        wlogSetSetting(argv[1])
  to change the log level dynamically.
  
  Output the log message:
        WLOGE(tag1Log, "something is wrong!");
  
  
  NOTE: to make WLOGVV works, you should
    #define WLOG_HARDCODE_LEVEL WLOGLEVEL_VV 
  before include this file.
  
  For C++ project, you can get the instance in the beginning of source file and use it later.
  -------mycpp.cpp ------------
  
    static WLogger * myLog = wlogGet("tag1");
    void foo() {
        WLOGD(myLog, "debug...");
    }
  
  For C project, you should defined all WLogger objects in a header file and init
  it in the main function. then all rest of your source code can use those WLogger objects.
  
  -------logobject.h ------------
  extern WLogger * tag1Logger;
  extern WLogger * tag2Logger;
  ...
  
  ------- main.c -----------------
  int main(int argc, char * argv[]) {
    //init all WLogger objects
  
    tag1Logger = wlogGet("tag1");
    tag2Logger = wlogGet("tag2");
    ...
    if( argc > 1) {
        wlogSetSetting(argv[1]);
    }
    WLOGD(tag1Logger, "debug message");
  
    }
  
 ---------- other.c ------------------ 
 #include "logboject.h" 
  
 void foo(void) { 
    WLOGD( tag1Logger, "debug message");
 } 
 */

#define WLOGLEVEL_ERROR    1
#define WLOGLEVEL_WARN     2
#define WLOGLEVEL_INFO     3
#define WLOGLEVEL_DEBUG    4
#define WLOGLEVEL_VERBOSE  5        //verbose
#define WLOGLEVEL_VV       6        //very verbose, default is not turn on


/**
 * if you want to enable WLOGVV, 
 * #define WLOG_HARDCODE_LEVEL WLOGLEVEL_VV before include this file.
 */
#ifndef WLOG_HARDCODE_LEVEL
 #if 1//VIA_DEBUG_CODE
   #define WLOG_HARDCODE_LEVEL WLOGLEVEL_DEBUG
 #else
    #define WLOG_HARDCODE_LEVEL WLOGLEVEL_INFO
 #endif
#endif

typedef struct WLogger{
    int   level;          /* you can change it freely */  
    const char * name;    /* shouldn't change it. note: we just save a pointer, the text should not free.*/
}WLogger;



/**
 * Get a logger object for the nameTAG and set init level
 * @param nameTag   the logger's name  note it is case-insenstive and should not free.
 * @param initLevel  set the logger's init level if this is first time to get the wlogger object. 
 * @return WLogger* return a logger object with this name, never return NULL 
 * @note don't call this function repeatly, instead save it to a variable in your c/c++ source file. 
 */

WLogger * wlogGetInit(const char * nameTag, int initLevel);


/**
 * Get a logger object for the nameTAG with WLOG_HARDCODE_LEVEL level if not inited.
 * @param nameTag   the logger's name , note it is case-insenstive and should not free.
 * @return WLogger* return a logger object with this name, never return NULL 
 * @note don't call this function repeatly, instead save it to a variable in your c/c++ source file. 
 */
static inline WLogger * wlogGet(const char * nameTag){
	return wlogGetInit(nameTag, WLOG_HARDCODE_LEVEL);
}



/**
 * logSettingString is sth like "tag1=level,tag2=level2" to change the WLogger's debug level. 
 *  
 * the level can be "V|v|d|i|w|e". V means WLOGLEVEL_VV, i means WLOGLEVEL_INFO, etc.
 * for example: 
 *  wlogSetSetting("tag1=i,tag2=V,tag3=v");
 *  set tag1 level to WLOGLEVEL_INFO,  tag2 to WLOGLEVEL_VV, tag3 to WLOGLEVEL_VERBOSE.
 *  
 * if no tag name specified, change all to the level 
 * for example: 
 *      wlogSetSetting("=w,tag1=d");
 * change all level to WLOGLEVEL_WARN, then set tag1's level to WLOGLEVEL_DEBUG
 *  
 * @return int 0 if ok, else error happens when parse the string.
 * @note logSettingString shouldn't contains any space characters.
 */
int       wlogSetSetting(const char * logSettingString);


void      wlogDump();

void wlogSaveLogtoFileInit();


typedef void (*wlogFmtTime)(char * buffer, int size);
extern wlogFmtTime g_wlogFmtTime;

#undef WLOGVV
#undef WLOGV
#undef WLOGD
#undef WLOGI
#undef WLOGW
#undef WLOGE
#undef WLOGVV_IF
#undef WLOGV_IF
#undef WLOGD_IF
#undef WLOGI_IF
#undef WLOGW_IF
#undef WLOGE_IF

#if WLOG_HARDCODE_LEVEL >= WLOGLEVEL_VV
    #define WLOGVV(wlogger, ...)          WLOG_LEVEL__(WLOGLEVEL_VV, wlogger,  __VA_ARGS__)
    #define WLOGVV_IF(cond, wlogger, ...) WLOG_LEVEL_IF__(cond, wlogger, WLOGLEVEL_VV, __VA_ARGS__)
#else
    #define WLOGVV(wlogger, ...) ((void)0)
    #define WLOGVV_IF(cond, wlogger, ...) ((void)0)
#endif


#if WLOG_HARDCODE_LEVEL >= WLOGLEVEL_VERBOSE
    #define WLOGV(wlogger, ...)          WLOG_LEVEL__(WLOGLEVEL_VERBOSE, wlogger,  __VA_ARGS__)
    #define WLOGV_IF(cond, wlogger, ...) WLOG_LEVEL_IF__(cond, wlogger, WLOGLEVEL_VERBOSE, __VA_ARGS__)
#else
    #define WLOGV(wlogger, ...) ((void)0)
    #define WLOGV_IF(cond, wlogger, ...) ((void)0)
#endif

#if WLOG_HARDCODE_LEVEL >= WLOGLEVEL_DEBUG
    #define WLOGD(wlogger, ...)          WLOG_LEVEL__(WLOGLEVEL_DEBUG, wlogger,  __VA_ARGS__)
    #define WLOGD_IF(cond, wlogger, ...) WLOG_LEVEL_IF__(cond, wlogger, WLOGLEVEL_DEBUG, __VA_ARGS__)
#else
    #define WLOGD(wlogger, ...) ((void)0)
    #define WLOGD_IF(cond, wlogger, ...) ((void)0)
#endif

#if WLOG_HARDCODE_LEVEL >= WLOGLEVEL_INFO
    #define WLOGI(wlogger, ...)          WLOG_LEVEL__(WLOGLEVEL_INFO, wlogger,  __VA_ARGS__)
    #define WLOGI_IF(cond, wlogger, ...) WLOG_LEVEL_IF__(cond, wlogger, WLOGLEVEL_INFO, __VA_ARGS__)
#else
    #define WLOGI(wlogger, ...) ((void)0)
    #define WLOGI_IF(cond, wlogger, ...) ((void)0)
#endif


#if WLOG_HARDCODE_LEVEL >= WLOGLEVEL_WARN
    #define WLOGW(wlogger, ...)          WLOG_LEVEL__(WLOGLEVEL_WARN, wlogger,  __VA_ARGS__)
    #define WLOGW_IF(cond, wlogger, ...) WLOG_LEVEL_IF__(cond, wlogger, WLOGLEVEL_WARN, __VA_ARGS__)
#else
    #define WLOGW(wlogger, ...) ((void)0)
    #define WLOGW_IF(cond, wlogger, ...) ((void)0)
#endif


#if WLOG_HARDCODE_LEVEL >= WLOGLEVEL_ERROR
    #define WLOGE(wlogger, ...)          WLOG_LEVEL__(WLOGLEVEL_ERROR, wlogger,  __VA_ARGS__)
    #define WLOGE_IF(cond, wlogger, ...) WLOG_LEVEL_IF__(cond, wlogger, WLOGLEVEL_ERROR, __VA_ARGS__)
#else
    #define WLOGE(wlogger, ...) ((void)0)
    #define WLOGE_IF(cond, wlogger, ...) ((void)0)
#endif

    
#define WLOG_LEVEL__(lvl, wlogger, ...)   \
    WLOG_LEVEL_IF__(wlogger->level >= (lvl), wlogger, lvl, __VA_ARGS__)

#define WLOG_LEVEL_IF__(cond, wlogger, level, ...)    \
    ( !(cond) \
    ? (void)0 \
    : (void)wlogOutput(wlogger, level, __VA_ARGS__))


/** 
 *  
 * Don't call this functions directly, instead use WLOGD, WLOGI...
 */
#ifdef __cplusplus
extern "C" 
#endif
void wlogOutput(const WLogger * logger, int level, const char *fmt, ...)
	__attribute__((__format__ (printf, 3, 4)));


void do_panic(const char *fmt, ...);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

//used for C++ only.
struct FunctionTracer {
    FunctionTracer(WLogger * log, const char * func) {
        funcname = func;
        logger = log;
        WLOGD(logger, " -> Enter %s",  funcname);
    }

    ~FunctionTracer() {
        WLOGD(logger, " <- Leave %s", funcname);
    }
    const char * funcname;
    WLogger * logger;
};

#ifdef FUNCTION_TRACE_DEBUG
#define FUNCTION_TRACE(); FunctionTracer temp_tracer(myLog, __FUNCTION__);
#else
#define FUNCTION_TRACE()
#endif

#endif  // __cplusplus


//Help macro
#define WTRACE()  printf("=====> %s:%d\n", __FUNCTION__, __LINE__)
#define PANIC(reason)   do_panic("BUG:  panic at %s:%d: %s\n", __FUNCTION__, __LINE__, reason)

typedef int (*panicCallback)();
void setPanicCallback(panicCallback cb);

#endif // WMT_WLOGGER_HPP_INCLUDED



