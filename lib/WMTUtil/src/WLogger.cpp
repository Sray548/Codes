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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
// #include <String8.h>
#include <time.h>
// #include "arm_backtrace.h"
// #include "PropFile.h"
#include <sys/syscall.h>//Linux system call for thread id
#include <syslog.h>

#include <sys/time.h>
#include <errno.h>
#ifdef ANDROID
    #include <android/log.h>
#endif

// #include "Timers.h"

#define WLOG_HARDCODE_LEVEL WLOGLEVEL_INFO
#include "WLogger.h"
// #include "debug_new.h"
// #include "ComFuns.h"

#define MAX_LOG_OBJ 96          //should be ok for normal case
static WLogger  sLogList[MAX_LOG_OBJ];


static WLogger* mylog = wlogGet("WLogger");
static int bOut2screen = -1;

#define     SAVE_LOG_FILE_PREFIX        "/mnt/sd/D700Log/log"
static FILE* save_log_fp = NULL;

//MUST call at the beginning of the process
void wlogSaveLogtoFileInit()
{
    // if(save_log_fp == NULL) {
    //     char filename[128] = {0};
    //     char procname[128] = {0};

    // 	time_t curTime;
    // 	struct tm now;
    	
    // 	time(&curTime);
    //     localtime_r(&curTime, &now);

    //     wmt::String8 timeStr = wmt::String8::format("%04d_%02d_%02d_%02d_%02d_%02d", (1900+now.tm_year), (1+now.tm_mon), now.tm_mday,
    //                 now.tm_hour, now.tm_min, now.tm_sec);

    //     getProcName(getpid(), procname, sizeof(procname));
        
    //     snprintf(filename, sizeof(filename), "%s_%s_%s_%d", 
    //                     SAVE_LOG_FILE_PREFIX, procname, timeStr.string(), getpid());
    //     save_log_fp = fopen(filename, "wb");
    //     printf("filename %s save_log_fp %p errno %s\n", filename, save_log_fp, strerror(errno));
    // }   
}

static WLogger * wlogFind(const char * nameTag){
    for ( int i = 0; i < MAX_LOG_OBJ; i++)  {
        if(sLogList[i].name == NULL)
            break;
        if(strcasecmp(sLogList[i].name, nameTag) == 0)
            return &sLogList[i];
    }
    return NULL;
}


WLogger * wlogGetInit(const char * nameTag, int initLevel)
{
    int i;
    for ( i = 0; i < MAX_LOG_OBJ; i++)  {
        if(sLogList[i].name == NULL)
            break;
        if(strcasecmp(sLogList[i].name, nameTag) == 0)
            return &sLogList[i];
    }
    if(i == MAX_LOG_OBJ) {
        printf("wlogGetInit over-range, set a big MAX_LOG_OBJ,now is %d\n", MAX_LOG_OBJ);
        return &sLogList[0];    //don't return null, instead return first object.
    }

    sLogList[i].name  = nameTag;
    sLogList[i].level = initLevel;
    return &sLogList[i];
}

// debugString is sth like "buffer=d,video=i,audio=2"
int wlogSetSetting(const char * logSettingString)
{
    char * name1, *name2;
    char * value1, *value2;

    char * dupString = strdup(logSettingString);
    if(!dupString)
        return -ENOMEM;

    char  * str = dupString;
repeat:
    name1 = name2 = value1 = value2 = NULL;

    /* lookup name*/
    name1 = str;
    for( ;*str != '\0'; str++) {
        if( *str == '=') {
            name2 = str;
            break;
        }
    }

    if( *str == '=') {
        //name found, lookup loglevel
        value1 = ++str;
        while(true) {
            if( *str == ','){
                value2 = str++;
                break;
            }
            else if(*str == '\0'){
                value2 = str;
                break;
            }
            str++;
        }
    }

    if( name2 != NULL && value2 != NULL){
        *name2 = *value2 = '\0';
        int loglevel;
        switch(*value1) {
        case 'V': loglevel = WLOGLEVEL_VV; break;
        case 'v': loglevel = WLOGLEVEL_VERBOSE; break;
        case 'd':
        case 'D': loglevel = WLOGLEVEL_DEBUG; break;
        case 'i':
        case 'I': loglevel = WLOGLEVEL_INFO; break;
        case 'w':
        case 'W': loglevel = WLOGLEVEL_WARN; break;
        case 'e':
        case 'E': loglevel = WLOGLEVEL_ERROR; break;
        default:
            WLOGW(mylog, "<%s> unknown loglevel %s", name1, value1);
            goto repeat;
        }
        
        if( name2 == name1) {
            //empty name for all log objects
            WLOGD(mylog, "Set all wlogger object's level to %d", loglevel);
            for ( int i = 0; i < MAX_LOG_OBJ; i++)  {
                if(sLogList[i].name != NULL) {
                    WLOGD(mylog, "  set <%s> level from %d to %d(%c)", 
                            sLogList[i].name, sLogList[i].level, loglevel, *value1);
                    sLogList[i].level = loglevel;                    
                }
                else {
                    break;
                }
            }
        }
        else {
            WLogger * log = wlogFind(name1);
			if(log){
            	log->level = loglevel;
            	WLOGD(mylog, "set <%s> level to %d(%c)", log->name, log->level, *value1);
			}
            else {
                WLOGW(mylog, "Can't find <%s> object", name1);
            }
        }
        goto repeat;
    }

    free(dupString);
    return 0;
}


static inline char levelString(int level)
{
    switch(level){
    case WLOGLEVEL_VV:
        return 'V';
    case WLOGLEVEL_VERBOSE:
        return 'v';
    case WLOGLEVEL_DEBUG:
        return 'D';
    case WLOGLEVEL_INFO:
        return 'I';
    case WLOGLEVEL_WARN:
        return 'W';
    case WLOGLEVEL_ERROR:
        return 'E';
    }
    return '?';
}

void wlogDump()
{
    printf(" NO.    Name                              Level\n");
    printf("***********************************************\n");

    for ( int i = 0; i < MAX_LOG_OBJ; i++)  {
        if(sLogList[i].name == NULL)
            return;
        printf("#%2d     %-32s : %c\n", i, sLogList[i].name, levelString(sLogList[i].level));
    }
}

int toSyslogLevel(int level){
    int priority;

    switch(level){
    case WLOGLEVEL_VV:
    case WLOGLEVEL_VERBOSE:
        priority = LOG_DEBUG;
        break;
    case WLOGLEVEL_DEBUG:
        priority = LOG_DEBUG;
        break;
    case WLOGLEVEL_INFO:
        priority = LOG_INFO;
        break;
    case WLOGLEVEL_WARN:
        priority = LOG_WARNING;
        break;
    case WLOGLEVEL_ERROR:
        priority = LOG_ERR;
        break;
    default:
        priority = LOG_DEBUG;
        break;
    }   
    return priority; 
}

void wlogOutput(const WLogger * logger, int level, const char *fmt, ...)
{
    // if(level > logger->level) {
    //     return;
    // }
    
    // if(bOut2screen == -1) {
    //     bOut2screen = getTestInt("debug_print_to_screen", 0);
    // }
    // const char * name = logger ? logger->name : "N/A";

    // if(save_log_fp == NULL) {
    //     va_list ap;
    //     va_start(ap, fmt);
    //     if(bOut2screen) {
    //         vprintf(fmt, ap);
    //         int n = strlen(fmt);
    //         if(n > 0 && fmt[n - 1] != '\n')
    //             printf("\n");
    //     }
    //     vsyslog(toSyslogLevel(level), fmt, ap);
    //     va_end(ap);
    // } else {
    //     va_list ap;
    //     #define LOG_BUF_SIZE	512
    //     char buf[LOG_BUF_SIZE];
    //     va_start(ap, fmt);
    //     vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);    
    //     va_end(ap);

    //     struct timeval temp_time;
    //     gettimeofday(&temp_time, NULL);

    //     time_t curTime = temp_time.tv_sec;
    //     struct tm now;
        
    //     localtime_r(&curTime, &now);
        
    //     char printbuf[LOG_BUF_SIZE];
    //     snprintf(printbuf, sizeof(printbuf), "%c %04d-%02d-%02d %02d:%02d:%02d.%06ld [%d][%ld]/%s: %s\n",
    //                     levelString(level), 
    //                     (1900+now.tm_year), (1+now.tm_mon), now.tm_mday,
    //                     now.tm_hour, now.tm_min, now.tm_sec, 
    //                     temp_time.tv_usec, 
    //                     getpid(), syscall(SYS_gettid), 
    //                     name ,buf);
                        
    //     fwrite(printbuf, strlen(printbuf), 1, save_log_fp);
	// 	syslog(toSyslogLevel(level), "%s: %s\n", name ,buf);        
    // }
}

static panicCallback sPanicCallback = NULL;
void setPanicCallback(panicCallback cb)
{
    sPanicCallback = cb;
}

void do_panic(const char *fmt, ...)
{
//     printf(" \n\n=======================================================================\n");
//     va_list ap;
//     va_start(ap, fmt);
//     vprintf(fmt, ap);    
//     va_end(ap);
//     printf(" \n\n=======================================================================\n");

// #if (__CURRENT_PLATFORM__ == __PLATFORMTYPE_RTOS__)
//     send_mcu_watchdog_cmd(0);           //FIXME: remove it when product

//     arm_backtrace(NULL, 20);
//     vTaskDelay(1000); //wait printf done.
//    	taskDISABLE_INTERRUPTS();
//     // //stop whold world
//     // vTaskSuspendAll();      
//     // taskENTER_CRITICAL();   
//     while(1);
// #else
// 	arm_backtrace(NULL,12);

//     if(sPanicCallback) {
//         sPanicCallback();
//     }

// 	usleep(1000000);
//     exit(1);
// #endif
}


#if 0

static WLogger * helloLog = wlogGet("hello");
static WLogger * worldLog = wlogGet("world");

int main(int argc, char * argv[1])
{
    wlogSetSetting(argv[1]);
    WLOGD_IF(0, helloLog, "hello\n");

    WLOGV(helloLog, "hello, can u see it?");
    WLOGD(worldLog, "world %d", 4);
    WLOGI(worldLog, "world %d\n", 4);

    return 0;
}
#endif

