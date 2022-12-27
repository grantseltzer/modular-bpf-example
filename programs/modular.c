#include "first.skel.h"
#include "second.skel.h"
#include <signal.h>
#include <stdio.h>

static volatile bool exiting = false;

static void sig_handler(int sig)
{
	exiting = true;
}

static int handle_event(void *ctx, void *data, size_t data_sz)
{
	int *e = data;
    printf("%d\n", *e);
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
    opts.sz = sizeof(opts);
    opts.btf_custom_path = "/sys/kernel/btf/vmlinux";
    

    first_skel = first_bpf__open_and_load();
    if (!first_skel) {
		fprintf(stderr, "Failed to open BPF skeleton\n");
		return 1;
	}

    first_bpf__attach(first_skel);

    struct second_bpf *second_skel;
    second_skel = second_bpf__open_and_load();
    if (!second_skel) {
		fprintf(stderr, "Failed to open BPF skeleton\n");
		return 1;
	}

    second_bpf__attach(second_skel);

    rb = ring_buffer__new(bpf_map__fd(second_skel->maps.events), handle_event, NULL, NULL);
    int err;
    while(!exiting) {
        err = ring_buffer__poll(rb, 100);
        if (err == -EINTR) {
            err = 0;
            break;
        }
        if (err < 0) {
			printf("Error polling ring buffer: %d\n", err);
 			break;
 		}
 	}
    
}
