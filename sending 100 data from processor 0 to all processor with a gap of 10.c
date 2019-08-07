Process 0 sends 100 elements to each process i such that there is a gap of 10 elements between 
neighboring send blocks.
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
int size,rank;
int *recvB,*sentB,*sentC,*display;
int sentCount;
void sent_100_element_to_each_processor(int *sentB,int *recvB)
{
    sentC=(int*)malloc(sizeof(int)*size);
    display=(int*)malloc(sizeof(int)*size);
    for(int i=0;i<size;i++)
     {
        sentC[i]=100;
        display[i]=i*110;
     } 
   
    MPI_Scatterv(sentB,sentC,display,MPI_INT,recvB,100,MPI_INT,0,MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc,&argv);        
    
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  
    
    //if(rank==0)
    //{
    //recvB=(int*)malloc(size*110*sizeof(int));
    
 
    recvB=(int*)malloc(110*sizeof(int));
    memset(recvB,10,sizeof(int)*100*size);
    if(rank==0)
    {
    sentB=(int*)malloc(size*400*110*sizeof(int));
     for(int i=0;i<400;i++)
    {
        sentB[i]=i;
    }
    }

 sent_100_element_to_each_processor(sentB,recvB);

      for(int i=0;i<size;i++)
    {
        if(rank==i)
        {
           
                for(int j=0;j<100;j++)
                {
                    printf("data from processor %d is %d \n",rank,recvB[j]);
                }
                printf("\n");
        }
        ///MPI_Barrier(MPI_COMM_WORLD);
    } 
        
    

     MPI_Finalize();
}