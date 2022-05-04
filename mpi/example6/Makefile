#
# Makefile to build example MPI programs 
#

CC=mpiicc

CFLAGS=-Wall

EXE1=broadcast.exe
EXE2=reduce_trapezoid.exe
EXE3=scatter_gather.exe
EXES=$(EXE1) $(EXE2) $(EXE3)

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean all

run:
	sbatch mpi_submit
	watch -n 1 squeue -u az16408
	# while true; do squeue -u az16408;date ; sleep 1; done

cancel:
	scancel -u az16408
cat:
	cat OUT.o

clean:
	\rm -f $(EXES) 
	\rm -f *.o
