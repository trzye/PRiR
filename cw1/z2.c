#include <stdio.h>
#include <unistd.h> //funkcje unixowe, fork
#include <sys/wait.h> 
#include <stdlib.h>

float maximum(int start, int end, float* num){
	float max = num[start];
	for(int j=start+1; j<end; j++){
		if(num[j] > max)
			max = num[j];
	}
	return max;
}

int main(int count, char** argv)
{
	
	if(count > 2){
		
		//wektor liczb 
		float num[count-2];
		
		//na ile chcemy podzielić wektor
		int split=atoi(argv[1]);
		
		//wczytanie liczb do sortowania
		for( int j=1; j<count - 1; j++){
			num[j-1]=atof(argv[j+1]);
		}
		
		//wypisanie wczytanych danych
		printf("Podział: %d\n", split);
		printf("Wektor: [ %f", num[0]);
		for( int j=1; j<count-2; j++){
			printf(", %f ", num[j]);
		}
		printf("]\n");
		
		//na jakiej długości odcinki dzielimy wektor
		int interval = ((count-2)/split);
		
		for(int j=0; j<split; j++){
			
			//ustawiam interwał
			int start = j * interval;
			int end = (j + 1) * interval;
			
			//jak jest nierówny podział to nie tracimy końcówki!
			if(j == split - 1){
				end = count - 2;
			}
			
			//nowy proces
			switch (fork()) {
				case -1: 
					fprintf(stderr, "Error in fork\n");
					return -1;
				case 0:
					//wykonaj CIEŻKIE obliczenia
					printf("%d: %d-%d Max: %f\n",getpid(), start, end, maximum(start, end , num));
					break;
				default: 
					if(wait(0) == -1){
						fprintf(stderr, "Error in wait\n");
						return -1;
					}
					return 0;
			}
		}

	}
	
	return 0;
}