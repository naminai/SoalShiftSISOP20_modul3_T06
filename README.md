# SoalShiftSISOP20_modul3_T06 
Penyelesaian Soal Shift Modul 3 Sistem Operasi 2020 
Kelompok T06 
  * Donny Kurnia Ramadhani (05311840000004) 
  * Made Krisnanda Utama (0531184000033) 

--- 
## Daftar Isi 
* [Soal 3](#soal-3)  
* [Soal 4](#soal-4) 
  * [Soal 4.a.](#soal-4a) 
  * [Soal 4.b.](#soal-4b) 
  * [Soal 4.c.](#soal-4c)
* [Kendala Praktikkan](#kendala)
--- 

## Soal 3
Source Code : [source](https://github.com/naminai/SoalShiftSISOP20_modul2_T06/tree/master/soal3)
```
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
```
Disini terdapat kita meng-include header file yang akan digunakan pada penyelesaian soal 3 ini. Kemudian juga kita `define MAX` untuk isi dari array-array yang akan kita gunakan dibawah. Lalu, kita juga `define _GNU_SOURCE` untuk mendapatkan beberapa fungsi Linux nonstandard (`DT_REG` dan `DT_DIR`). 
```
char wrdir[MAX];
pthread_t tid[3]; 
char *path[MAX];
```
Deklarasi variabel `wrdir[MAX]` untuk menampung `cwd` dan `*path[MAX]` untuk mendapatkan full path direktori. Kemudian kita deklarasikan pula `tid[3]` sesuai jumlah argumen.
```
char *struprt(char *str) {
        char *next = str;
        while (*str != '\0') {
            *str = toupper((unsigned char)*str);
            return str; 
        } 
}
```
Disini kita deklarasikan fungsi untuk membuat karakter lowercase menjadi uppercase sehingga tidak case-sensitive.

```
void* kategori(void *arg)
{
    char temp[MAX];
    char *temp2[MAX];
    char newname[MAX];
    int length1 = 0; 
    int length2 = 0;
    char *pathfile;
    char *filename;
```
Deklarasi variabel yang akan digunakan. `temp` dan `temp2` digunakan untuk menampung nama file dalam absolute path beserta ekstensi. `newname` digunakan untuk menampung hasil dari `temp` yang sudah menjadi nama file dan ekstensi. `length` dan `length2` untuk mendapatkan panjang dari string full path dan nama file. 
```
    strcpy(temp,arg);
    filename = strtok(temp, "/");
    while(filename != NULL){
        temp2[length1] = filename;
        length1++;
        filename = strtok(NULL, "/");
    }
    strcpy(newname,temp2[length1-1]);
```
Menghasilkan file denga nama serta ekstensi.
```
    pathfile = strtok(temp2[length1-1], ".");
    while(pathfile !=  NULL){
        path[length2] = pathfile;
        length2++;
        pathfile = strtok(NULL, ".");
    }
```
Menghasilkan ekstensi dari file.
```
    char lcase[100];
    strcpy(lcase,path[length2-1]);
    struprt(lcase);
```
Membuat ekstensi file tidak case-sensitive menggunakan fungsi struprt.
```
    DIR *d;
    struct dirent *dir;
    char path[MAX];
    d = opendir(wrdir);
```
Membuka current working directory untuk di-stream dengan menggunakan `opendir`.
```
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
```
Melakukan cek apakah sudah ada direktori bernama ekstensi file pada direktori ini.	
```
        if(flag == 0){
            strcpy(path,wrdir);
            strcat(path,"/");
            strcat(path,lcase);
            mkdir(path, 0777);
        } else{
            // puts(error);
        }
```
Jika belum ada, maka kita buat direktori ekstensi file.
```
    }else{
        strcpy(path,wrdir);
        strcat(path,"/");
        strcat(path,"Unknown");
        mkdir(path, 0777);
    }
```
Untuk file yang tidak memiliki ekstensi, kita buat direktori `Unkown`
```
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
```
Membuat fullpath dari file tanpa nama file.
```
    strcat(moved,"/");
    strcat(moved,newname);
    rename(move,moved);
    length2 = 0;
    length1 = 0;
}
```
Menambahkan nama file ke fullpath. Lalu reset `length2` dan `length1` untuk digunakan pada file berikutnya.
```
int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;
    char pathdir[100];
    if(getcwd(wrdir, sizeof(wrdir)) != NULL) {
        printf("Current directory: %s\n", wrdir);
    } 
```
Membuka direktori yang kita gunakan sekarang dan print nama dari current working directory.  
```
    if (strcmp(argv[1],"-f") == 0) {
        int i=0;
        for(int j = 2 ; j < argc ; j++ ){
            pthread_create(&(tid[i]),NULL,kategori,argv[j]);
            pthread_join(tid[i],NULL);
            i++;
        }
    }
```
Untuk case pertama `-f` dimana 2 file dicari, maka kita hanya membuat 2 thread untuk memindahkan kedua file tersebut.
```
     
    else if(strcmp(argv[1],"*") == 0){
        d = opendir(wrdir);
        if(d){
            while((dir = readdir(d)) != NULL){
                if ((strcmp(dir->d_name, ".") == 0) || (strcmp(dir->d_name, "..") == 0) )
                    continue;
```
Untuk case kedua `*` kita buka current working directory, lalu kita skip `.` dan `..`.
```
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
```
Jika didalamnya terdapat file  (`if (dir->d_type == DT_REG)`) maka  kita buat thread sesuai dengan banyaknya file disitu.
```
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
```
Untuk `-d` yaitu direktori lain, maka sama saja dengan diatas, hanya bedanya kita menggunakan path direktori sebagai input, bukan `getcwd`. Lalu untuk setiap file dalam direktori kita create thread dan join.

## Soal 4
Source Code : [source](https://github.com/naminai/SoalShiftSISOP20_modul2_T06/tree/master/soal4)
### Soal 4.a.
```
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define UNREADY -1
#define READY 0
#define TAKEN 1
```
Diatas merupakan beberapa header file yang akan digunakan dalam pengerjaan soal 4. Terdapat juga beberapa definisi: 
`UNREADY -1` adalah status dimana shared memory belum connect;
`READY 0` adalah status dimana shared memory siap menjadi memory attachment dalam file lain;
`TAKEN 1` adalah status dimana memory sharing sudah selesai. 

```
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
```
Diatas terdapat beberapa deklarasi. Yang pertama adalah struktur data memory yang memiliki element `int status` yang akan menjadi buffer menampung status `READY`, `TAKEN`, dan `UNREADY`  dan array `int data[256]` yang digunakan untuk menampung data matriks 2 dimensi hasil perkalian matriks A dan B yaitu matriks C yang telah dikonversi menjadi bentuk 1 dimensi. Kemudian terdapat matriks A dan B yang didefinisikan di dalam kodingan. Lalu dideklarasikan hasil perkalian keduanya yaitu matriks C. Terakhir, kita definisikan `int row = 0` untuk digunakan dalam fungsi `kali` dibawah.

```
void* kali(void* arg){
	int i = row++;
	for (int j=0; j<5; j++){
		for(int k=0; k<2; k++){
			matC[i][j] += matA[i][k] * matB[k][j];		
		} 	
	}
}
```
Fungsi kali mengalikan kedua matriks, bekerja dengan cara mengalikan setiap baris matriks A dan B `matA[4][2] * matB[2][5]` dan kemudian hasil dari kedua matriks itu menjadi matriks C `matC[4][5]`.
```
int main(){
	
	pthread_t tid[4];
	for (int i=0; i<4; i++){
		pthread_create(&(tid[i]), NULL, &kali, NULL);
	}
		
	for (int i=0; i<4; i++){
		pthread_join((tid[i]), NULL);
	}
 ```
Membuat thread dengan argumen fungsi kali untuk mengalikan matriks A dan B kemudian kita join keempat thread tersebut. Kita lakukan thread 4 kali dikarenakan kita mendapat hasil perkalian matC per baris.
  ```
	printf("C = \n");
	for (int i=0; i<4; i++){
		for(int j=0; j<5; j++){
			printf("%4d", matC[i][j]);		
		}
		printf("\n");
	}
```
Kita print matriks C.
```
	key_t key;
   	int ID;
   	struct memory *ptr;

   	key = ftok("key",256);
   	ID = shmget(key,sizeof(struct memory),IPC_CREAT|0666);
   	ptr = (struct memory *) shmat(ID, NULL, 0); 
	ptr->status = UNREADY;
	int j = 0;
	int k = 0;
```
Deklarasi key dan ID untuk digunakan dalam proses shared memory. Kita juga membuat pointer ptr untuk menampung struktur data memory. Kita juga mendeklarasikan `int j = 0` dan `int k = 0` untuk persiapan dibawah. Disini, status masih `UNREADY` sehingga data belum kita share.
```
	for(int i = 0; i < 20; i++){
		if(k > 4){
	   		j++;
	   		k = 0;
		}
		ptr->data[i] = matC[j][k];
		k++;
	}
	ptr->status = READY;
```
Disini kita memasukkan data matriks C 2 dimensi menjadi matriks 1 dimensi pada 
`ptr->data[i] = matC[j][k]`
Lalu, kita update status menjadi `READY` sehingga data bisa kita share melalui shared memory.
```
	while (ptr->status != TAKEN)
	    sleep(1);

	shmdt((void *) ptr);
	shmctl(ID, IPC_RMID, NULL);
	exit(0);

	return 0;
}
```
Selama `status` masih belum `TAKEN`, yang berarti bahwa program 4b belum selesai berjalan (berarti pula shared memory belum boleh diputus/detach). Maka kita sleep. Jika sudah, maka kita gunakan `shmdt` untuk detach shared memory dan `shmctl` untuk melakukan kontrol sehingga status IPC shared memory di-reset.

### Soal 4.b.
```
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define UNREADY -1
#define READY 0
#define TAKEN 1
```
Diatas merupakan beberapa header file yang akan digunakan dalam pengerjaan soal 4. Terdapat juga beberapa definisi: 
`UNREADY -1` adalah status dimana shared memory belum connect;
`READY 0` adalah status dimana shared memory siap menjadi memory attachment dalam file lain;
`TAKEN 1` adalah status dimana memory sharing sudah selesai. 
```
struct memory{
    int status;
    int data[256];
};

int baris = 0;
int row = 0;
int matC[4][5];
typedef long long ll;
```
Diatas terdapat beberapa deklarasi. Yang pertama adalah struktur data memory yang memiliki element `int status` yang akan menjadi buffer menampung status `READY`, `TAKEN`, dan `UNREADY`  dan array `int data[256]` yang digunakan untuk menampung data matriks 2 dimensi hasil perkalian matriks A dan B yaitu matriks C yang telah dikonversi menjadi bentuk 1 dimensi. Lalu dideklarasikan pula matriks C dari soal 4a, karena kita harus mendeklarasikannya agar bisa digunakan untuk melakukan shared memory hasil dari matriks C. Kita juga mendeklarasikan tipe `long long`, agar menjadi lebih mudah digunakan, menjadi `ll` (Tidak terlalu berguna karena akhirnya soalnya bukan faktorial melainkan penambahan). 
```
void* factorial(void* arg){
	int i = *((int*)arg);
	free(arg);
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
```
Diatas merupakan fungsi factorial (ini karena soal sebelum revisi adalah faktorial) yang digunakan untuk menambahkan setiap elemen dari matriks itu  dengan ketentuan (misal: 5, maka 1+2+3+4+5 = 15).
Pertama kita deklarasikan int i sebagai penampung argumen yang akan berbentuk 20 elemen dari matriks C[4][5] yang telah dikonversi menjadi bentuk 1 dimensi. Kemudian kita deklarasikan `ll total = 0` atau `long long total = 0` sebagai hasil dari penambahan setiap elemen.
```	for(int j = i; j > 0 ;j--){
		total += j;
	}
```
Disini kita misalkan kondisi awal for j = i (misal i = 5), maka j akan terus dikurangi sampai j = 1, sehingga ketika `total += j`, maka akan menjadi (5+4+3+2+1 = 15). Kemudian jika row, atau satu baris sudah terdapat 5 elemen, maka `printf("\n")` untuk membuat newline dibawahnya, terus repeat step nya dengan `row++` untuk mengakses elemen array berikutnya. 
```
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
```
Seperti pada program sebelumnya, menyiapkan shared memory.
```
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
```
Jika status masih belum `READY` maka jangan lakukan apapun. Kemudian dibawhanya kita print hasil dari Matriks C atau output program 4a diatas. Dengan cara mengcopy data dari array `data[256]` pada struct memory ke `matC` dengan `memcpy`.
Kemudian kita print per baris hasilnya, kemudian repeat dengan `baris++` hingga baris sudah 4(`while (baris < 4)`).
```
	printf("Matriks C Faktorial: \n");
	pthread_t tid[20];
	for(int i = 0; i < 20;i++){
		int *x =  malloc(sizeof(*x));
		*x = ptr->data[i];
		pthread_create(&(tid[i]), NULL, &factorial, x);
		pthread_join(tid[i], NULL);

	}
```
Menampilkan hasil dari penambahan per elemen dari matriks C dengan menggunakan elemen dari `data[i]` pada struct memory yang ditampung dengan pointer ptr. Lalu kita create thread sebanyak 20 kali (karena terdapat 20 elemen dalam matriks 4x5). Kemudian setelah create thread tersebut kita join.
```
	ptr->status = TAKEN; 
	shmdt((void *) ptr);
	printf("\n");
	return 0;
}
```
Kita rubah status menjadi `TAKEN` lalu kita detach dengan `shmdt`.

### Soal 4.c.
```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

int main() {
  int fd[2];
  pid_t pid;

  pipe(fd);

  pid = fork();
  if (pid == 0) {
    dup2(fd[1], 1);
    close(fd[0]);
    close(fd[1]);
    execl("/bin/ls", "ls", NULL);
  }
  while(wait(NULL) > 0);

  pid = fork();
  if (pid == 0) {
    dup2(fd[0], 0);
    close(fd[0]);
    close(fd[1]);
    execl("/usr/bin/wc", "wc", "-l", NULL);
  }
  return 0;
}
```
Disini cukup singkat, kita buat deklarasi fd[2] untuk digunakan sebagai jumlah pipe (1 masuk 1 keluar) nanti. Kemudian `pid` untuk fork kita. Kemudian kita gunakan `dup2(fd[1]), 1` untuk melakukan proses write pada `fd[1]` melalui pipe ke proses berikutnya. Kemudian kita close agar tidak error. Lalu kita ` execl("/bin/ls", "ls", NULL)` untuk menggunakan command `ls`.

Kemudian, kita wait hingga selesai, barulah child kedua jalan, kita gunakan `dup2(fd[0]), 0` untuk melakukan proses read melalui pipe pada fd[0] (yang merupakan perintah `ls` sebelumnya). Kemudian kita close agar tidak error. Terakhir kita gunakan `execl("/usr/bin/wc", "wc", "-l", NULL)` untuk mendapatkan jumlah file dalam direktori ini.
## Kendala
1. Soal terlalu kompleks untuk pemahaman kelompok kami yang sederhana 
2. Minggu ETS sehingga fokus dalam mengerjakan tugas menjadi terbagi-bagi
3. Soal 1 dan 2 kurang bisa dimengerti secara jelas, sehingga tidak terselesaikan sama sekali.
