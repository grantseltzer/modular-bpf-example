// struct bpf_map_def SEC(".maps") cookie_map = {
// 	.type        = BPF_MAP_TYPE_HASH,
// 	.key_size    = sizeof(u8),
// 	.value_size  = sizeof(instruction_set_t),
// 	.max_entries = 10, 
// };

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 10);
	__type(key, u8);
	__type(value, instruction_set_t);
} cookie_map SEC(".maps");

struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 1 << 24);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
} events SEC(".maps");