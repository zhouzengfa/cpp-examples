#pragma once

#ifdef _WIN32

#if (_MSC_VER >= 1915)
#pragma warning(disable:4845)   // __declspec(no_init_all) used but d1initall not set
#endif

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <signal.h>

// 日期和时间
struct port_date_time_t
{
	int nYear;
	int nMonth;
	int nDay;
	int nHour;
	int nMinute;
	int nSecond;
	int nMilliseconds;
	int nDayOfWeek;
};

// 内存使用情况
struct port_memory_info_t
{
	double dMemoryLoad;
	double dTotalMemory;
	double dAvailMemory;
};

#ifdef _WIN32
#define SECS_TO_FT_MULT 10000000

typedef struct win_timeval
{
	/** The seconds part of the time. */
	long sec;

	/** The millisecond fraction of the time. */
	long msec;
} win_timeval_t;

// Find 1st Jan 1970 as a FILETIME 
static void get_base_time(LARGE_INTEGER *base_time)
{
	SYSTEMTIME st;
	FILETIME ft;

	memset(&st,0,sizeof(st));
	st.wYear=1970;
	st.wMonth=1;
	st.wDay=1;
	SystemTimeToFileTime(&st, &ft);

	base_time->LowPart = ft.dwLowDateTime;
	base_time->HighPart = ft.dwHighDateTime;
	base_time->QuadPart /= SECS_TO_FT_MULT;
}

inline int win_gettimeofday(win_timeval_t *tv)
{
	SYSTEMTIME st;
	FILETIME ft;
	LARGE_INTEGER li;

	LARGE_INTEGER base_time;
	get_base_time(&base_time);

	/* Standard Win32 GetLocalTime */
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);

	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	li.QuadPart /= SECS_TO_FT_MULT;
	li.QuadPart -= base_time.QuadPart;

	tv->sec = li.LowPart;
	tv->msec = st.wMilliseconds;

	return 0;
}

inline int win_localtime(const win_timeval_t *tv, port_date_time_t *time)
{
	LARGE_INTEGER li;
	FILETIME ft;
	FILETIME lft;
	SYSTEMTIME st;

	LARGE_INTEGER base_time;
	get_base_time(&base_time);

	li.QuadPart = tv->sec;
	li.QuadPart += base_time.QuadPart;
	li.QuadPart *= SECS_TO_FT_MULT;

	ft.dwLowDateTime = li.LowPart;
	ft.dwHighDateTime = li.HighPart;
	FileTimeToLocalFileTime(&ft, &lft);
	FileTimeToSystemTime(&lft, &st);

	time->nYear = st.wYear;
	time->nMonth = st.wMonth;
	time->nDay = st.wDay;
	time->nDayOfWeek = st.wDayOfWeek;

	time->nHour = st.wHour;
	time->nMinute = st.wMinute;
	time->nSecond = st.wSecond;
	time->nMilliseconds = tv->msec;

	return 0;
}

#endif

// 获得线程安全本地时间
inline struct tm * Port_LocalTime(const time_t *timep, struct tm* nt)
{
	nt->tm_isdst = 0;
	localtime_s(nt, timep);

	return nt;
}

// 获得UTC时间ms
inline int64_t Port_GetTimeOfDay()
{
	struct win_timeval tv;

	win_gettimeofday(&tv);

	int64_t now = ((int64_t)tv.sec * 1000) + (int64_t)tv.msec;

	return now;
}

// 获得本地当前时间
inline bool Port_GetLocalTime(port_date_time_t* dt)
{
	struct win_timeval tv;

	win_gettimeofday(&tv);

	win_localtime(&tv, dt);

	return true;
}

#endif
