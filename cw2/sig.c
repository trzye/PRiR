#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/* Funkcja uruchamiana, gdy napotkamy sygna³ HUP */
void on_hup(int signal) {
	printf("HUP..Czekam..\n");
	sleep(10); /* Czekamy najwy¿ej 10s na sygna³ */
	printf("..Skoñczy³em czekaæ..\n");
	pause(); /* Czekamy na kolejny sygna³ */
	printf("..Koniec HUP\n");
}

/* Funkcja uruchamiana, gdy napotkamy sygna³ USR1 */
/* Czy kiedykolwiek siê uruchomi? */
void on_usr1(int signal) {
	printf("Otrzyma³em USR1\n");
}

/* Funkcja uruchamiana, gdy napotkamy sygna³ USR2 */
void on_usr2(int signal) {
	printf("Otrzyma³em USR2\n");
}

/* Funkcja uruchamiana, gdy napotkamy sygna³ USR2 */
//void on_sigtstp(int signal) {
//	printf("Otrzyma³em SIGTSTP\n");
//}

int main (int argc, char** argv) {
	/* Pobierz swój PID */
	pid_t my_pid = getpid();
	sigset_t mask; /* Maska sygna³ów */

	/* Konfiguracja obs³ugi sygna³u USR1 */
	struct sigaction usr1;
	sigemptyset(&mask); /* Wyczyœæ maskê */
	usr1.sa_handler = (&on_usr1);
	usr1.sa_mask = mask;
	usr1.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &usr1, NULL);

	/* Konfiguracja obs³ugi sygna³u USR2 */
	struct sigaction usr2;
	sigemptyset(&mask); /* Wyczyœæ maskê */
	usr2.sa_handler = (&on_usr2);
	usr2.sa_mask = mask;
	usr2.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR2, &usr2, NULL);
	
	/*Ignorowanie ctrl-z */
	signal(SIGTSTP, SIG_IGN);
	
	/* Konfiguracja obs³ugi sygna³u SIGTSTP */
//	struct sigaction sigtstp;
//	sigemptyset(&mask); /* Wyczyœæ maskê */
//	sigtstp.sa_handler = (&on_sigtstp);
//	sigtstp.sa_mask = mask;
//	sigtstp.sa_flags = SA_SIGINFO;
//	sigaction(SIGTSTP, &sigtstp, NULL);

	/* Konfiguracja obs³ugi sygna³u
	 * HUP */
	struct sigaction hup;
	sigemptyset(&mask); /* Wyczyœæ maskê */
	sigaddset(&mask, SIGUSR1); /* Dodaj do niej SIGUSR1 */
	hup.sa_handler = (&on_hup);
	hup.sa_mask = mask;
	hup.sa_flags = SA_SIGINFO;
	sigaction(SIGHUP, &hup, NULL);

	sigemptyset(&mask); /* Wyczyœæ maskê */
	sigprocmask(SIG_BLOCK, &mask, NULL); /* Ustaw maskê dla ca³ego procesu */

	printf("PID: %d\n", my_pid);
	while(1)
		pause(); /* Czekaj na sygna³y */

	return 0;
}