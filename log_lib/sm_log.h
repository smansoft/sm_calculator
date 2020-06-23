/*
 *    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#pragma once

#include "pch.h"

#if defined __cplusplus
extern "C" {
#endif

#if !defined MAX_PATH
    #define MAX_PATH    260
#endif

#if defined SM_OS_LINUX
    #define SM_FILES_DIR_SLASH      "/"
#elif defined SM_OS_WINDOWS
    #define SM_FILES_DIR_SLASH		"\\"
#endif

#ifndef SM_RES_OK
    #define SM_RES_OK			0
#endif

#ifndef SM_RES_ERROR
    #define SM_RES_ERROR		1
#endif

typedef int				errno_t;        // definitition of errno_t (int) 
                                        // errno_t - is a basic type, returned by log output functions

#if defined SM_SYNC_LOG

/*
    multi-platform log synchronizing type;

    as a rule (if a developer don't use non-synchronized run-time in VS),
    multi-platform log synchronizing type don't need;

    if developer use multi-thread run-time in VS, all output to file will be synchronized;

    this log synchronizing type can be used if developer need to synchronize several log outs:

    sm_log_config log_config;

    ...

    sm_sync_type_lock(&log_config.m_sync);

    sm_log_print(&log_config, "some categorty", "log message 1");
    sm_log_print(&log_config, "some categorty", "log message 2");
    sm_log_print(&log_config, "some categorty", "log message 3");

    sm_sync_type_unlock(&log_config.m_sync);

    In this case all calls of sm_log_print between sm_sync_type_lock() and
    sm_sync_type_unlock() should be thread-synchronized;
*/
typedef struct _sm_sync_type
{
#if defined SM_OS_LINUX
	mtx_t				m_sync; // sync object (mutex), supported by C11 (ISO/IEC 9899:2011) (Linux)
#elif defined SM_OS_WINDOWS
    CRITICAL_SECTION    m_sync; // sync object (critical section) (Windows)
#endif
} sm_sync_type, *psm_sync_type;

/*  initializing the multi-platform synchronizing object    */
errno_t sm_sync_type_init(sm_sync_type* const sync_type);

/*  destroying the multi-platform synchronizing object  */
errno_t sm_sync_type_close(sm_sync_type* const sync_type);

/*  locking the current thread, using multi-platform synchronizing object   */
errno_t sm_sync_type_lock(sm_sync_type* const sync_type);

/*  checking for lock the current thread, using multi-platform synchronizing object   */
errno_t sm_sync_type_try_lock(sm_sync_type* const sync_type);

/*  unlocking the current thread, using multi-platform synchronizing object   */
errno_t sm_sync_type_unlock(sm_sync_type* const sync_type);

#endif

/*  
    internal type for saving of time data;
    this type object is used 
*/
typedef struct _sm_log_time
{
    unsigned int m_year;
    unsigned int m_month;
    unsigned int m_day_of_week;
    unsigned int m_day;
    unsigned int m_hour;
    unsigned int m_minute;
    unsigned int m_second;
    unsigned int m_milliseconds;
} sm_log_time, *psm_log_time;

/*
    main log support structure;
    contains all necessary info for control/providing the log output;
    supports simple log output without rolling;
    supports synchronizing (requires preprocessor definition: SM_SYNC_LOG);
*/
typedef struct _sm_log_config
{
    unsigned int    m_start;    //  starting flag:   if m_start == 0 - even if log file is opened,
                                //                                      log functions will not write in the log file
                                //                   if m_start == 1 - if log file is opened,
                                //                                      log functions will write in the log file
    char            m_log_dpath[MAX_PATH];  //  directory, where log file will be created
    char            m_log_fname[MAX_PATH];  //  file name of log file
    char            m_log_fpath[MAX_PATH];  //  full path of flog file 

#if defined SM_SYNC_LOG
    sm_sync_type    m_sync;         //  multi-platform synchronized object
#endif

    FILE            *m_log_file;    //  log file object, where all log will be output
} sm_log_config, *psm_log_config;

/*
    initializing the sm_log_config object;
    also initalizing sm_sync_type object (m_sync), if SM_SYNC_LOG is defined;

    args:
        log_config  - sm_log_config object
        log_dpath   - directory, where log file will be created 
        log_fname   - file name of log file
    result:
        SM_RES_OK 
        SM_RES_ERROR
*/
errno_t sm_log_init(sm_log_config *const log_config, const char *const log_dpath, const char *const log_fname);

/*
    destroying the sm_log_config object;
    result:
        SM_RES_OK
        SM_RES_ERROR
*/
errno_t sm_log_close(sm_log_config* const log_config);

/*
    starting the log output;
    if file isn't open, returns SM_RES_ERROR 
*/
errno_t sm_log_start(sm_log_config* const log_config);

/*
    stopping the log output without closing of file;
    if log output isn't started, returns SM_RES_ERROR
*/
errno_t sm_log_stop(sm_log_config* const log_config);

/*
    multi-platform initializing the sm_log_time object;
*/
errno_t sm_log_get_local_time(sm_log_time *const time);

/*
    log of ready text array (without formatting);

    args:
        log_config      - sm_log_config object
        log_category    - category of log (for filtering)
        log_text        - file name of log file

    for example:

        20/06/2020 15:52:06.339 - <main> - sm_calc_test_expr is started
        20/06/2020 15:52:06.340 - <main> - ------------------
        20/06/2020 15:52:06.340 - <sm_init_random> - random starting point has been initialized

        date/time (with msecs)  :   20/06/2020 15:52:06.339
        category                :   <main>, <sm_init_random>
        output log text         :   sm_calc_test_expr is started

*/
errno_t sm_log_print(sm_log_config* const log_config, const char *const log_category, const char *const log_text);

/*
    log of text (with formatting);

    args:
        log_config      - sm_log_config object
        log_category    - category of log (for filtering)
        log_format      - formatted string and suite of follow arguments

    for example:
        sm_log_printf(SM_LOG_CONFIG, "sm_conv_quot_sz_2_sz", "in: in_buf = %s out_buf_len = %d", in_buf, out_buf_len);

*/
errno_t sm_log_printf(sm_log_config* const log_config, const char *const log_category, const char *const log_format, ...);

#if defined __cplusplus
}
#endif

