#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/wait.h>
//#include <signal.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>

int myglobal;
pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;

double **A;
double **B;
double **C;
double sum;
double result;
int ma, mb, na, nb;


struct v {
    int i; /* rzad */
    int j; /* kolumna */
};

void *runner(void *param); /* watek*/


print_matrix(double**A, int m, int n)
{
    int i, j;
    printf("[");
    for(i =0; i< m; i++)
    {
        for(j=0; j<n; j++)
        {
            printf("%f ", A[i][j]);
        }
        printf("\n");
    }
    printf("]\n");
}

void readfiles(){
    FILE *fpa;
    FILE *fpb;
    int i, j;
    double x;

    fpa = fopen("A.txt", "r");
    fpb = fopen("B.txt", "r");
    if( fpa == NULL || fpb == NULL )
    {
        perror("blad otwarcia pliku");
        exit(-10);
    }

    fscanf (fpa, "%d", &ma);
    fscanf (fpa, "%d", &na);
    fscanf (fpb, "%d", &mb);
    fscanf (fpb, "%d", &nb);

    if(na != mb)
    {
        printf("Zle wymiary macierzy!\n");
        exit(-10);
    }

//a
    A = malloc(ma*sizeof(double));
    for(i=0; i< ma; i++)
    {
        A[i] = malloc(na*sizeof(double));
    }

    for(i =0; i< ma; i++)
    {
        for(j = 0; j<na; j++)
        {
            fscanf( fpa, "%lf", &x);
            A[i][j] = x;
        }

    }
//b
    B = malloc(mb*sizeof(double));
    for(i=0; i< mb; i++)
    {
        B[i] = malloc(nb*sizeof(double));
    }

    for(i =0; i< mb; i++)
    {
        for(j = 0; j<nb; j++)
        {
            fscanf( fpb, "%lf", &x);
            B[i][j] = x;
        }
    }

    fclose(fpa);
    fclose(fpb);

}

double frobeniusNorm(double* matrix, int size1, int size2)
{
    result = 0.0;
    int i,j;
    for(i = 0; i < size1; ++i)
    {
        for(j = 0; j < size2; ++j)
        {
            double value = *(matrix + (i*size2) + j);
            result += value * value;
        }
    }
    return sqrt(result);
}

int main(int argc, char *argv[]) {

    int i,j, count = 0;

    readfiles(); //odczytanie plik�w

    //zaalokowanie pami�ci na macierz wynikowa
    C = malloc(ma*sizeof(double));
    for(i=0; i< ma; i++)
    {
        C[i] = malloc(nb*sizeof(double));
    }

    double fn = frobeniusNorm(*A, ma, na);
    printf("fn dla A: %f\n",fn);

    for(i = 0; i < ma; i++)
    {
        for(j = 0; j < nb; j++)
        {
            //rzad kolumna dla kazdego watku
            struct v *data = (struct v *) malloc(sizeof(struct v));
            data->j = j;
            data->i = i;
            pthread_t tid;
            pthread_create(&tid,NULL,runner,data);
            pthread_join(tid, NULL);
        }
    }


    printf("A:\n");
    print_matrix(A, ma, mb);
    printf("B:\n");
    print_matrix(B, mb, nb);
    printf("C:\n");
    print_matrix(C, ma, nb);

}

void *runner(void *param) {

    struct v *data = param;
    pthread_mutex_lock(&mymutex);

    int n;
    sum = 0.0;

    //rzedy przez kolumny
    for(n = 0; n< mb; n++)
    {
        sum += A[data->i][n] * B[n][data->j];
    }
    sleep(1);
    //wynik do macierzy
    C[data->i][data->j] = sum;

    pthread_mutex_unlock(&mymutex);

    return NULL;
}
