#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

void scatter_triangular(float *row,float *matrix)
{
    int p,rank,howmany;
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int rcount =rank+1;
    int *scounts,*display;
    scounts=(int*)malloc(sizeof(int)*p);
    display=(int*)malloc(sizeof(int)*p);

    for(unsigned int i=0;i<p;i++)
    {
        scounts[i]=i+1;
        display[i]=i*p;
    }
    MPI_Scatterv(matrix,scounts,display,MPI_FLOAT,row,rcount,MPI_FLOAT,0,MPI_COMM_WORLD);
    

}
int main(int argc,char **argv)
{
    int rank,p;
    float *row;
    float *matrix;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&p);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    if(rank==0)
    {
        int cnt=0;
    
    matrix=(float*)malloc(sizeof(float)*p*p);
    for(unsigned int i=0;i<p;++i)
     {
        for(unsigned int j=0;j<p;++j)
        {
            matrix[i*p+j]=++cnt;
        }

     }
    }
    row=(float*)malloc(sizeof(float)*p);
    memset(row,0,sizeof(float)*p);           

    scatter_triangular(row,matrix);
    if(rank==0)
        printf("single rows:\ n");
    for(int i=0;i<p;i++)
    {
        if(rank==i)
        {
            printf("Row %d",rank);
                for(int j=0;j<p;j++)
                {
                    printf("%4.1f",row[j]);
                }
                printf("\n");
        }
        MPI_Barrier(MPI_COMM_WORLD);
    } 
    

     MPI_Finalize();
}