#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

#include "maps.bpf.h"
#include "types.h"

SEC("fentry/do_unlinkat")
int BPF_PROG(do_unlinkat, int dfd, struct filename *name)
{
    struct event *e;
	e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    e->x = 1;
	return 0;
}

char LICENSE[] SEC("license") = "GPL";