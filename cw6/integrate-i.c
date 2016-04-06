// mpicc integrate.c -o integrate
// mpirun -np 3 integrate 5 10 50
// literature: https://en.m.wikipedia.org/wiki/Simpson%27s_rule
// 			   http://condor.cc.ku.edu/~grobe/docs/intro-MPI-C.shtml

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define send_data_tag 2001
#define return_data_tag 2002
#define root_process 0
   
float simpsons( float (*f)(float x), float a, float b, int n) {
        float h = (b - a) / n;
        float x;
        float r;
        char m = 0;
        float s = 0.0;

        for (x = a; x <= b; x+=h) {
                r = f(x);
                if (x == a || x == b) {
                        s += r;
                } else {
                        m = !m;
                        s += r * (m+1) * 2.0;
                }
        }
        return s * (h/3.0);
}


float function(float x){
	return x*x;
}


void masterExecution(int num_procs, int argc, char **argv){
	int an_id, ierr;
	float a,b,n, result, single_result, interval;
	float single_a, single_b, single_n;
	MPI_Status status;
	MPI_Request req;
	
	if(argc < 4){
		printf("Too few arguments\n");
		return;
	}
	
	a = atof(argv[1]);
	b = atof(argv[2]);
	n = atof(argv[3]);
	
	if(num_procs == 1){
		  result = simpsons(function, a, b, n);
		  printf("result: %f\n", result);
		  return;
	 }
	 
	 interval = (b - a) / (num_procs - 1);
	 single_n = n/(num_procs - 1);
	
	for(an_id = 1; an_id < num_procs; an_id++) { 
	
	 single_a = (an_id - 1) * interval + a;
	 single_b = single_a + interval;
	 
	 if(an_id == num_procs - 1){
		 single_b = b;
	 }

	ierr = MPI_Isend( &single_a, 1, MPI_FLOAT,
			an_id, send_data_tag, MPI_COMM_WORLD, &req);

	ierr = MPI_Isend( &single_b, 1, MPI_FLOAT,
			an_id, send_data_tag, MPI_COMM_WORLD, &req);
	
	ierr = MPI_Isend( &single_n, 1, MPI_FLOAT,
			an_id, send_data_tag, MPI_COMM_WORLD, &req);
	
   }
   
   result = 0;
   
   for(an_id = 1; an_id < num_procs; an_id++) { 
		ierr = MPI_Recv( &single_result, 1, MPI_FLOAT,
            MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		result = result + single_result;
   }
   
   printf("result: %f\n", result);
   
}


void slaveExecution(int my_id){
	int ierr;
	MPI_Status status;
	MPI_Request req;
	float a, b, n, result;
	
	ierr = MPI_Recv( &a, 1 , MPI_FLOAT,
		root_process, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		 
	ierr = MPI_Recv( &b, 1 , MPI_FLOAT,
		root_process, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	
	ierr = MPI_Recv( &n, 1 , MPI_FLOAT,
		root_process, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	
	result = simpsons(function, a, b, n);
    printf("%d: a: %f b: %f n: %f\n", my_id, a, b, n);
	printf("%d: result: %f\n", my_id, result);
	
	ierr = MPI_Isend( &result, 1, MPI_FLOAT,
		root_process, return_data_tag, MPI_COMM_WORLD, &req);
}



main(int argc, char **argv) 
{
    int my_id, ierr, num_procs;
    MPI_Status status;
	  
    ierr = MPI_Init(&argc, &argv);
	  
	  
	/* find out MY process ID, and how many processes were started. */
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
		  
	if(num_procs < 1){
		printf("Too few processes\n");
		return 0;
	}

	if( my_id == root_process ) {
		masterExecution(num_procs, argc, argv);
	}
	else{
		slaveExecution(my_id);
	}

    ierr = MPI_Finalize();
}