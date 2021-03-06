#include <i386/cpu.h>
#include <i386/cpuid.h>
#include <i386/vga.h>

void getCPUVendor(void)
{
    uint16_t d;
    char vendor[13];
    cpuid(0, &d, (uint16_t *)(vendor + 0), (uint16_t *)(vendor + 8), (uint16_t *)(vendor + 4));
    vendor[12] = '\0';

    printk("CPU Vendor: %s\n", vendor);
}

void getCPUFeatures(void)
{
    uint16_t check;
    uint16_t ccheck = 0x01;
    if (&check >= &ccheck)
    {
        cpuid(0x01, &eax, &ebx, &ecx, &edx);

        printk("CPU Features:");

        if (edx & CPUID_FEAT_EDX_PSE)      printk(" PSE");
        if (edx & CPUID_FEAT_EDX_PAE)      printk(" PAE");
        if (edx & CPUID_FEAT_EDX_APIC)     printk(" APIC");
        if (edx & CPUID_FEAT_EDX_MTRR)     printk(" MTRR");

        printk("\n");
        printk("CPU Instruction Set:");

        if (edx & CPUID_FEAT_EDX_TSC)      printk(" TSC");
        if (edx & CPUID_FEAT_EDX_MSR)      printk(" MSR");
        if (edx & CPUID_FEAT_EDX_SSE)      printk(" SSE");
        if (edx & CPUID_FEAT_EDX_SSE2)     printk(" SSE2");
        if (ecx & CPUID_FEAT_ECX_SSE3)     printk(" SSE3");
        if (ecx & CPUID_FEAT_ECX_SSSE3)    printk(" SSSE3");
        if (ecx & CPUID_FEAT_ECX_SSE41)    printk(" SSE41");
        if (ecx & CPUID_FEAT_ECX_SSE42)    printk(" SSE42");
        if (ecx & CPUID_FEAT_ECX_AVX)      printk(" AVX");
        if (ecx & CPUID_FEAT_ECX_F16C)     printk(" F16C");
        if (ecx & CPUID_FEAT_ECX_RDRAND)   printk(" RDRAND");

        printk("\n");
    }
}

void getCPUArch(void)
{
    uint16_t a;
    cpuid(0x80000000, &a, &ebx, &ecx, &edx);

    if (a >= 0x80000001)
    {
        cpuid(0x80000001, &eax, &ebx, &ecx, &edx);

        if (edx & CPUID_FEAT_EDX_64_BIT)
        {
            printk("CPU Architecture: 64-bit\n");
        }
        else
        {
            printk("CPU Architecture: 32-bit\n");
        }
    }
}

void getCPUName(void)
{
    uint16_t cpuop;
    uint16_t ccpuop = 0x80000004;
    if (&cpuop >= &ccpuop)
    {
        char name[48];
        /*  EBX = 'h' 't' 'u' 'A'
            EDX = 'c' 't' 'n' 'e'
            ECX = 'D' 'M' 'A' 'c' */
        cpuid(0x80000002, (uint16_t *)(name +  0), (uint16_t *)(name +  4), (uint16_t *)(name +  8), (uint16_t *)(name + 12));
        cpuid(0x80000003, (uint16_t *)(name + 16), (uint16_t *)(name + 20), (uint16_t *)(name + 24), (uint16_t *)(name + 28));
        cpuid(0x80000004, (uint16_t *)(name + 32), (uint16_t *)(name + 36), (uint16_t *)(name + 40), (uint16_t *)(name + 44));

        const char *p = name;

        while (*p == ' ')
        {
            ++p;
        }

        printk("CPU Name: %s\n", p);
    }
}