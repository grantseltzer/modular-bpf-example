#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

#include "maps.bpf.h"
#include "types.h"

SEC("fexit/do_unlinkat")
int fexit__do_unlinkat() 
{
    struct event *e;
	e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    e->x = 2;
	return 0;
}

char LICENSE[] SEC("license") = "GPL";