#include <stdio.h>
#include <stdlib.h>
#define BUFFOR_SIZE 80

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
void print_vector(double *vector, int vector_length){
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
double* read_vector(int *vector_length){
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



int main(int argc, char **argv) { 

	
	int vector_length ;
	double* vector = read_vector(&vector_length);
	double vector_sum = sum(vector, vector_length);
	
	print_vector(vector, vector_length);
	printf("Suma elementow w wektorze = %f\n", vector_sum );
	
	return 0;
}