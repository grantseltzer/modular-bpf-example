# Build bpf objects for maps, first, second
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

all: $(OUTPUT)/first.bpf.o $(OUTPUT)/second.bpf.o $(OUTPUT)/example

.PHONY: $(LIBBPF_OBJ)
$(LIBBPF_OBJ): $(wildcard $(LIBBPF_SRC)/*.[ch] $(LIBBPF_SRC)/Makefile)
	$(MAKE) -C $(LIBBPF_SRC) BUILD_STATIC_ONLY=1 \
		OBJDIR=$(OUTPUT)/libbpf DESTDIR=$(OUTPUT)             \
		INCLUDEDIR= LIBDIR= UAPIDIR=                          \
		install

$(OUTPUT)/first.bpf.o: programs/first.bpf.c $(LIBBPF_OBJ)
	$(CLANG) -g -O2 -target bpf -D__TARGET_ARCH_$(ARCH) $(INCLUDES) -c programs/first.bpf.c -o $@
	$(BPFTOOL) gen skeleton $@ > $(OUTPUT)/first.skel.h

$(OUTPUT)/second.bpf.o: programs/second.bpf.c $(LIBBPF_OBJ)
	$(CLANG) -g -O2 -target bpf -D__TARGET_ARCH_$(ARCH) $(INCLUDES) -c programs/second.bpf.c -o $@
	$(BPFTOOL) gen skeleton $@ > $(OUTPUT)/second.skel.h

$(OUTPUT)/example: $(OUTPUT)/first.bpf.o $(OUTPUT)/second.bpf.o
	$(CLANG) programs/modular.c $(LIBBPF_OBJ) -lelf -lz $(INCLUDES) -o $@

clean:
	rm -rf $(OUTPUT)