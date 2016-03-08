#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

/* Funkcja uruchamiana, gdy napotkamy sygna� HUP */
void on_hup(int signal) {
	printf("HUP..Czekam..\n");
	sleep(10); /* Czekamy najwy�ej 10s na sygna� */
	printf("..Sko�czy�em czeka�..\n");
	pause(); /* Czekamy na kolejny sygna� */
	printf("..Koniec HUP\n");
}

/* Funkcja uruchamiana, gdy napotkamy sygna� USR1 */
/* Czy kiedykolwiek si� uruchomi? */
void on_usr1(int signal) {
	printf("Otrzyma�em USR1\n");
}

/* Funkcja uruchamiana, gdy napotkamy sygna� USR2 */
void on_usr2(int signal) {
	printf("Otrzyma�em USR2\n");
}

/* Funkcja uruchamiana, gdy napotkamy sygna� USR2 */
//void on_sigtstp(int signal) {
//	printf("Otrzyma�em SIGTSTP\n");
//}

int main (int argc, char** argv) {
	/* Pobierz sw�j PID */
	pid_t my_pid = getpid();
	sigset_t mask; /* Maska sygna��w */

	/* Konfiguracja obs�ugi sygna�u USR1 */
	struct sigaction usr1;
	sigemptyset(&mask); /* Wyczy�� mask� */
	usr1.sa_handler = (&on_usr1);
	usr1.sa_mask = mask;
	usr1.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &usr1, NULL);

	/* Konfiguracja obs�ugi sygna�u USR2 */
	struct sigaction usr2;
	sigemptyset(&mask); /* Wyczy�� mask� */
	usr2.sa_handler = (&on_usr2);
	usr2.sa_mask = mask;
	usr2.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR2, &usr2, NULL);
	
	/*Ignorowanie ctrl-z */
	signal(SIGTSTP, SIG_IGN);
	
	/* Konfiguracja obs�ugi sygna�u SIGTSTP */
//	struct sigaction sigtstp;
//	sigemptyset(&mask); /* Wyczy�� mask� */
//	sigtstp.sa_handler = (&on_sigtstp);
//	sigtstp.sa_mask = mask;
//	sigtstp.sa_flags = SA_SIGINFO;
//	sigaction(SIGTSTP, &sigtstp, NULL);

	/* Konfiguracja obs�ugi sygna�u
	 * HUP */
	struct sigaction hup;
	sigemptyset(&mask); /* Wyczy�� mask� */
	sigaddset(&mask, SIGUSR1); /* Dodaj do niej SIGUSR1 */
	hup.sa_handler = (&on_hup);
	hup.sa_mask = mask;
	hup.sa_flags = SA_SIGINFO;
	sigaction(SIGHUP, &hup, NULL);

	sigemptyset(&mask); /* Wyczy�� mask� */
	sigprocmask(SIG_BLOCK, &mask, NULL); /* Ustaw mask� dla ca�ego procesu */

	printf("PID: %d\n", my_pid);
	while(1)
		pause(); /* Czekaj na sygna�y */

	return 0;
}