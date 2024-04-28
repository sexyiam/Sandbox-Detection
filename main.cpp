#include <Windows.h>
#include <stdio.h>
#include <intrin.h>


const int THRESHOLD = 48;
const int MAX_SAND = 60;
const int ITERATIONS = 100;
const int INNER_ITERATIONS = 100;


double measureFYL2XP1Time();
double measureCPUTime();
void check();

double measureFYL2XP1Time() {
    LARGE_INTEGER start, end;
    QueryPerformanceCounter(&start);
    for (int i = 0; i < INNER_ITERATIONS; i++)
        __asm FYL2XP1;
    QueryPerformanceCounter(&end);
    return static_cast<double>(end.QuadPart - start.QuadPart) / INNER_ITERATIONS;
}

double measureCPUTime() {
    LARGE_INTEGER start, end;
    double time;
    QueryPerformanceCounter(&start);
    _mm_lfence(); 
    for (int i = 0; i < INNER_ITERATIONS; i++) {
        __asm {
            rdtsc
            shl edx, 32
            or eax, edx
        };
        _mm_lfence(); 
    }
    QueryPerformanceCounter(&end);
    time = static_cast<double>(end.QuadPart - start.QuadPart) / INNER_ITERATIONS;
    return time;
}

void check() {
    int phys = 0;
    int sand = 0;
    double sum = 0.0;

    for (int j = 0; j < ITERATIONS; j++) {
        double fyTime = measureFYL2XP1Time();
        double cpuTime = measureCPUTime();
        double ratio = cpuTime / fyTime;

        if (ratio > THRESHOLD)
            sand++;
        else
            phys++;

        sum += ratio;
        Sleep(10);
    }

    sum /= ITERATIONS;

    if (sand > MAX_SAND) {
        printf("Potential Sandbox or Virtual Machine Detected\n");
    }
    else {
        printf("Not a Potential VM\n");
    }

    printf("Press any key to continue . . .\n");
    getchar();
}

int main() {
    check();
    return 0;
}
