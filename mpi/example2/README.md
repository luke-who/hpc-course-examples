Example 2: Basic Communication
==============================

In this directory we have our first message passing examples.

send_recv
---------

This is a very simple program in which all the processes with ranks > 0
send a message to the master process (`0`), which duly receives them and
prints them to stdout.

deadlock
--------

This program highlights the pitfall of two or more process waiting for
events that will never happen—deadlock.

The program can function in two modes:

1\) Both processes call `MPI_send()` first, or <br />
0\) Both processes call `MPI_Recv()` first.

`MPI_Recv()` is a blocking function, meaning that it cannot return until
a message has been received.
 If all our processes make a call to
`MPI_Recv()`, without any messages being sent, then the overall MPI cohort
will deadlock.

The situation is less clear-cut for `MPI_Send()`.
This is also a blocking
function, but it may have access to a system buffer.
If so, it _may_ be able
(we don't know the system buffer size limits) to deposit the message in
the system buffer, and return, in the hope that a subsequent call to `MPI_Recv()`
by another process will retrieve the message from the buffer.

If, however, there is not a system buffer, and the destination process does not
call `MPI_Recv()`, then we will have a deadlock in this situation too.

simple-pingpong
---------------

This program uses repeated calls to blocking send and recv functions to estimate
the latency and apparent bandwidth of the MPI setup (inc. possibly interconnect)
in use.
Note that the physical location of the resources requested through the
queuing system will likely influence the estimates.

Exercise
--------

Modify the send-recv program to experiment with different communication patterns.
For example:

1. 'round-robin', where each `rank` sends to `rank+1`, which duly receives,
   but don't forget to warp back around to rank `0`.
2. 'linear exchange', like the round-robin, but this time messages are
   sent between any given pair of processors in both directions.
3. arrange for communication to be in the pattern:

    ```
	0 <-> 1
	^     ^
	|     |
	v     v
	2 <-> 3
    ```

Watch out for deadlocks!

