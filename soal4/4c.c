#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define MAX 256
#define UNREADY -1
#define READY 0
#define TAKEN 1

struct memory{
    int status;
    int data[256];
};

int baris = 0;
int row = 0;
int matC[4][5];
typedef long long ll;

void* factorial(void* arg){
	int i = *((int*)arg);
	free(arg);;
	ll total = 0;
	for(int j = i; j > 0 ;j--){
		total += j;
	}
	if(row > 4){
		printf("\n");
		row = 0;
	}
	printf("%15llu", total);
	row++;
}

int main(){
	key_t key;
	int ID;
	struct memory *ptr;
	key = ftok("key",100);
	ID = shmget(key, sizeof(struct memory), 0666);
	if(ID < 0){
		printf(" ** SHMERROR CLIENT! ** \n");
		exit(1);
	}
	
	ptr = (struct memory*) shmat(ID, NULL, 0); 
	while (ptr->status != READY) 
	;

	printf("Matriks C: \n");
	while(baris < 4){
		memcpy(matC, &ptr->data, 256 * sizeof(int));
		for(int k=0; k<5; k++){
      			printf("%4d", matC[baris][k]);
		}
		baris++;
		printf("\n");
	}

	printf("Matriks C Faktorial: \n");
	pthread_t tid[20];
	for(int i = 0; i < 20;i++){
		int *x =  malloc(sizeof(*x));
		*x = ptr->data[i];
		pthread_create(&(tid[i]), NULL, &factorial, x);
		pthread_join(tid[i], NULL);

	}
	ptr->status = TAKEN; 
	shmdt((void *) ptr);
	printf("\n");
	return 0;
}
