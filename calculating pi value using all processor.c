#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include<stdio.h>

int main()
{
   int j;
   int intervals = 1000000000;
   MPI_Init(NULL, NULL);
   double delta,recv, x, pi = 0.0,total_result=0.0;
   int rank,size;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   delta = 1.0 / (double)intervals;

   for (j = rank; j <= intervals; j+=size)
   {
      x = ((double) j - 0.5) * delta;
      pi += 4.0 / (1.0 + (x * x));
         
   }
   

   printf("%f",pi*delta);
   
  
   
   if(rank==0)
   {
       for(int i=1;i<size;++i)
       {
       MPI_Recv(&recv,1,MPI_DOUBLE,i,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
       total_result+=recv;

       }
   }
   else 
   {
        MPI_Send(&pi,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
   }
   if(rank==0)
       printf ("Ï€ = %.10f\n", total_result * delta);
   MPI_Finalize();
   return 0;
}