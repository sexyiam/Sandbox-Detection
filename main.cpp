#include <Windows.h>
#include <stdio.h>
#include <intrin.h>

#define THRESHOLD 4.8
#define MAX_SAND 60
LARGE_INTEGER start;
LARGE_INTEGER end;
int phys = 0;
int sand = 0;
double sum = 0;
void check() {
    for (int j = 0; j < 100; j++) {
        QueryPerformanceCounter(&start);
        for (int i = 0; i < 100; i++)
            __asm FYL2XP1;
        QueryPerformanceCounter(&end);
        double fy = static_cast<double>(end.QuadPart - start.QuadPart);
        QueryPerformanceCounter(&start);
        for (int i = 0; i < 100; i++)
            __asm {
            rdtsc
            shl edx, 32
            or eax, edx
            push eax
            push ebx
            push ecx
            push edx
            xor eax, eax
            CPUID
            pop edx
            pop ecx
            pop ebx
            pop eax
        };
        QueryPerformanceCounter(&end);
        double cpu = static_cast<double>(end.QuadPart - start.QuadPart);
        double ratio = cpu / fy;
        if (ratio > THRESHOLD)
            sand++;
        else
            phys++;
        sum += ratio;
        Sleep(10);

    }
    sum /= 100;
    if (sand > MAX_SAND) {
        printf("Sandbox...\n");
        system("pause");
    }
    else {
        printf("Not a Potential VM\n");
        system("pause");
    }
}

