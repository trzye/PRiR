#include <stdio.h>
#include <unistd.h> //funkcje unixowe, fork
#include <sys/wait.h> 

int printPID(int offsprings){
	
	// wypisuje identyfikator procesu 
	printf("%d:%d:E execution\n", offsprings, getpid());
	
	if(offsprings != 0){
		
		// tworzy nowy proces
		switch (fork()) {
			// błąd
			case -1: 
				fprintf(stderr, "Error in fork\n");
				return -1;
			// proces potomny 
			case 0: 
				printf("%d:%d:C creating\n", offsprings, getpid());
				//wywołaj funkcję jeszcze raz i zmniejsz licznik
				return printPID(--offsprings);
			// proces macierzysty 
			default: 
				printf("%d:%d:P waiting\n", offsprings, getpid());
				// czeka na zakończenie procesu potomnego 
				if(wait(0) == -1){
					fprintf(stderr, "Error in wait\n");
					return -1;
				}
				printf("%d:%d:P closing\n", offsprings, getpid());
				return 0;
		}
	}
	
	return 0;
}

int main ()
{
	return printPID(4);
}