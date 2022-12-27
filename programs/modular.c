#include "first.skel.h"
#include "second.skel.h"
#include "maps.skel.h"
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

    int err;

	struct ring_buffer *rb = NULL;
    
    struct first_bpf *first_skel;
    struct second_bpf *second_skel;
    struct maps_bpf *map_skel;

    map_skel = maps_bpf__open_and_load();
    if (!map_skel) {
        fprintf(stderr, "Failed to open BPF skeleton\n");
		return 1;
    }

    first_skel = first_bpf__open_and_load();
    if (!first_skel) {
		fprintf(stderr, "Failed to open BPF skeleton\n");
		return 1;
	}

    second_skel = second_bpf__open_and_load();
    if (!second_skel) {
		fprintf(stderr, "Failed to open BPF skeleton\n");
		return 1;
	}

    first_bpf__attach(first_skel);
    second_bpf__attach(second_skel);

    rb = ring_buffer__new(bpf_map__fd(second_skel->maps.events), handle_event, NULL, NULL);

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

    bpf_object__unpin_maps(map_skel->obj, "/sys/fs/bpf");
}
