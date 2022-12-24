struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 1 << 24);
	__uint(pinning, LIBBPF_PIN_BY_NAME);
} events SEC(".maps");
