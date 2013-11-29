.text
.globl HAL_INTR_ENTRY,IRQ_stack
HAL_INTR_ENTRY:
#    mov $IRQ_stack,%eax
#    mov %esp,(%eax)
    pushl %esp
    call intr_special
    popl %eax
    mov 0x4(%esp),%eax
    pushl %eax
    mov $intr_c_entry,%eax
    call (%eax)
    call acoral_intr_exit
    popl %eax
    ret
