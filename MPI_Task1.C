#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processor\n",
           processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
}
//output
..................................................................................................................................
Hello world from processor cetus1, rank 0 out of 4 processor
Hello world from processor cetus2, rank 2 out of 4 processor
Hello world from processor cetus2, rank 3 out of 4 processor
Hello world from processor cetus1, rank 1 out of 4 processor
...................................................................................................................................


// MPI_Send, MPI_Recv example. Communicates the number -1 from process 0
// to process 1.
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int number;
  if (world_rank == 0) {
    // If we are rank 0, set the number to -1 and send it to process 1
    number = -1;
    MPI_Send(
      /* data         = */ &number, 
      /* count        = */ 1, 
      /* datatype     = */ MPI_INT, 
      /* destination  = */ 1, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD);
  } else if (world_rank == 1) {
    MPI_Recv(
      /* data         = */ &number, 
      /* count        = */ 1, 
      /* datatype     = */ MPI_INT, 
      /* source       = */ 0, 
      /* tag          = */ 0, 
      /* communicator = */ MPI_COMM_WORLD, 
      /* status       = */ MPI_STATUS_IGNORE);
    printf("Process 1 received number %d from process 0\n", number);
  }
  MPI_Finalize();
}
//output
..................................................................................................................................
Process 1 received number -1 from process 0
..................................................................................................................................
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
}
......................................................................................................................
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  const int PING_PONG_LIMIT = 10;

  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // We are assuming at least 2 processes for this task
  if (world_size != 2) {
    fprintf(stderr, "World size must be two for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int ping_pong_count = 0;
  int partner_rank = (world_rank + 1) % 2;
  while (ping_pong_count < PING_PONG_LIMIT) {
    if (world_rank == ping_pong_count % 2) {
      // Increment the ping pong count before you send it
      ping_pong_count++;
      MPI_Send(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD);
      printf("%d sent and incremented ping_pong_count %d to %d\n",
             world_rank, ping_pong_count, partner_rank);
    } else {
      MPI_Recv(&ping_pong_count, 1, MPI_INT, partner_rank, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
      printf("%d received ping_pong_count %d from %d\n",
             world_rank, ping_pong_count, partner_rank);
    }
  }
  MPI_Finalize();
}
........................................................................................................

.................................................................................................
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  if (world_size != 2) {
    fprintf(stderr, "Must use two processes for this example\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  const int MAX_NUMBERS = 100;
  int numbers[MAX_NUMBERS];
  int number_amount;
  if (world_rank == 0) {
    // Pick a random amount of integers to send to process one
    srand(time(NULL));
    number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;
    // Send the amount of integers to process one
    MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("0 sent %d numbers to 1\n", number_amount);
  } 
  else if (world_rank == 1) {
    MPI_Status status;
    // Receive at most MAX_NUMBERS from process zero
    MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    // After receiving the message, check the status to determine how many
    // numbers were actually received
    MPI_Get_count(&status, MPI_INT, &number_amount);
    // Print off the amount of numbers, and also print additional information
    // in the status object
    printf("1 received %d numbers from 0. Message source = %d, tag = %d\n",
           number_amount, status.MPI_SOURCE, status.MPI_TAG);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
................................................................................................................
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void my_bcast(void* data, int count, MPI_Datatype datatype, int root,
              MPI_Comm communicator) {
  int world_rank;
  MPI_Comm_rank(communicator, &world_rank);
  int world_size;
  MPI_Comm_size(communicator, &world_size);

  if (world_rank == root) {
    // If we are the root process, send our data to everyone
    int i;
    for (i = 0; i < world_size; i++) {
      if (i != world_rank) {
        MPI_Send(data, count, datatype, i, 0, communicator);
      }
    }
  } else {
    // If we are a receiver process, receive the data from the root
    MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
  }
}

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int data;
  if (world_rank == 0) {
    data = 100;
    printf("Process 0 broadcasting data %d\n", data);
    my_bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    my_bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received data %d from root process\n", world_rank, data);
  }

  MPI_Finalize();
}
........................................................................................................................
Although the root process and receiver processes do different jobs, they all call the same MPI_Bcast function. When the root
 process (in our example, it was process zero) calls MPI_Bcast, the data variable will be sent to all other processes. When all 
 of the receiver processes call MPI_Bcast, the data variable will be filled in with the data from the root process.
........................................................................................................................
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
        int rank;
        int buf=0;
        const int root=0;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);

        if(rank == root) {
           buf = 777;
        }

        printf("[%d]: Before Bcast, buf is %d\n", rank, buf);

        /* everyone calls bcast, data is taken from root and ends up in everyone's buf */
        MPI_Bcast(&buf, 1, MPI_INT, root, MPI_COMM_WORLD);

        printf("[%d]: After Bcast, buf is %d\n", rank, buf);

        MPI_Finalize();
        return 0;
}
............................................................................................................................
#include <stdio.h>
#include "mpi.h"
// This example simply uses MPI_Bcast to broadcast a read in value to all other processes from root process
int main(argc, argv)
int argc;
char **argv;
{
    int rank, value;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //what rank is the current processor

    if (rank == 0) {
        // if root process we read the value to broadcast
        printf("Enter a number to broadcast:\n");
        scanf("%d", &value);
    } 
    else 
    {
        printf("process %d: Before MPI_Bcast, value is %d\n", rank, value); 
    }

    // each processor calls MPI_Bcast, data is broadcast from root processor and ends up in everyone value variable
    // root process uses MPI_Bcast to broadcast the value, each other process uses MPI_Bcast to receive the broadcast value
    MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);

    printf("process %d: After MPI_Bcast, value is %d\n", rank, value);

    MPI_Finalize();

    return 0;
}
..........................................................................................................................
MPI_Scatter is a collective routine that is very similar to MPI_Bcast (If you are unfamiliar with these terms, please
read the previous lesson). MPI_Scatter involves a designated root process sending data to all processes in a communicator. 
The primary difference between MPI_Bcast and MPI_Scatter is small but important. MPI_Bcast sends the same piece of data to
all processes while MPI_Scatter sends chunks of an array to different processes. Check out the illustration below for further
clarification.In the illustration, MPI_Bcast takes a single data element at the root process (the red box) and copies it to all
other processes. MPI_Scatter takes an array of elements and distributes the elements in the order of process rank. The first 
element (in red) goes to process zero, the second element (in green) goes to process one, and so on. Although the root process
(process zero) contains the entire array of data, MPI_Scatter will copy the appropriate element into the receiving buffer of 
the process. If send_count is one and send_datatype is MPI_INT, then process zero gets the first integer of the array, process
 one gets the second integer, and so on. If send_count is two, then process zero gets the first and second integers, process 
 one gets the third and fourth, and so on
..........................................................................................................................
MPI_Gather is the inverse of MPI_Scatter. Instead of spreading elements from one process to many processes, MPI_Gather takes 
elements from many processes and gathers them to one single process. This routine is highly useful to many parallel algorithms,
such as parallel sorting and searching. Below is a simple illustration of this algorithm.Similar to MPI_Scatter, MPI_Gather takes
elements from each process and gathers them to the root process. The elements are ordered by the rank of the process from which 
they were received. The function prototype for MPI_Gather is identical to that of MPI_Scatter.In MPI_Gather, only the root process
needs to have a valid receive buffer. All other calling processes can pass NULL for recv_data. Also, don’t forget that the recv_count
parameter is the count of elements received per process, not the total summation of counts from all processes. This can often confuse
beginning MPI programmers.
...........................................................................................................................
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>

// Creates an array of random numbers. Each number has a value from 0 - 1
float *create_rand_nums(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);
  int i;
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

// Computes the average of an array of numbers
float compute_avg(float *array, int num_elements)
 {
  float sum = 0.f;
  int i;
  for (i = 0; i < num_elements; i++)
   {
    sum += array[i];
  }
  return sum / num_elements;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: avg num_elements_per_proc\n");
    exit(1);
  }

  int num_elements_per_proc = atoi(argv[1]);
  // Seed the random number generator to get different results each time
  srand(time(NULL));

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Create a random array of elements on the root process. Its total
  // size will be the number of elements per process times the number
  // of processes
  float *rand_nums = NULL;
  if (world_rank == 0) {
    rand_nums = create_rand_nums(num_elements_per_proc * world_size);
  }

  // For each process, create a buffer that will hold a subset of the entire
  // array
  float *sub_rand_nums = (float *)malloc(sizeof(float) * num_elements_per_proc);
  assert(sub_rand_nums != NULL);

  // Scatter the random numbers from the root process to all processes in
  // the MPI world
  MPI_Scatter(rand_nums, num_elements_per_proc, MPI_FLOAT, sub_rand_nums,
              num_elements_per_proc, MPI_FLOAT, 0, MPI_COMM_WORLD);

  // Compute the average of your subset
  float sub_avg = compute_avg(sub_rand_nums, num_elements_per_proc);

  // Gather all partial averages down to the root process
  float *sub_avgs = NULL;
  if (world_rank == 0) {
    sub_avgs = (float *)malloc(sizeof(float) * world_size);
    assert(sub_avgs != NULL);
  }
  MPI_Gather(&sub_avg, 1, MPI_FLOAT, sub_avgs, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

  // Now that we have all of the partial averages on the root, compute the
  // total average of all numbers. Since we are assuming each process computed
  // an average across an equal amount of elements, this computation will
  // produce the correct answer.
  if (world_rank == 0) {
    float avg = compute_avg(sub_avgs, world_size);
    printf("Avg of all elements is %f\n", avg);
    // Compute the average across the original data for comparison
    float original_data_avg =
      compute_avg(rand_nums, num_elements_per_proc * world_size);
    printf("Avg computed across original data is %f\n", original_data_avg);
  }

  // Clean up
  if (world_rank == 0) {
    free(rand_nums);
    free(sub_avgs);
  }
  free(sub_rand_nums);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
...............................................................................................................



#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>
#include <stdlib.h>


int main(int argc, char** argv) {
        int *rand_nums = NULL;
        int *rand_nums2 = NULL;
        int *hold_data=NULL;
        int *hold_data2=NULL;
        MPI_Init(NULL, NULL);
        int rank,size;
        MPI_Comm MPI_COMM_WORLD;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
         MPI_Comm_size(MPI_COMM_WORLD, &size);
       
       
        rand_nums = (int *)malloc(sizeof(int)*size*1);
        rand_nums[0]=777;
        rand_nums[1]=778;
        rand_nums[2]=779;
        rand_nums[3]=710;

        rand_nums2 = (int *)malloc(sizeof(int)*size*1);
        rand_nums2[0]=888;
        rand_nums2[1]=875;
        rand_nums2[2]=569;
        rand_nums2[3]=874;
        
        hold_data = (int *)malloc(sizeof(int)*size*100); 
        
        hold_data2 = (int *)malloc(sizeof(int)*size*10000);
        
        MPI_Scatter(rand_nums,1,MPI_INT, hold_data,1, MPI_INT, 0,MPI_COMM_WORLD);
        
                    
        if(rank==0)
        {   
            printf("data from 0 process %d",hold_data[0]);
        }
        else if(rank==1)
        {   
            printf("data from 1 process %d",hold_data[0]);
        }
        else if(rank==2)
        {   
            printf("data from 2 process %d",hold_data[0]);
            
        }   
        else if(rank==3)
        {
            printf("data from 3 process %d",hold_data[0]);
         }
         //rank=0;
       // MPI_Gather(rand_nums2,1,MPI_INT, hold_data2,1, MPI_INT, 0,MPI_COMM_WORLD);
        //printf("data from 3 process %d",hold_data2[0]);

        //if(rank==0)
          { 
            //  for(int i=0;i<=sizeof(hold_data2);i++)
             // printf("data from root process %d",hold_data2[i]);
                
          }
        MPI_Finalize();
        return 0;
}
....................................................................................................
Reduce is a classic concept from functional programming. Data reduction involves reducing a set of numbers
into a smaller set of numbers via a function. For example, let’s say we have a list of numbers [1, 2, 3, 4, 5].
Reducing this list of numbers with the sum function would produce sum([1, 2, 3, 4, 5]) = 15. Similarly,
the multiplication reduction would yield multiply([1, 2, 3, 4, 5]) = 120.
MPI_Reduce(
    void* send_data,
    void* recv_data,
    int count,
    MPI_Datatype datatype,
    MPI_Op op,
    int root,
    MPI_Comm communicator)

The processes from the above illustration each have two elements. The resulting summation happens on a per-element 
basis. In other words, instead of summing all of the elements from all the arrays into one element, the ith element 
from each array are summed into the ith element in result array of process 0.




MPI_MAX - Returns the maximum element.
MPI_MIN - Returns the minimum element.
MPI_SUM - Sums the elements.
MPI_PROD - Multiplies all elements.
MPI_LAND - Performs a logical and across the elements.
MPI_LOR - Performs a logical or across the elements.
MPI_BAND - Performs a bitwise and across the bits of the elements.
MPI_BOR - Performs a bitwise or across the bits of the elements.
MPI_MAXLOC - Returns the maximum value and the rank of the process that owns it.
MPI_MINLOC - Returns the minimum value and the rank of the process that owns it.
....................................................................................................
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>
#include <time.h>

// Creates an array of random numbers. Each number has a value from 0 - 1
float *create_rand_nums(int num_elements) {
  float *rand_nums = (float *)malloc(sizeof(float) * num_elements);
  assert(rand_nums != NULL);
  int i;
  for (i = 0; i < num_elements; i++) {
    rand_nums[i] = (rand() / (float)RAND_MAX);
  }
  return rand_nums;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: avg num_elements_per_proc\n");
    exit(1);
  }

  int num_elements_per_proc = atoi(argv[1]); //atoi:convert string to int

  MPI_Init(NULL, NULL);

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Create a random array of elements on all processes.
  srand(time(NULL)*world_rank);   // Seed the random number generator to get different results each time for each processor
  float *rand_nums = NULL;
  rand_nums = create_rand_nums(num_elements_per_proc);

  // Sum the numbers locally
  float local_sum = 0;
  int i;
  for (i = 0; i < num_elements_per_proc; i++) {
    local_sum += rand_nums[i];
  }

  // Print the random numbers on each process
  printf("Local sum for process %d - %f, avg = %f\n",
         world_rank, local_sum, local_sum / num_elements_per_proc);

  // Reduce all of the local sums into the global sum
  float global_sum;
  MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0,
             MPI_COMM_WORLD);

  // Print the result
  if (world_rank == 0) {
    printf("Total sum = %f, avg = %f\n", global_sum,
           global_sum / (world_size * num_elements_per_proc));
  }

  // Clean up
  free(rand_nums);

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
............................................................................................................
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <assert.h>
#include <stdlib.h>


int main(int argc, char** argv) {
        int *rand_nums = NULL;
        
        int *hold_data=NULL;
        
        MPI_Init(NULL, NULL);
        int rank,size;
        MPI_Comm MPI_COMM_WORLD;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
         MPI_Comm_size(MPI_COMM_WORLD, &size);
       
       
        rand_nums = (int *)malloc(sizeof(int)*size*1);
        rand_nums[0]=10;
        rand_nums[1]=10;                  //every process provide the fired then st element so only 10 will be counted if any random data can be created then ok
        rand_nums[2]=10;
        rand_nums[3]=10;
       
        

        
        
        hold_data = (int *)malloc(sizeof(int)*size*100); 
        //int x=0;
                
        MPI_Reduce(rand_nums, hold_data, 1, MPI_INT,MPI_SUM, 0,MPI_COMM_WORLD);                 
        if(rank==0)
        {   
            printf("data from 0 process %d",hold_data[0]);
        }               
        MPI_Finalize();
        return 0;
}
.......................................................................................................
Many parallel applications will require accessing the reduced results across all processes rather than the
root process. In a similar complementary style of MPI_Allgather to MPI_Gather, MPI_Allreduce will reduce 
the values and distribute the results to all processes. The function prototype is the following:

MPI_Allreduce(
    void* send_data,
    void* recv_data,
    int count,
    MPI_Datatype datatype,
    MPI_Op op,
    MPI_Comm communicator)
As you might have noticed, MPI_Allreduce is identical to MPI_Reduce with the exception that it does not need 
a root process id (since the results are distributed to all processes). The following illustrates the communication
pattern of MPI_Allreduce is the equivalent of doing MPI_Reduce followed by an MPI_Bcast.
...........................................................................................................
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);

  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  if (world_size != 2) {
    fprintf(stderr, "Must use two processes for this example\n");
    MPI_Abort(MPI_COMM_WORLD, 1);
  }
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  const int MAX_NUMBERS = 100;
  int numbers[MAX_NUMBERS];
  int number_amount;
  if (world_rank == 0) {
    // Pick a random amount of integers to send to process one
    srand(time(NULL));
    number_amount = (rand() / (float)RAND_MAX) * MAX_NUMBERS;
    // Send the amount of integers to process one
    MPI_Send(numbers, number_amount, MPI_INT, 1, 0, MPI_COMM_WORLD);
    printf("0 sent %d numbers to 1\n", number_amount);
  } else if (world_rank == 1) {
    MPI_Status status;
    // Receive at most MAX_NUMBERS from process zero
    MPI_Recv(numbers, MAX_NUMBERS, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    // After receiving the message, check the status to determine how many
    // numbers were actually received
    MPI_Get_count(&status, MPI_INT, &number_amount);
    // Print off the amount of numbers, and also print additional information
    // in the status object
    printf("1 received %d numbers from 0. Message source = %d, tag = %d\n",
           number_amount, status.MPI_SOURCE, status.MPI_TAG);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
}
...............................................................................
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
..............................................................................................
processor 1  processor 2  processor 3  processor 4
MPI_Send()   MPI_Recv()   MPI_Send()   MPI_Send()
MPI_Recv()   MPI_Send()   MPI_Recv()   MPI_Recv()

..............................................................................................
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv) {
 MPI_Init(NULL, NULL);
 float *x;
 float *y;
 int blocksize=5;
 int i,p,my_rank,succ,pred,send_offset,recv_offset;
 MPI_Status status;
 MPI_Comm_size(MPI_COMM_WORLD,&p);
 MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
 for(int i=0;i<blocksize;i++)
 {
     y[i+my_rank*blocksize]=x[i];
 }
 succ=(my_rank+1)%p;
 pred=(my_rank-1+p)%p;
 for(int i=0;i<p-1;i++)
 {
     send_offset=((my_rank-i+p)%p)*blocksize;
     recv_offset=((my_rank-i-1+p)%p)*blocksize;
     MPI_Send(y+send_offset,blocksize,MPI_FLOAT,succ,0,MPI_COMM_WORLD);
     MPI_Recv(y+send_offset,blocksize,MPI_FLOAT,succ,0,MPI_COMM_WORLD,&status);
 }
 MPI_Barrier(MPI_COMM_WORLD);
 MPI_Finalize();
}

..................................................................................................................
#include <mpi.h>
int main(int argc, char *argv[ ])
{
MPI_Init(&argc, &argv);
int rank, size;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
int x = size ­-rank;
if (rank != 0)
{
MPI_Send(&x, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
}
int y = (rank + 1) % 2;
MPI_Bcast(&y, 1, MPI_INT, size-1, MPI_COMM_WORLD);
if (rank == 0)
{
int r = y + x;
for (int i = 1; i < size; i++)
{
MPI_Recv(&x, 1, MPI_INT, i, 0, MPI_COMM_WORLD,
MPI_STATUS_IGNORE);
r += x;
}
printf(“r = %d\n”, r);
}
MPI_Finalize( );
return 0;
}
..........................................................................................................
//ubung 8 task-1
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
//memset() is used to fill a block of memory with a particular value.
// ptr ==> Starting address of memory to be filled
// x   ==> Value to be filled
// n   ==> Number of bytes to be filled starting 
//         from ptr to be filled
//void *memset(void *ptr, int x, size_t n);
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
...............................................................................................
use of MPI_GatherV()
-each process provides 100 integer values
-the block received are stored in the received buffer such that there is a free gap of 10 elements between 2 blocks.
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
.....................................................................................................
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
...................................................................................................................
Another function is MPI_Comm_create. At first glance, this function looks very similar to MPI_Comm_create_group.
Its signature is almost identical:
 MPI_Comm_create(
    MPI_Comm comm,
    MPI_Group group,
    MPI_Comm* newcomm)
The key difference however (besides the lack of the tag argument), is that MPI_Comm_create_group is only collective
over the group of processes contained in group, where MPI_Comm_create is collective over every process in comm
.................................................................................................................
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
int world_rank, world_size;
MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
MPI_Comm_size(MPI_COMM_WORLD, &world_size);

int color =world_size-world_rank;//  world_rank/2;   
printf("color value is %d ",color);// Determine color based on row

// Split the communicator based on the color and use the
// original rank for ordering
MPI_Comm row_comm;
MPI_Comm_split(MPI_COMM_WORLD, color, world_rank, &row_comm);

int row_rank, row_size;
MPI_Comm_rank(row_comm, &row_rank);
MPI_Comm_size(row_comm, &row_size);

printf("WORLD RANK/SIZE: %d/%d \t ROW RANK/SIZE: %d/%d\n",
    world_rank, world_size, row_rank, row_size);

MPI_Comm_free(&row_comm);

    // Finalize the MPI environment.
    MPI_Finalize();
}
......................................................................................................................
color value is 2 WORLD RANK/SIZE:2/4 ROW RANK?SIZE:0/1
color value is 4 WORLD RANK/SIZE:0/4 ROW RANK?SIZE:0/1
color value is 1 WORLD RANK/SIZE:3/4 ROW RANK?SIZE:0/1
color value is 3 WORLD RANK/SIZE:1/4 ROW RANK?SIZE:0/1 
because different color value make different 4 sub groups have size 1 (only 1 process per group) and rank 0;
....................................
when int color =5;
color value is 5 WORLD RANK/SIZE:2/4 ROW RANK/SIZE:2/4
color value is 5 WORLD RANK/SIZE:0/4 ROW RANK/SIZE:0/4
color value is 5 WORLD RANK/SIZE:3/4 ROW RANK/SIZE:3/4
color value is 5 WORLD RANK/SIZE:1/4 ROW RANK/SIZE:1/4
if coler value is 5 or any const value like 100/200 or other they make a sub group(because they all have color value)
so the size is 4 and rank according world rank;
.....................................
if int color = world_rank/2;
color value is 1 WORLD RANK/SIZE:2/4 ROW RANK/SIZE:1/2
color value is 0 WORLD RANK/SIZE:0/4 ROW RANK/SIZE:1/2
color value is 0 WORLD RANK/SIZE:3/4 ROW RANK/SIZE:0/2
color value is 1 WORLD RANK/SIZE:1/4 ROW RANK/SIZE:0/2

world_rank/2 produce 0,0,1,1 (coz my pc have only 4 core) so 0,0 and 1,1 make 2 sub group 
so toatal size will be 2 so our new communicator have tatal size 2;
......................................................................
about key value 
so in new sub group we have total 4 process color=2 (or any constnt value) 
key==(world_size-world_rank)
processor root(0) provide (4-0)=4 ,rank in new sub group 3 
processor 1 provide (4-1)=3 ,rank in new sub group 2
processor 2 provide (4-2)=2 ,rank in new sub group 1
processor 3 provide (4-3)=1 rank in new sub group 0
(key=smaller makes  rank=smaller ,the lowest key value in a sub group make lowest rank value )

..........................................................
when
int color = world_rank/2;
int key=world_size-world_rank;
color value is 1 WORLD RANK/SIZE:2/4 ROW RANK/SIZE:1/2
color value is 0 WORLD RANK/SIZE:0/4 ROW RANK/SIZE:1/2
color value is 0 WORLD RANK/SIZE:1/4 ROW RANK/SIZE:0/2
color value is 1 WORLD RANK/SIZE:3/4 ROW RANK/SIZE:0/2
process 0 and process 1 make a group because they produce color value 0;
and key for process 0 is (4-0)=4 and process 1 (4-1)=3
so in the subgroup there is 2 process with key 4,3 and 
rank is 1,0.

process 2 and process 3 make different sub group because they 
produce color = 1 and key value for process 2  (4-2)=2 and process 
3  (4-3)=1 so there rank in sub group is 1,0
.................................................................................................
//ubung-8 task 3
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
int main(int argc ,char **argv)
{
 MPI_Comm cart,subcart;
 int pnr,pid,subpid,subpnr;
 int p,dir,src,dest;
 int dims[3]={2,3,4};
 int per[3]={0,0,0};
 int sub[3]={1,0,1};
 MPI_Init(&argc,&argv);
 MPI_Cart_create(MPI_COMM_WORLD,3,dims,per,1,&cart);
 MPI_Comm_size(cart,&pnr);
 MPI_Comm_rank(cart,&pid);
 for(p=0;p<pnr;p++)
 {
    if(p==pid)
    {
        printf("[%02d] my neigbours :",pid);
        for(dir=0;dir<3;dir++)
        {
            MPI_Cart_shift(cart,dir,1,&src,&dest);
            printf("[dimention %d] % 3d % 3d ",dir,src,dest);     
        }

    }
    fflush(stdout);
    MPI_Barrier(cart);
    
 }
 MPI_Cart_sub(cart,sub,&subcart);
 MPI_Comm_size(subcart,&subpnr);
 MPI_Comm_rank(subcart,&subpid);
 for(p=0;p<pnr;p++)
 {
     if(p==pid)
         printf("[%02d i am no % 2d out of % 2d in my y-plane\n",pid,subpid,subpnr);
 }
 MPI_Finalize();
}


//
Sends and receives a message//Input Parameters
sendbuf
initial address of send buffer (choice)
sendcount
number of elements in send buffer (integer)
sendtype
type of elements in send buffer (handle)
dest
rank of destination (integer)
sendtag
send tag (integer)
recvcount
number of elements in receive buffer (integer)
recvtype
type of elements in receive buffer (handle)
source
rank of source (integer)
recvtag
receive tag (integer)
comm
communicator (handle)
Output Parameters
recvbuf
initial address of receive buffer (choice)
status
status object (Status). This refers to the receive operation.
...........................................................................
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);
    // Get the number of processes
int coords[2],dims[2],period[2],source,dest;
int my_rank,reorder=0;
float a,b;
MPI_Comm comm_2d;
MPI_Status status;
MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
dims[0]=3;
dims[1]=4;
period[0]=period[1]=1;
MPI_Cart_create(MPI_COMM_WORLD,2,dims,period,reorder,&comm_2d);
MPI_Cart_coords(comm_2d,my_rank,2,coords);
for(int i=0;i<2;i++)
{
    printf("%f",coords[i]);               //coords[0]=0,coords[1]=0;means 0,0//coords[0]=0,coords[1]=1;means 0,1 cordinate and so on

}
MPI_Cart_shift(comm_2d,0,coords[1],&source,&dest);
a=my_rank;
MPI_Sendrecv(&a,1,MPI_FLOAT,dest,0,&b,1,MPI_FLOAT,source,0,comm_2d,&status);
    // Finalize the MPI environment.
    MPI_Finalize();
}
........................................................................................
int MPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype,
                int dest, int sendtag,
                void *recvbuf, int recvcount, MPI_Datatype recvtype,
                int source, int recvtag,
                MPI_Comm comm, MPI_Status *status)
..............................................................................................................
//6 th ubung,task-2.
//using sendrecv to transfer two blocking calls into only one blocking call doing the same operations
void Gather_ring (float x[], int size, float y[])
{
   for (i = 0; i < p - 1; i++)
   {
      MPI_Sendrecv(y + send_offset, size, ..., succ, 0,
                   y + recv_offset, size, ..., pred, 0,...);
   }
}

//using non-blocking communication
void Gather_ring (float x[], int size, float y[])
{
   for (i = 0; i < p - 1; i++)
   {
      MPI_Isend (... &send_requ);
      MPI_Irecv (... &recv_requ);
      MPI_Wait (&send_requ, &status);
      MPI_Wait (&recv_requ, &status);
   }
}

//global ordering/ hierarchy
void Gather_ring (float x[], int blocksize, float y[])
{
   for (i = 0; i < p - 1; i++)
   {
      if (my_rank == 0)
      {
         MPI_Send (...);
         MPI_Recv (...);
      }
      else
      {
         MPI_Recv (...);
         MPI_Send (...);
      }
   }
}

//classical with deadlock
void Gather_ring (float x[], int blocksize, float y[])
{
   for (i = 0; i < p - 1; i++)
   {
      MPI_Send (y + send_offset, blocksize, ..., succ, ...);
      MPI_Recv (y + recv_offset, blocksize, ..., pred, ...);
   }
}


int main(int argc, char** argv)
{
   x = (float*)calloc(MAX_SIZE , sizeof(float));
   y = (float*)calloc(MAX_SIZE * comm_size, sizeof(float));

   for(i = 1; i < MAX_SIZE; i *= 2)
   {
    Gather_ring(x, i, y);
   }
}
...............................................................................................................
//Ubung-7..task 2:
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
   double delta,recv, x, pi = 0.0;
   int rank,size;
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   delta = 1.0 / (double)intervals;

   for (j = rank; j <= intervals; j+=size) //for (j = rank; j <= intervals/size; j++)
   {
      x = ((double) j - 0.5) * delta;
      pi += 4.0 / (1.0 + (x * x));
   }

   printf("%f",pi*delta); // print .785398 4 times....
   MPI_Reduce(&pi,&recv,1,MPI_DOUBLE,MPI_SUM,0, MPI_COMM_WORLD);
   if(rank==0)
   printf ("Ï€ = %.10f\n", recv * delta);
   MPI_Finalize();
   return 0;
}
...............................................
//6 ubung /task-3
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

.................................................................................................
//7 no ubung,task:3
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
...............