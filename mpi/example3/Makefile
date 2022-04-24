#
# Makefile to build example MPI programs 
#

CC=mpiicc

CFLAGS=-Wall

EXE1=dartboard_pi_send.exe
EXE2=serial_trapezoid.exe
EXE3=send_trapezoid.exe
EXES=$(EXE1) $(EXE2) $(EXE3) 

all: $(EXES)

$(EXES): %.exe : %.c
	$(CC) $(CFLAGS) -o $@ $^
run:
	sbatch mpi_submit
	watch -n 1 squeue -u az16408
cancel:
	scancel -u az16408
cat:
	cat OUT.o

.PHONY: clean all

clean:
	\rm -f $(EXES)
	\rm -f *.o

