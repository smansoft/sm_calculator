/*
 *    Copyright (c) 2020 SManSoft <http://www.smansoft.com/>
 *    Sergey Manoylo <info@smansoft.com>
 */

#pragma once

#include "pch.h"

#include "sm_log_types.h"

#if defined __cplusplus
extern "C" {
#endif

	/*
		initializing the sm_log_config object, using one argument: log file path;
		also initalizing sm_sync_type object (m_sync), if SM_SYNC_LOG is defined;

		args:
			log_config  - sm_log_config object
			log_fpath   - path of the log file
		result:
			SM_RES_OK
			SM_RES_ERROR
	*/
	errno_t sm_log_init_fpath(sm_log_config* const log_config, const char* const log_fpath);

	/*
		initializing the sm_log_config object, using two argumets: directory path and log file name;
		also initalizing sm_sync_type object (m_sync), if SM_SYNC_LOG is defined;

		args:
			log_config  - sm_log_config object
			log_dpath   - directory, where log file will be created
			log_fname   - file name of log file
		result:
			SM_RES_OK
			SM_RES_ERROR
	*/
	errno_t sm_log_init_dpath_fname(sm_log_config* const log_config, const char* const log_dpath, const char* const log_fname);

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
	errno_t sm_log_get_local_time(sm_log_time* const time);

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
	errno_t sm_log_print(sm_log_config* const log_config, const char* const log_category, const char* const log_text);

	/*
		log of text (with formatting);

		args:
			log_config      - sm_log_config object
			log_category    - category of log (for filtering)
			log_format      - formatted string and suite of follow arguments

		for example:
			sm_log_printf(SM_LOG_CONFIG, "sm_conv_quot_sz_2_sz", "in: in_buf = %s out_buf_len = %d", in_buf, out_buf_len);

	*/
	errno_t sm_log_printf(sm_log_config* const log_config, const char* const log_category, const char* const log_format, ...);

#if defined __cplusplus
}
#endif
