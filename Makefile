# Build bpf objects for maps, use_vm, second
# Use the bpf skeletons in userspace for loading and attaching
# Build libbpf.a
# Link userspace against libbpf.a

OUTPUT := $(abspath ./dist)
LIBBPF_SRC = ./libbpf/src
LIBBPF_OBJ := $(OUTPUT)/libbpf.a
INCLUDES := -I$(OUTPUT)
CFLAGS := -g -O2 -Wall

CLANG := clang
BPFTOOL := bpftool

all: $(OUTPUT)/use_vm.bpf.o $(OUTPUT)/example

.PHONY: $(LIBBPF_OBJ)
$(LIBBPF_OBJ): $(wildcard $(LIBBPF_SRC)/*.[ch] $(LIBBPF_SRC)/Makefile)
	$(MAKE) -C $(LIBBPF_SRC) BUILD_STATIC_ONLY=1 \
		OBJDIR=$(OUTPUT)/libbpf DESTDIR=$(OUTPUT)             \
		INCLUDEDIR= LIBDIR= UAPIDIR=                          \
		install

$(OUTPUT)/use_vm.bpf.o: programs/use_vm.bpf.c $(LIBBPF_OBJ)
	$(CLANG) -g -O2 -target bpf -D__TARGET_ARCH_$(ARCH) $(INCLUDES) -c programs/use_vm.bpf.c -o $@
	$(BPFTOOL) gen skeleton $@ > $(OUTPUT)/use_vm.skel.h

$(OUTPUT)/example: $(OUTPUT)/use_vm.bpf.o
	$(CLANG) programs/modular.c $(LIBBPF_OBJ) -lelf -lz $(INCLUDES) -o $@

clean:
	rm -rf $(OUTPUT)