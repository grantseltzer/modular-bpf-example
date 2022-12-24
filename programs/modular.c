#include "first.skel.h"
#include "second.skel.h"
#include "types.h"
#include <signal.h>

static volatile bool exiting = false;

static void sig_handler(int sig)
{
	exiting = true;
}

static int handle_event(void *ctx, void *data, size_t data_sz)
{
	const struct event *e = data;
    printf("%d\n", e->x);
	return 0;
}

int main()
{

    signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	struct ring_buffer *rb = NULL;
    struct ring_buffer *events = NULL;
    struct first_bpf *first_skel;

    struct bpf_object_open_opts opts;
    opts.btf_custom_path = "/sys/kernel/btf/vmlinux";

    first_skel = first_bpf__open_opts(&opts);
    if (!first_skel) {
		fprintf(stderr, "Failed to open BPF skeleton\n");
		return 1;
	}

    struct second_bpf *second_skel;
    second_skel = second_bpf__open_and_load();
    if (!second_skel) {
		fprintf(stderr, "Failed to open BPF skeleton\n");
		return 1;
	}

    rb = ring_buffer__new(bpf_map__fd(second_skel->maps.events), handle_event, NULL, NULL);

    while(!exiting) {}
}
