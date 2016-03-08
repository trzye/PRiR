#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#define BUFFOR_SIZE 80
#define CHILDREN_NUMBER 5

/*
	Sums numbers in vector.
*/
double sum(double* vector, int vector_length) {
	int i;
	double sum = 0.0f;
	for(i=0; i < vector_length; i++) {
		sum += vector[i];
	}
	return sum;
}

/*
	Print vector informations.
*/
void print_vector(double *vector, int vector_length) {
	int i;
	
	printf("Vector has %d elements\n", vector_length);
	printf("v = [ ");
	for(i=0; i<vector_length; i++) {
		printf("%f ", vector[i]);
	}
	printf("]\n");
}

/*
	Return readed vector. Vector lenght as pointer.
*/
double* read_vector(int *vector_length) {
	FILE* f = fopen("vector.dat", "r");
	
    char buffor[BUFFOR_SIZE+1];
	int i;
	
	double* vector;
	
	fgets(buffor, BUFFOR_SIZE, f);
 	*vector_length = atoi(buffor);
	vector = malloc(sizeof(double) * *vector_length);
	for(i=0; i<*vector_length; i++) {
		fgets(buffor, BUFFOR_SIZE, f);
		vector[i] = atof(buffor);
	}
	fclose(f);
	return vector;
}

/*
	Operation after receiving USR1 signal.
*/
void on_usr1(int signal) {
	printf("%d: Otrzymalem USR1\n", getpid());
	exit(0);
}

/*
	Setting management of USR1 signal
*/
void set_usr1_signal() {
	sigset_t mask;
	
	struct sigaction usr1;
	sigemptyset(&mask); 
	usr1.sa_handler = (&on_usr1);
	usr1.sa_mask = mask;
	usr1.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &usr1, NULL);
}

/*
	Creates children and sets operation for USR1 signal.
*/
void create_children() {
	int child_id;
	
	for(child_id=0; child_id<CHILDREN_NUMBER; child_id++){
		pid_t pid = fork();
		if(pid < 0) {
			printf("Error");
			exit(1);
		} else if (pid == 0) {
			printf("Child\t (%d): %d\n", child_id + 1, getpid());
			set_usr1_signal();
			while(1) 
				pause();
		} else  {
			 printf("Parent\t (%d): %d\n", child_id + 1, getpid());
		}
	}
}

/*
	Waits for all children to be closed.
*/
void wait_for_children() {
	int j=0;
	
	for(j=0; j<CHILDREN_NUMBER; j++){
					 wait(NULL);
	}
}


int main(int argc, char **argv) { 

	int vector_length;
	double* vector; 
	double vector_sum;
	
	create_children();
	
	vector = read_vector(&vector_length);
	vector_sum = sum(vector, vector_length);
		
	print_vector(vector, vector_length);
	
	wait_for_children() ;
	
	printf("Suma elementow w wektorze = %f\n", vector_sum );

	
	return 0;
}