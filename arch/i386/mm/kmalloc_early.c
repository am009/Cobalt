#include <i386/kmalloc_early.h>
#include <i386/panic.h>
#include <stdint.h>
#include <types.h>

extern uint32_t kernel_end;
uint32_t placement_address = (uint32_t)&kernel_end;
uint8_t enabled = 1;

static uint32_t kmalloc_imp(uint32_t sz, int align, uint32_t *phys)
{
    if(!enabled)
    {
        // Panic Reason Title MSG: Early Kmalloc
        PANIC("Can't kmalloc yet as memory management routines aren't initialized");
    }

    if (align == 1 && (placement_address & 0xFFFFF000))
    {
        // If the address is not already page-aligned
        // Align it.
        placement_address &= 0xFFFFF000; // Set to current page
        placement_address += 0x1000;     // Advance one page
    }

    if (phys)
    {
        *phys = placement_address;
    }

    uint32_t tmp = placement_address;
    placement_address += sz;
    return tmp;
}

uint32_t e_kmalloc_a(uint32_t sz)
{
    return kmalloc_imp(sz, 1, NULL);
}

uint32_t e_kmalloc_p(uint32_t sz, uint32_t *phys)
{
    return kmalloc_imp(sz, 0, phys);
}

uint32_t e_kmalloc_ap(uint32_t sz, uint32_t *phys)
{
    return kmalloc_imp(sz, 1, phys);
}

uint32_t e_kmalloc(uint32_t sz)
{
    return kmalloc_imp(sz, 0, NULL);
}

uint32_t disable_early_kmalloc()
{
    uint32_t end = placement_address;
    if(!(end & 0xFFF)) return end;

    end &= 0xFFFFF000; // Set to current page
    end += 0x1000;
    enabled = 0;
    return end;
}