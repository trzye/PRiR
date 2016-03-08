/*
Należy napisać własny program, który utworzy proces potomny z użyciem funkcji fork(). 
Proces nadrzędny powinien wysłać wybrany sygnał do swojego potomka korzystając z funkcji języka C - kill(...) 
- Proszę wykorzystać funkcję printf do weryfikacji obsługi sygnałów (proszę spróbować zaobserwować synchroniczny l
ub asynchroniczny charakter obsługi sygnałów).
*/

#include <stdio.h>
#include <unistd.h> //funkcje unixowe, fork
#include <sys/wait.h> 
#include <signal.h> //sygnaly

/* Funkcja uruchamiana, gdy napotkamy sygnał USR1 */
/* Czy kiedykolwiek się uruchomi? */
void on_usr1(int signal) {
	printf("Otrzymałem USR1\n");
}

int printPID(int offsprings){
	
	// wypisuje identyfikator procesu 
	printf("%d:%d:E execution\n", offsprings, getpid());
	
	pid_t pid;
	
	if(offsprings != 0){
		
		// tworzy nowy proces
		switch (pid = fork()) {
			// błąd
			case -1: 
				fprintf(stderr, "Error in fork\n");
				return -1;
			// proces potomny 
			case 0: 
				printf("%d:%d:C creating\n", offsprings, getpid());
				
				sigset_t mask; /* Maska sygnałów */

				/* Konfiguracja obsługi sygnału USR1 */
				struct sigaction usr1;
				sigemptyset(&mask); /* Wyczyść maskę */
				usr1.sa_handler = (&on_usr1);
				usr1.sa_mask = mask;
				usr1.sa_flags = SA_SIGINFO;
				sigaction(SIGUSR1, &usr1, NULL);
	
				return 0;
			// proces macierzysty 
			default: 
				printf("%d:%d:P waiting\n", offsprings, getpid());
				// czeka na zakończenie procesu potomnego 
				
				if(wait(0) == -1){
					fprintf(stderr, "Error in wait\n");
					return -1;
				}
				
				kill(pid, SIGUSR1);
				
				printf("%d:%d:P closing\n", offsprings, getpid());
				return 0;
		}
	}
	
	return 0;
}

int main ()
{
	printPID(1);
	
	while(1)
			pause(); /* Czekaj na sygnały */
}
