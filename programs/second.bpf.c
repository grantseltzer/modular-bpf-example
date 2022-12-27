#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

#include "maps.bpf.h"

SEC("fentry/do_unlinkat")
int fentry__do_unlinkat()
{
    int *e;
	e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e) {
        return 0;
    }
    *e = 1;

    bpf_ringbuf_submit(e, 0);

	return 0;
}

char LICENSE[] SEC("license") = "GPL";