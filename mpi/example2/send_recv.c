/*
** Sending simple, point-to-point messages.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h" 

#define MASTER 0

void SPMD(int myrank, int size, int dest, int source, int tag, MPI_Status status, char *send_type);
void round_robin(int myrank, int size, int dest, int source, int tag, MPI_Status status, char *send_type);
void linear_exchange(int myrank, int size, int tag, MPI_Status status, char *send_type);
void this_pattern(int myrank, int size, int tag, MPI_Status status, char *send_type);

int main(int argc, char* argv[])
{
  int myrank;
  int size;
  int dest;              /* destination rank for message */
  int source;            /* source rank of a message */
  int tag = 0;           /* scope for adding extra information to a message */
  MPI_Status status;     /* struct used by MPI_Recv */

  /* MPI_Init returns once it has started up processes */
  MPI_Init( &argc, &argv );

  /* size and rank will become ubiquitous */ 
  MPI_Comm_size( MPI_COMM_WORLD, &size );
  MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

  /*Uncomment the below lines to try out different communication patterns*/
  SPMD(myrank,size,dest,source,tag,status,"char *");
  // round_robin(myrank,size,dest,source,tag,status,"int");
  // round_robin(myrank,size,dest,source,tag,status,"char *");
  // linear_exchange(myrank,size,tag,status,"char");
  // this_pattern(myrank,size,tag,status,"char");

  /* don't forget to tidy up when we're done */
  MPI_Finalize();

  /* and exit the program */
  return EXIT_SUCCESS;
}

void SPMD(int myrank, int size, int dest, int source, int tag, MPI_Status status, char *send_type){
  /* 
  ** SPMD - conditionals based upon rank
  ** will also become ubiquitous
  */
  if(send_type=="char *"){
    char message[BUFSIZ];

    if (myrank != MASTER) {  /* this is _NOT_ the master process */
      /* create a message to send, in this case a character array */
      sprintf(message, "Come-in Danny-Boy, this is process %d!", myrank);
      /* send to the master process */
      dest = MASTER;
      /* 
      ** Send our first message!
      ** use strlen()+1, so that we include the string terminator, '\0'
      */
      MPI_Send(message,strlen(message)+1, MPI_CHAR, dest, tag, MPI_COMM_WORLD);

    }
    else {             /* i.e. this is the master process */
      printf("messages recieved by rank %d (master):\n", myrank);
      /* loop over all the over processes */
      for (source=1; source<size; source++) {
        /* recieving their messages.. */
        MPI_Recv(message, BUFSIZ, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
        /* and then printing them */
        printf("recieved message: %s\n", message);
      }
    }
  }

}
void round_robin(int myrank, int size, int dest, int source, int tag, MPI_Status status, char *send_type){
  /* 
  ** round-robin : eg for 4 processes 0 -> 1 -> 2 -> 3 -> 0
  */
  if (send_type=="int"){

    int buffer_send_int = myrank+10;
    int buffer_recv_int;
    int tag_send = tag;
    int tag_recv = tag_send;
    dest = (myrank == size-1) ? 0 : myrank+1;
    source = (myrank == 0) ? size-1 : myrank-1;
    // Issue the send + receive at the same time
    printf("MPI process %d sends value %d to MPI process %d.\n", myrank, buffer_send_int, dest);
    MPI_Sendrecv(&buffer_send_int, 1, MPI_INT, dest, tag_send,
                  &buffer_recv_int, 1, MPI_INT, source, tag_recv, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("MPI process %d received value %d from MPI process %d.\n", myrank, buffer_recv_int, source);

  }else if (send_type=="char *"){

    char buffer_send_char[BUFSIZ];
    sprintf(buffer_send_char, "Come-in Danny-Boy, this is process %d!", myrank);
    char buffer_recv_char[BUFSIZ];
    int tag_send = tag;
    int tag_recv = tag_send;
    dest = (myrank == size-1) ? 0 : myrank+1;
    source = (myrank == 0) ? size-1 : myrank-1;
    // Issue the send + receive at the same time
    printf("MPI process %d sends value %s to MPI process %d.\n", myrank, buffer_send_char, dest);
    MPI_Sendrecv(buffer_send_char, strlen(buffer_send_char)+1, MPI_CHAR, dest, tag_send,
                  buffer_recv_char, BUFSIZ, MPI_CHAR, source, tag_recv, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("MPI process %d received value %s from MPI process %d.\n", myrank, buffer_recv_char, source);

  }else {
    printf("Invalide type, please choose: \"int\" | \"char *\" as the send type\n");
  }

}

void linear_exchange(int myrank, int size, int tag, MPI_Status status, char *send_type){
  /* 
  ** linear exchange : <- 0 <-> 1 <-> 2 <-> 3 ->
  */ 
  if (send_type=="char"){
    char buffer_send[BUFSIZ]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};;
    char buffer_recv;
    int tag_send = tag; 
    int tag_recv = tag_send;
    int right;
    int left;

    /*sending a message between myrank and myrank+1 to each other*/
    right = (myrank + 1) % size;
    left = (myrank == 0) ? (myrank + size - 1) : (myrank - 1);
    // Issue the send + receive at the same time
    printf("MPI process %d sends value %c to MPI process %d.\n",myrank,buffer_send[myrank],right);
    MPI_Sendrecv(&buffer_send[myrank], sizeof(buffer_send[myrank]), MPI_CHAR, right, tag_send,
                  &buffer_recv, sizeof(buffer_recv), MPI_CHAR, left, tag_recv, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("MPI process %d receives value %c from MPI process %d.\n",myrank,buffer_recv,left);

    printf("MPI process %d sends value %c to MPI process %d.\n",myrank,buffer_send[myrank],left);
    MPI_Sendrecv(&buffer_send[myrank], sizeof(buffer_send[myrank]), MPI_CHAR, left, tag_send,
                  &buffer_recv, sizeof(buffer_recv), MPI_CHAR, right, tag_recv, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("MPI process %d receives value %c from MPI process %d.\n",myrank,buffer_recv,right);

  }
}

void this_pattern(int myrank, int size, int tag, MPI_Status status, char *send_type) {
  /*
  arrange for communication to be in the pattern:
      ```
    0 <-> 1
    ^     ^
    |     |
    v     v
    2 <-> 3
      ```
  */
  if (send_type=="char"){
    char buffer_send[BUFSIZ]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};;
    char buffer_recv;
    int tag_send = tag; 
    int tag_recv = tag_send;
    int right;
    int left;

    /*sending a message between the ranks specified above*/
    switch (myrank) {
      case 0:
        right = 1;
        left = 2;
        break;
      case 1:
        right = 3;
        left = 0;
        break;
      case 2:
        right = 0;
        left = 3;
        break;
      case 3:
        right = 2;
        left = 1;
        break;
      // default:

    }
  
    // Issue the send + receive at the same time
    printf("MPI process %d sends value %c to MPI process %d.\n",myrank,buffer_send[myrank],right);
    MPI_Sendrecv(&buffer_send[myrank], sizeof(buffer_send[myrank]), MPI_CHAR, right, tag_send,
                  &buffer_recv, sizeof(buffer_recv), MPI_CHAR, left, tag_recv, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("MPI process %d receives value %c from MPI process %d.\n",myrank,buffer_recv,left);

    printf("MPI process %d sends value %c to MPI process %d.\n",myrank,buffer_send[myrank],left);
    MPI_Sendrecv(&buffer_send[myrank], sizeof(buffer_send[myrank]), MPI_CHAR, left, tag_send,
                  &buffer_recv, sizeof(buffer_recv), MPI_CHAR, right, tag_recv, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("MPI process %d receives value %c from MPI process %d.\n",myrank,buffer_recv,right);
  }
}
