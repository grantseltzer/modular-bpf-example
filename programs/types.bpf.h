struct event {
	__u64 array[MAX_OUTPUT_COUNT];
};
const struct event *unused __attribute__((unused));

typedef struct instruction {
    __u8 op;
    __u32 arg1;
} instruction_t;

typedef struct instruction_set {
    instruction_t ins[MAX_INSTRUCTIONS];
} instruction_set_t;