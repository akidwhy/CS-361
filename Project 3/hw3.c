#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

void sigint_handler(int sig){
  char msg[] = "SIGINT HANDLED\n361>";
  write(1, msg, sizeof(msg));
}

void sigtstp_handler(int sig){
  char msg[] = "SIGTSTP HANDLED\n361>";
  write(1, msg, sizeof(msg));
}


void freestuff(char ** args, int i){
    // int n;
    // for(n = 0; n< i; n++){
    //   free(args[n]);
    // }
    // free(args);
}


int main(){
  
  while(1){

  int i = 0;
  int f_index;
  int file;
  char line[500];
  char *args[20];

  
  signal(SIGTSTP, sigtstp_handler);
  signal(SIGINT, sigint_handler);

    printf("CS361> ");
    
    fgets (line, 500, stdin);
    char *word = strtok(line, " ");
  
    if (strcmp(line, "exit\n") == 0){
      printf("exiting shell...\n");
      exit(1);
    }

    while (word) {
      //copy a word to the arg array
      args[i] = (char *) malloc(sizeof(char *) *100);
      strcpy(args[i], word);
      //get next word
      word = strtok(NULL, " ");
      i = i + 1;
    }

    strtok(args[i-1], "\n");
    args[i] = (char *) malloc(sizeof(char *) * 100);
    args[i] = (char *) 0;

    int pid = fork();
    if (pid == 0) {
      for (f_index = 0; f_index < i; f_index++){
          
          if (strncmp(args[f_index], ">>", 2) == 0) {
            // freopen(args[f_index+1], "a", stdout); 
            // break;     
            args[f_index] = (char *) 0;
            file = open(args[f_index + 1], O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
            dup2(file, 1);
            close(file);
            break;
           }

          else if (strncmp(args[f_index], ">", 1) == 0) {
            //freopen(args[f_index+1], "w", stdout); 
            //break;
            args[f_index] = (char *) 0;
            file = open(args[f_index + 1], O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
            dup2(file, 1);
            close(file);
            break;
          }
          else if (strncmp(args[f_index], "<", 1) == 0) {
            // freopen(args[f_index+1], "r", stdout); 
            // break;
            args[f_index] = (char *) 0;     
            file = open(args[f_index+1], O_RDONLY);
            dup2(file, 0);
            close(file);
            break;
          }
          
        }
        
        execvp(args[0], args);
        exit(0);
    } 
    else {
      printf("PID: %d\n", pid);
      int status;
      wait(&status);
      printf("Exit: %d\n", status);
    }

    freestuff(args, i);

    //print out our array
    // int j=0; 
    // for (;j<i;j++){
    //   printf("args[%d]: %s\n", j, args[j]);
    // }
    }
return 0;
}