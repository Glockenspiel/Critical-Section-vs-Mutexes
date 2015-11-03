#include <windows.h>
#include <stdio.h>

using namespace std;

/*
Test compaing performance of mutexes and critical sections
Source code from:
http://stackoverflow.com/questions/800383/what-is-the-difference-between-mutex-and-critical-section
*/

int main(){
	HANDLE mutex = CreateMutex(NULL, FALSE, NULL);
	CRITICAL_SECTION critSec;
	InitializeCriticalSection(&critSec);

	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER start, end;

	// Force code into memory, so we don't see any effects of paging.
	EnterCriticalSection(&critSec);
	LeaveCriticalSection(&critSec);
	QueryPerformanceCounter(&start);
	for (int i = 0; i < 1000000; i++)
	{
		EnterCriticalSection(&critSec);
		LeaveCriticalSection(&critSec);
	}

	QueryPerformanceCounter(&end);

	int totalTimeCS = (int)((end.QuadPart - start.QuadPart) * 1000 / freq.QuadPart);

	// Force code into memory, so we don't see any effects of paging.
	WaitForSingleObject(mutex, INFINITE);
	ReleaseMutex(mutex);

	QueryPerformanceCounter(&start);
	for (int i = 0; i < 1000000; i++)
	{
		WaitForSingleObject(mutex, INFINITE);
		ReleaseMutex(mutex);
	}

	QueryPerformanceCounter(&end);

	int totalTime = (int)((end.QuadPart - start.QuadPart) * 1000 / freq.QuadPart);

	printf("Mutex: %d CritSec: %d\n", totalTime, totalTimeCS);
	system("pause");
}