static __always_inline int push(__u8* value, __u8 stack[512], int* top) {

    // Uninitialized stack
    if (*top == -1) {
        stack[0] = *value;
        *top = 0;
    } else if (*top == STACK_SIZE-1) {
        // stack is full
        bpf_printk("stack is full, can't push");
        return 1;
    } else {
        stack[(*top) + 1] = *value;
        (*top) += 1;
    }
    return 0;
}

static __always_inline __u8 pop(__u8 stack[32], int* top) {
    __u32 popValue = 0;
    
    if((*top) == -1) {
        bpf_printk("stack is empty, can't pop");
        return 0;
    } else {
        popValue = stack[(*top)];
        *top = (*top) - 1;
    }
    return popValue;
}
