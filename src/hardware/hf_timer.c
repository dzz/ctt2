#include <stdio.h>
#include <windows.h>
#include "hf_timer.h"

LARGE_INTEGER Frequency;
double pcf;

void initHfTimer()
{

    QueryPerformanceFrequency(&Frequency);
    pcf = (double)Frequency.QuadPart;
    printf("frequency:%i\n", Frequency.QuadPart);
    printf("time:%f\n", getTimeMs());
}

double getTimeMs()
{
    LARGE_INTEGER CurrentTime;
    QueryPerformanceCounter(&CurrentTime);

	CurrentTime.QuadPart *= 1000;

    return (double)CurrentTime.QuadPart / pcf;
}
