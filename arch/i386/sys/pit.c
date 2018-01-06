#include <stdint.h>
#include <i386/pit.h>

void pit_irq(void) {
    asm volatile("add $0x1c, %esp");
    asm volatile("pusha");
    pic_send_eoi(0);
    asm volatile("popa");
    asm volatile("iret");
}

static inline void __pit_send_cmd(uint8_t cmd) {
    outb(PIT_REG_COMMAND, cmd);
}

static inline void __pit_send_data(uint16_t data, uint8_t counter){
    uint8_t port = (counter==PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER0 :
        ((counter==PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER1 : PIT_REG_COUNTER2);

    outb (port, (uint8_t)data);
}

static inline uint8_t __pit_read_data (uint16_t counter) {
    uint8_t port = (counter==PIT_OCW_COUNTER_0) ? PIT_REG_COUNTER0 :
        ((counter==PIT_OCW_COUNTER_1) ? PIT_REG_COUNTER1 : PIT_REG_COUNTER2);

    return inb(port);
}

static void pit_start_counter (uint32_t freq, uint8_t counter, uint8_t mode) {
    if (freq==0) {
        return;
    }
#ifdef DBG_PIT
    printf("Starting counter %d with frequency %dHz\n", counter/0x40, freq);
#endif
    uint16_t divisor = (uint16_t)( 1193181 / (uint16_t)freq);
    uint8_t ocw = 0;
    ocw = (ocw & ~PIT_OCW_MASK_MODE) | mode;
    ocw = (ocw & ~PIT_OCW_MASK_RL) | PIT_OCW_RL_DATA;
    ocw = (ocw & ~PIT_OCW_MASK_COUNTER) | counter;
    __pit_send_cmd (ocw);
    __pit_send_data (divisor & 0xff, 0);
    __pit_send_data ((divisor >> 8) & 0xff, 0);
}

void pit_init(void) {
#ifdef DBG_PIT
    printk("Starting PIT initialization...\n");
#endif
    irq_install_handler(32, (uint32_t)pit_irq);
    pit_start_counter (200,PIT_OCW_COUNTER_0, PIT_OCW_MODE_SQUAREWAVEGEN);
#ifdef DBG_PIT
    printk("Started PIT successfully...\n");
#endif
}