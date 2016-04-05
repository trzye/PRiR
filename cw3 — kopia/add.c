#include <stdio.h>
#include <stdlib.h>
#define BUFFOR_SIZE 80

double sum(double* vector, int n) {
	int i;
	double sum = 0.0f;
	for(i=0; i<n; i++) {
		sum += vector[i];
	}
	return sum;
}

int main(int argc, char **argv) { 
	FILE* f = fopen("vector.dat", "r");
    char buffor[BUFFOR_SIZE+1];
	double* vector;
	int n;
	int i;
	fgets(buffor, BUFFOR_SIZE, f);
 	n = atoi(buffor);
	vector = malloc(sizeof(double) * n);
	printf("Vector has %d elements\n", n);
	for(i=0; i<n; i++) {
		fgets(buffor, BUFFOR_SIZE, f);
		vector[i] = atof(buffor);
	}
	fclose(f);
	printf("v = [ ");
	for(i=0; i<n; i++) {
		printf("%f ", vector[i]);
	}
	printf("]\n");

	printf("Suma elementów w wektorze = %f\n", sum(vector, n));
	return 0;
}