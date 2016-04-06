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


float masterExecution(int num_procs, int argc, char **argv, int my_id){
	int an_id, ierr;
	float a,b,n, result, single_result, interval;
	float single_a, single_b, single_n;
	float data_array[num_procs][3];
	float receive_array[3];
	float result_array[num_procs];
	
	MPI_Status status;
	
	if(argc < 4){
		printf("Too few arguments\n");
		return;
	}
	
	a = atof(argv[1]);
	b = atof(argv[2]);
	n = atof(argv[3]);
	
	if(num_procs == 1){
		  result = simpsons(function, a, b, n);
		  return result;
	}
	 
	 interval = (b - a) / (num_procs );
	 single_n = n/(num_procs);
	
	for(an_id = 0; an_id < num_procs; an_id++) { 
	
	 single_a = (an_id) * interval + a;
	 single_b = single_a + interval;
	 
	 if(an_id == num_procs){
		 single_b = b;
	 }
	 
	 data_array[an_id][0]=single_a;
	 data_array[an_id][1]=single_b;
	 data_array[an_id][2]=single_n;
	 
/*
	ierr = MPI_Send( &single_a, 1, MPI_FLOAT,
			an_id, send_data_tag, MPI_COMM_WORLD);

	ierr = MPI_Send( &single_b, 1, MPI_FLOAT,
			an_id, send_data_tag, MPI_COMM_WORLD);
	
	ierr = MPI_Send( &single_n, 1, MPI_FLOAT,
			an_id, send_data_tag, MPI_COMM_WORLD);
*/
    }
	
	MPI_Scatter(data_array, 3, MPI_FLOAT, &receive_array, 3, MPI_FLOAT, root_process, MPI_COMM_WORLD);

	result = simpsons(function, receive_array[0], receive_array[1], receive_array[2]);
	printf("%d: a: %f b: %f n: %f\n", my_id, receive_array[0], receive_array[1], receive_array[2]);
	printf("%d: result: %f\n", my_id, result);
	
	MPI_Gather(&result, 1, MPI_FLOAT, result_array, 1, MPI_FLOAT, root_process, MPI_COMM_WORLD);

	
	result = 0;
   
   for(an_id = 0; an_id < num_procs; an_id++) { 
		result = result + result_array[an_id];
   }
   
   return result;
}



main(int argc, char **argv) 
{
    int my_id, ierr, num_procs;
	float result;
    MPI_Status status;
	  
    ierr = MPI_Init(&argc, &argv);
	  
	  
	/* find out MY process ID, and how many processes were started. */
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
	ierr = MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
		  
	if(num_procs < 1){
		printf("Too few processes\n");
		return 0;
	}

	result = masterExecution(num_procs, argc, argv, my_id);
	if(my_id == root_process){
		printf("result: %f\n", result);
	}
	
    ierr = MPI_Finalize();
}