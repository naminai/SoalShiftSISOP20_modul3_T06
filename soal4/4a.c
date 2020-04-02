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

int matA[4][2]={
		{1, 2},
		{2, 3},
		{2, 1},
		{3, 1}
};

int matB[2][5]={
		{3, 1, 2, 3, 1},
		{2, 3, 1, 1, 1}
};

int matC[4][5];
int row = 0;

void* kali(void* arg){
	int i = row++;
	for (int j=0; j<5; j++){
		for(int k=0; k<2; k++){
			matC[i][j] += matA[i][k] * matB[k][j];		
		} 	
	}
}

int main(){
	
	pthread_t tid[4];
	for (int i=0; i<4; i++){
		pthread_create(&(tid[i]), NULL, &kali, NULL);
	}
		
	for (int i=0; i<4; i++){
		pthread_join((tid[i]), NULL);
	}
	
	printf("C = \n");
	for (int i=0; i<4; i++){
		for(int j=0; j<5; j++){
			printf("%4d", matC[i][j]);		
		}
		printf("\n");
	}

	key_t key;
   	int ID;
   	struct memory *ptr;

   	key = ftok("key",256);
   	ID = shmget(key,sizeof(struct memory),IPC_CREAT|0666);
   	ptr = (struct memory *) shmat(ID, NULL, 0); 
	ptr->status = UNREADY;
	int j = 0;
	int k = 0;

	for(int i = 0; i < 20; i++){
		if(k > 5){
	   		j++;
	   		k = 0;
		}
		ptr->data[i] = matC[j][k];
		k++;
	}
	ptr->status = READY;

	while (ptr->status != TAKEN)
	    sleep(1);

	shmdt((void *) ptr);
	shmctl(ID, IPC_RMID, NULL);
	exit(0);

	return 0;
}
