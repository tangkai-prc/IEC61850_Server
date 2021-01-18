#ifndef PERFORMANCE_ANALYSIS_H
#define PERFORMANCE_ANALYSIS_H

#ifdef WIN32
#include <time.h>
#elif defined(VXWORKS)
#include <tickLib.h>
#else
#include <sys/time.h>
#endif

static void tstart();
static void tend();
static double tval();

#if defined(WIN32)
static LARGE_INTEGER _tstart, _tend;
static LARGE_INTEGER freq;

void tstart(void)
{
	static int first = 1;

	if(first) {
		QueryPerformanceFrequency(&freq);
		first = 0;
	}

	QueryPerformanceCounter(&_tstart);
}

void tend(void)
{
	QueryPerformanceCounter(&_tend);
}

double tval()
{
	return ((double)_tend.QuadPart - (double)_tstart.QuadPart)/((double)freq.QuadPart);
}

#elif defined(VXWORKS)

static unsigned int _tstart, _tend;

void tstart(void)
{
	_tstart = tickGet();
}

void tend(void)
{
	_tend = tickGet();
}

double tval()
{
	return _tend-_tstart;
}

#else

static struct timeval _tstart, _tend;
static struct timezone tz;

void tstart(void)
{
	gettimeofday(&_tstart, &tz);
}

void tend(void)
{
	gettimeofday(&_tend, &tz);
}

double tval()
{
	double t1, t2;
	t1 =  (double)_tstart.tv_sec + (double)_tstart.tv_usec/(1000*1000);
	t2 =  (double)_tend.tv_sec + (double)_tend.tv_usec/(1000*1000);
	return t2-t1;
}

#endif

#endif
