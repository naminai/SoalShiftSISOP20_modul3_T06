#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>
#define _GNU_SOURCE 
#define MAX 10000

char wrdir[MAX];
pthread_t tid[3]; 
char *path[MAX];

char *struprt(char *str) {
        char *next = str;
        while (*str != '\0') {
            *str = toupper((unsigned char)*str);
            return str; 
        } 
}

    
void* kategori(void *arg)
{
    char temp[MAX];
    char *temp2[MAX];
    char newname[MAX];
    int length1 = 0; 
    int length2 = 0;
    char *pathfile;
    char *filename;

    strcpy(temp,arg);
    filename = strtok(temp, "/");
    while(filename != NULL){
        temp2[length1] = filename;
        length1++;
        filename = strtok(NULL, "/");
    }
    strcpy(newname,temp2[length1-1]);
    
    pathfile = strtok(temp2[length1-1], ".");
    while(pathfile !=  NULL){
        path[length2] = pathfile;
        length2++;
        pathfile = strtok(NULL, ".");
    }
    char lcase[100];
    strcpy(lcase,path[length2-1]);
    struprt(lcase);
  
    DIR *d;
    struct dirent *dir;
    char path[MAX];
    d = opendir(wrdir);
    int flag = 0;
    if(length2 > 1){
        if(dir){
            while((dir = readdir(d)) != NULL){
                if(strcmp(dir->d_name,lcase) == 0 && dir->d_type == DT_DIR){
                    flag = 1;
                    break;
                }
            }
        }
        if(flag == 0){
            strcpy(path,wrdir);
            strcat(path,"/");
            strcat(path,lcase);
            mkdir(path, 0777);
        } else{
            // puts(error);
        }
    }else{
        strcpy(path,wrdir);
        strcat(path,"/");
        strcat(path,"Unknown");
        mkdir(path, 0777);
    }
	
    char move[MAX];
    char moved[MAX];
    strcpy(move,arg);
    strcpy(moved,wrdir);
    strcat(moved,"/");
    if(length2 == 1){
        strcat(moved,"Unknown");
    }
    else{
        strcat(moved,lcase);
    }
    strcat(moved,"/");
    strcat(moved,newname);
    rename(move,moved);
    length2 = 0;
    length1 = 0;
}

int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;
    char pathdir[100];
    if(getcwd(wrdir, sizeof(wrdir)) != NULL) {
        printf("Current directory: %s\n", wrdir);
    } 
    if (strcmp(argv[1],"-f") == 0) {
        int i=0;
        for(int j = 2 ; j < argc ; j++ ){
            pthread_create(&(tid[i]),NULL,kategori,argv[j]);
            pthread_join(tid[i],NULL);
            i++;
        }
    }
  
     
    else if(strcmp(argv[1],"*") == 0){
        d = opendir(wrdir);
        if(d){
            while((dir = readdir(d)) != NULL){
                if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0) )
                    continue;
                strcpy(pathdir,wrdir);
                strcat(pathdir,"/");
                strcat(pathdir,dir->d_name);
                int i  = 0;
                if(dir->d_type == DT_REG){
                    pthread_create(&(tid[i]),NULL,kategori,pathdir); 
                    pthread_join(tid[i],NULL);
                    i++;
                }
            }

        }
        else if(strcmp(argv[1], "-d") == 0) {
            d = opendir(argv[2]);
            if(d){
                while ((dir = readdir(d)) != NULL){
                        if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0) )
                            continue;
                        strcpy(pathdir,argv[2]);
                        printf("%s", pathdir);
                        strcat(pathdir,"/");
                        strcat(pathdir,dir->d_name);
                        printf("%s", pathdir);
                        int i = 0;
                        if(dir->d_type == DT_REG){
                            pthread_create(&(tid[i]),NULL,kategori,pathdir); 
                            pthread_join(tid[i],NULL);
                            i++;
                    }
                }
            }
        }

    }
    return 0; 
}
