BINDIR=bin
BPFDIR=ebpf
BPFBIN=${BINDIR}/bpf

.PHONY: all
.default: ebpf

ebpf: ${BPFBIN}
	clang -O2 -g -Wall -emit-llvm -c ${BPFDIR}/ebpf.c -o ${BPFBIN}/ebpf.bc
	llc -march=bpf -mcpu=probe -filetype=obj ${BPFBIN}/ebpf.bc -o ${BPFBIN}/ebpf.o

${BINDIR}:
	mkdir -p ${BINDIR}

${BPFBIN}:
	mkdir -p ${BPFBIN}

clean:
	rm -r ${BINDIR}
