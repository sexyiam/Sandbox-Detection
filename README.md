We will be doing some Sandboxie detection based on comparing the execution time of two different instructions, specifically the FYL2XP1 and RDTSC instruction.

The FYL2XP1 instruction calculates the logarithm base 2 of the number specified in ST(1). In this case, it's a pointless calculation because the result is discarded. The reason for using this instruction is that it has a longer execution time than RDTSC.

The RDTSC instruction reads the Time Stamp Counter (TSC), a 64-bit register that increments at each clock cycle.

[code]
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
[/code]
We will use inlined_asm using [b]rdtsc[/b] we're reading the Time Stamp Counter and passing into EDX:EAX, using [b]shl edx, 32[/b] wiill shift the high 32 bits of the TSC (in EDX) 32 positions to the left, the others should be self-explanatory we're pushing the value of EDX,EAX etc... to the stack, [b]CPUID[/b] for fetching the CPU Info, [b]pop edx,ecx[/b] etc... for pops the value from stack.

To measure the CPU performance, the program will repeat this process 100 times and takes the average execution time.
We will be using [b]QueryPerformanceCounter[/b] function to measure time accurately.
So, the code should look like,
[code]
#define THRESHOLD 4.8
#define MAX_SAND 60 
LARGE_INTEGER start;
LARGE_INTEGER end;
int phys = 0;
int sand = 0;
double sum = 0;
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
[/code]

Next, the program calculates a score by dividing the execution time of the FYL2XP1 instruction by the execution time of the RDTSC instruction. If the ratio is above the threshold (4.8), the program considers it likely to be running in a sandbox. So, if number of times it detected a sandbox (sand) is greater than a maximum threshold (MAX_SAND), it concludes that it's running in a sandbox.

The final checking part should be like this,
[code]
printf("Avg. score: %f\n", sum);
printf("%d%% likely to be physical\n", phys);
if (sand > MAX_SAND) {
	printf("Sandbox...\n");
	system("pause");
}
else {
	printf("Not a Potential VM\n");
	system("pause");
}
[/code]

On real Machine :
[img]https://i.imgur.com/ljZKFN4.png[/img]

On Tria.ge : 
[img]https://i.imgur.com/eKUdjBL.png[/img]

[b]Note: This method is not foolproof. Anyone can modify the CPU microcode to slow down the FYL2XP1 instruction while keeping the RDTSC instruction unchanged, thereby giving a false negative.[/b]
