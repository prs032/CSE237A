#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prashant Singh");
MODULE_DESCRIPTION("PMUON");

int init_module(void) {
    unsigned int v;
    printk("Turn PMU on\n");

    // 1. Enable "User Enable Register"
    asm volatile("mcr p15, 0, %0, c9, c14, 0\n\t" :: "r" (0x00000001)); 

    // 2. Reset Performance Monitor Control Register(PMCR), Count Enable Set Register, and Overflow Flag Status Register
    asm volatile ("mcr p15, 0, %0, c9, c12, 0\n\t" :: "r"(0x00000017));
    asm volatile ("mcr p15, 0, %0, c9, c12, 1\n\t" :: "r"(0x8000003f));
    asm volatile ("mcr p15, 0, %0, c9, c12, 3\n\t" :: "r"(0x8000003f));

    // 3. Disable Interrupt Enable Clear Register
    asm volatile("mcr p15, 0, %0, c9, c14, 2\n\t" :: "r" (~0));

    // 4. Read how many event counters exist 
    asm volatile("mrc p15, 0, %0, c9, c12, 0\n\t" : "=r" (v)); // Read PMCR
    printk("pmon_init(): have %d configurable event counters.\n", (v >> 11) & 0x1f);

    // 5. Set event counter registers 
    // ***** YOUR CODE STARTS HERE *****

    // Write PMXEVTYPER #r
    // This is done by first writing the counter number to PMSELR and then writing PMXEVTYPER

    // 1. For L1 Data cache access (L1D_CACHE)
    asm volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x0));     // Select event counter in PMSELR
    asm volatile ("MCR p15, 0, %0, c9, c13, 1\t\n" :: "r"(0x04));    // Set the event number in PMXEVTYPER

    // 2. For L1 data cache miss (L1D_CACHE_REFILL)
    asm volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x1));     // Select event counter in PMSELR
    asm volatile ("MCR p15, 0, %0, c9, c13, 1\t\n" :: "r"(0x03));    // Set the event number in PMXEVTYPER

    // 3. For L2 data cache access (L2D_CACHE)
    asm volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x2));     // Select event counter in PMSELR
    asm volatile ("MCR p15, 0, %0, c9, c13, 1\t\n" :: "r"(0x16));    // Set the event number in PMXEVTYPER

    // 4. For L2 data cache miss (L2D_CACHE_REFILL)
    asm volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x3));     // Select event counter in PMSELR
    asm volatile ("MCR p15, 0, %0, c9, c13, 1\t\n" :: "r"(0x17));    // Set the event number in PMXEVTYPER

    // 5. For # of conditional branches executed
    asm volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x4));     // Select event counter in PMSELR
    asm volatile ("MCR p15, 0, %0, c9, c13, 1\t\n" :: "r"(0xC9));    // Set the event number in PMXEVTYPER

    // 6. For # of conditional branches mispredicted
    asm volatile ("MCR p15, 0, %0, c9, c12, 5\t\n" :: "r"(0x5));     // Select event counter in PMSELR
    asm volatile ("MCR p15, 0, %0, c9, c13, 1\t\n" :: "r"(0xCC));    // Set the event number in PMXEVTYPER
    
    return 0;
}

void cleanup_module(void) {
    printk("GOODBYE, PMU Off\n");
}

