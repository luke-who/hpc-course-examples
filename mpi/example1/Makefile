#
# Makefile to build example MPI programs
#

CC=mpiicc

CFLAGS=-Wall

EXES=hello_world_c


hello_world_c: hello_world.c
	$(CC) $(CFLAGS) -o $@ $^

all: $(EXES)

run:
	sbatch mpi_submit
	watch -n 1 squeue -u az16408

cat:
	cat OUT.o

.PHONY: clean all

clean:
	\rm -f $(EXES)
	\rm -f *.o
