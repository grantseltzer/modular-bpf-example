#include "use_vm.skel.h"
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
    
    struct use_vm_bpf *use_vm_skel;

    use_vm_skel = use_vm_bpf__open_and_load();
    if (!use_vm_skel) {
		fprintf(stderr, "Failed to open BPF skeleton\n");
		return 1;
	}

    use_vm_bpf__attach(use_vm_skel);

    rb = ring_buffer__new(bpf_map__fd(use_vm_skel->maps.events), handle_event, NULL, NULL);

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

    bpf_object__unpin_maps(use_vm_skel->obj, "/sys/fs/bpf");
}
