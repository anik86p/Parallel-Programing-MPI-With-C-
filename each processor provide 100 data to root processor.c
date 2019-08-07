#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
int size,rank;
int *recvB,sentB[100],*recvC,*display;
int sentCount;
void sent_100_element_to_root(int *sentB,int *recvB)
{
     recvC=(int*)malloc(sizeof(int)*size*110);
    display=(int*)malloc(sizeof(int)*110);
    for(int i=0;i<size;++i)
     {
        recvC[i]=100;
        display[i]=i*110;
     }
    //}

    
    for(int i=0;i<100;i++)
    {
        sentB[i]=i+10;
    }
    MPI_Gatherv(sentB,100,MPI_INT,recvB,recvC,display,MPI_INT,0,MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(&argc,&argv);        
    
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  
    
    //if(rank==0)
    //{
    recvB=(int*)malloc(size*110*sizeof(int));
    memset(recvB,0,sizeof(int)*size*110);
    
    sent_100_element_to_root(sentB,recvB);
    
        if(rank==0)
        {
            for(int i=0;i<400;i++)
            {
                printf("the data of recv %d buffer is %d \n",i,recvB[i]);
                // MPI_Barrier(MPI_COMM_WORLD);
            }
        }
    

     MPI_Finalize();
}