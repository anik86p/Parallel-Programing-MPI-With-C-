#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include<stdio.h>

int main()
{

   MPI_Init(NULL, NULL);
   int rank,size=0;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   int *value;
  if(rank==0)
  {
     value=(int*)malloc(sizeof(int)*100000);
     for(int j=0;j<100000;j++)
     {
         value[j]=5+j;
     }
  }
  
  int slice=100000/size;
  int recv[slice];
  MPI_Scatter(value,slice,MPI_INT,&recv,slice,MPI_INT,0,MPI_COMM_WORLD); 
  int sum=0;
  for(int i=0;i<slice;i++)
  {
      sum+=recv[i];
  }
  int data_recv_root;
  MPI_Reduce(&sum,&data_recv_root,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
  if(rank==0)
  {
  printf("%d",data_recv_root/100000);
  }
  MPI_Finalize();
   return 0;
}