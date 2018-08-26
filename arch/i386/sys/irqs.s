.section .text
.align 4
.global irq0
.global irq1
.global irq2
.global irq3
.global irq4
.global irq5
.global irq6
.global irq7
.global irq8
.global irq9
.global irq10
.global irq11
.global irq12
.global irq13
.global irq14
.global irq15

# 32: IRQ0
irq0:
    cli
    push $0
    push $32
    jmp irq_common_stub

# 33: IRQ1
irq1:
    cli
    push $0
    push $33
    jmp irq_common_stub

# 34: IRQ2
irq2:
    cli
    push $0
    push $34
    jmp irq_common_stub

# 35: IRQ3
irq3:
    cli
    push $0
    push $35
    jmp irq_common_stub

# 36: IRQ4
irq4:
    cli
    push $0
    push $36
    jmp irq_common_stub

# 37: IRQ5
irq5:
    cli
    push $0
    push $37
    jmp irq_common_stub

# 38: IRQ6
irq6:
    cli
    push $0
    push $38
    jmp irq_common_stub

# 39: IRQ7
irq7:
    cli
    push $0
    push $39
    jmp irq_common_stub

# 40: IRQ8
irq8:
    cli
    push $0
    push $40
    jmp irq_common_stub

# 41: IRQ9
irq9:
    cli
    push $0
    push $41
    jmp irq_common_stub

# 42: IRQ10
irq10:
    cli
    push $0
    push $42
    jmp irq_common_stub

# 43: IRQ11
irq11:
    cli
    push $0
    push $43
    jmp irq_common_stub

# 44: IRQ12
irq12:
    cli
    push $0
    push $44
    jmp irq_common_stub

# 45: IRQ13
irq13:
    cli
    push $0
    push $45
    jmp irq_common_stub

# 46: IRQ14
irq14:
    cli
    push $0
    push $46
    jmp irq_common_stub

# 47: IRQ15
irq15:
    cli
    push $0
    push $47
    jmp irq_common_stub

.extern x86_irq_handler
.type x86_irq_handler, @function

# This is our common IRQ stub. It saves the processor state, sets
# up for kernel mode segments, calls the C-level fault handler,
# and finally restores the stack frame.
irq_common_stub:
    pusha
    push %ds
    push %es
    push %fs
    push %gs
    mov $0x10, %ax # Load the Kernel Data Segment descriptor!
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %esp, %eax # Push the stack
    cld
    push %esp
    call x86_irq_handler
    add $4, %esp
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa
    add $8, %esp # Cleans up the pushed error code and pushed ISR number
    iret # pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP!
