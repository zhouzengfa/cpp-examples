#pragma once 
#ifdef __linux__

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include <sched.h>
#include <errno.h>

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

// 获得线程安全本地时间
inline struct tm * Port_LocalTime(const time_t *timep, struct tm* nt)
{
	nt->tm_isdst = 0;
	return localtime_r(timep, nt);
}

// 获得UTC时间ms
inline int64_t Port_GetTimeOfDay()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	int64_t now = ((int64_t)tv.tv_sec * 1000) + ((int64_t)tv.tv_usec / 1000);

	return now;
}

// 获得本地当前时间
inline bool Port_GetLocalTime(port_date_time_t* dt)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	struct tm nt;
	tm* pt = Port_LocalTime(&tv.tv_sec, &nt);
	
	dt->nYear = pt->tm_year + 1900;
	dt->nMonth = pt->tm_mon + 1;
	dt->nDay = pt->tm_mday;
	dt->nHour = pt->tm_hour;
	dt->nMinute = pt->tm_min;
	dt->nSecond = pt->tm_sec;
	dt->nMilliseconds = tv.tv_usec / 1000;
	dt->nDayOfWeek = pt->tm_wday;
	
	return true;
}

// time单位ms
inline bool Port_GetLocalTimeByTime(int64_t time, port_date_time_t* dt)
{
	struct timeval tv;

	tv.tv_sec = time / 1000;
	tv.tv_usec = time % 1000;

	struct tm nt;
	tm* pt = Port_LocalTime(&tv.tv_sec, &nt);

	dt->nYear = pt->tm_year + 1900;
	dt->nMonth = pt->tm_mon + 1;
	dt->nDay = pt->tm_mday;
	dt->nHour = pt->tm_hour;
	dt->nMinute = pt->tm_min;
	dt->nSecond = pt->tm_sec;
	dt->nMilliseconds = tv.tv_usec;
	dt->nDayOfWeek = pt->tm_wday;

	return true;
}

#endif // __linux__
