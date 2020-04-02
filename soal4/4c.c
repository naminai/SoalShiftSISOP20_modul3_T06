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
    close(fd[2]);
    close(fd[3]);
    execl("/usr/bin/wc", "wc", "-l", NULL);
  }
  return 0;
}

