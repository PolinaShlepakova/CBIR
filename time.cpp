// for benchmarking
#include <windows.h>

// returns frequency (ticks per second)
long long getFrequency() {
	LARGE_INTEGER f;
	::QueryPerformanceFrequency(&f);
	return f.QuadPart;
}

// returns time in ticks
long long getTimeInTicks() {
	LARGE_INTEGER t;
	::QueryPerformanceCounter(&t);
	return t.QuadPart;
}

// returns elapsed time from start in milliseconds
double elapsedTimeInMillis(long long start, long long frequency) {
	return ((getTimeInTicks() - start) * 1000.0) / frequency;
}