#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  char msg[36];
  char rmsg[36];
   MPI_Status status,status2;
  strcpy(msg,"this is anik islam from processor 0");
  int size=sizeof(msg);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

 int numbers = 5;
 
  int Nums=0,total=0,total2=0;
  
  if (world_rank == 0) 
  {

    MPI_Send(&numbers, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); 
    MPI_Send(&msg, size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
  } 
  else if (world_rank == 1) 
  {
    MPI_Recv(&Nums, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    MPI_Recv(&rmsg, size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status2);
    MPI_Get_count(&status,MPI_INT,&total );
    MPI_Get_count(&status2,MPI_CHAR,&total2 );
    printf("the string is %s und total recvd data is %d",rmsg,total2);
    printf("the number is %d und total recvd data is %d",Nums,total);
  }


  
   
   MPI_Barrier(MPI_COMM_WORLD);
   MPI_Finalize();
}