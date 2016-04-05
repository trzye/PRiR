#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define BUFFOR_SIZE 80
#define CHILDREN_NUMBER 16
#define MEMORY_KEY 123

int   shmid;
char* shmaddr;
double* vector; 
int child_id;
/*
void get_system_time(){
    time_t result = time(NULL);
    return gmtime(&result);
}
*/
void save_data_to_shared_memory(int key, char* data){
	if( (shmid = shmget(key, BUFFOR_SIZE, 0666 | IPC_CREAT)) < 0)
		printf("shmget error");
		
	if( (shmaddr = shmat(shmid, 0, 0)) < 0)
		printf("shmat error");
		
	strcpy (shmaddr, data);
}

char* read_data_from_shared_memory(int key){
	if( (shmid = shmget(key, BUFFOR_SIZE, 0666 | IPC_CREAT)) < 0)
		printf("shmget error");
		
	if( (shmaddr = shmat(shmid, 0, 0)) < 0)
		printf("shmat error");
		
	return shmaddr;
}

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
		save_data_to_shared_memory(10000 + i, buffor);
	}
	fclose(f);
	return vector;
}

/*
	Writes vector indexes to shared memory
*/
void write_vector_indexes2(int vector_length) {
	char buffor[BUFFOR_SIZE+1];
	int interval = vector_length / CHILDREN_NUMBER + 1;
	int j;
	for(j=0; j<CHILDREN_NUMBER*2; j=j+2){
		sprintf(buffor, "%d", interval * j);
		save_data_to_shared_memory(j, buffor );
		if(j == (CHILDREN_NUMBER*2 - 6)){
			sprintf(buffor, "%d", vector_length);
			save_data_to_shared_memory(j + 1, buffor );
		} else {
			sprintf(buffor, "%d", interval * (j + 1));
			save_data_to_shared_memory(j + 1, buffor );
		}
	}
}

/*
	Writes vector indexes to shared memory
*/
void write_vector_indexes(int vector_length) {
	char buffor[BUFFOR_SIZE+1];
	int interval = vector_length / CHILDREN_NUMBER;
	int index1;
	int index2;
	
	 for (j = 0; j < CHILDREN_NUMBER - 1; j++) {
            index1 = interval * j;
            index2 = interval * (j + 1) - 1;
            sprintf(buffor, "%d", index1);
			save_data_to_shared_memory(j * 2, buffor );
			sprintf(buffor, "%d", index2);
			save_data_to_shared_memory(j * 2 + 1, buffor );
     }
	 if (CHILDREN_NUMBER > 1) {
			buffor = read_data_from_shared_memory((CHILDREN_NUMBER * 2) - 3);
			save_data_to_shared_memory((CHILDREN_NUMBER * 2) - 2, buffor );
			sprintf(buffor, "%d", vector_length - 1);
			save_data_to_shared_memory((CHILDREN_NUMBER * 2) - 1, buffor );
            this.indexesMap.put(threads - 1, new Pair<>(this.indexesMap.get(threads - 2).getValue() + 1, indexes - 1));
     } else { //TODO
			sprintf(buffor, "%d", 0);
			save_data_to_shared_memory(0, buffor );
			sprintf(buffor, "%d", vector_length - 1);
			save_data_to_shared_memory(1, buffor );
            this.indexesMap.put(threads - 1, new Pair<>(this.indexesMap.get(threads - 2).getValue() + 1, indexes - 1));
    }
}

/*
	Operation after receiving USR1 signal.
*/
void on_usr1(int signal) {

	int index1 = atoi(read_data_from_shared_memory(child_id ));
	int index2 = atoi(read_data_from_shared_memory(child_id + 1));	
	char buffor[BUFFOR_SIZE+1];

	printf("%d: Otrzymalem USR1\t\t", getpid());	
	printf("%d: Moje indexy: [%d:%d]]\t\t",getpid(), index1, index2);	
	
	double sum = 0.0f;
	for(; index1< index2; index1++) {
		double num = atof(read_data_from_shared_memory(10000 + index1));
		sum += num;
	}
	
	sprintf(buffor, "%f", sum);
	save_data_to_shared_memory(-child_id - 1, buffor);

	printf("%d: Moj wynik sumowania: %f\n",getpid(), sum);	
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
void create_children(int parent_pid) {
	for(child_id=0; child_id<CHILDREN_NUMBER; child_id++){
		pid_t pid = fork();
		if(pid < 0) {
			printf("Error");
			exit(1);
		} else if (pid == 0) {
			printf("Child\t (%d)\t: %d\n", child_id + 1, getpid());
			set_usr1_signal();
			return;
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

    clock_t start = clock();

	pid_t parent_pid = getpid() ;

	int vector_length;
	double vector_sum;
	int j;
	
	vector = malloc(sizeof(double) * 10000);
	
	create_children(parent_pid);
	
	//process parent
	if(getpid() == parent_pid) {
		
		vector = read_vector(&vector_length);
		write_vector_indexes(vector_length) ;
		
		for(j=1; j<=CHILDREN_NUMBER; j++){
			kill(parent_pid+j, SIGUSR1);
		}
		
		wait_for_children();
		int n;
		float vector_sum_p = 0.0f;
		for(n=-CHILDREN_NUMBER; n < 0; n++){
			float res = atof(read_data_from_shared_memory(n));
			vector_sum_p += res;
		}
		printf("Suma elementow w wektorze (z procesami)= %f\n", vector_sum_p);
		
		clock_t end = clock();
		double time_elapsed_in_seconds = (end - start)/(double)CLOCKS_PER_SEC;
		
		
		vector_sum = sum(vector, vector_length);
		printf("Suma elementow w wektorze (bez procesow)= %f\n", vector_sum );
		printf("Czas wykonania: %f [s] dla %d procesow potomnych\n", time_elapsed_in_seconds, CHILDREN_NUMBER);
		
		return 0;
	} 
	
	//wait for children USR1 signals
	while(1)
		pause();
	
	return 0;
}
