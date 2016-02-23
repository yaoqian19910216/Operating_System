/*
    CS570 Fall 2015
   Instructor Carroll 
   Qian Yao   Red ID 815260362
   San Diego State University
 */#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "p2.h"
#include <signal.h>
extern char **environ;

void parse(char *argv[], char *inputfile, char *outputfile, char *outputstyle, int *argc, int *inflag, int *outflag){
   int c = 0;
   while(1){
      char *commandPointer = malloc(sizeof(char) * STORAGE);
      c = getword(commandPointer);
      if(c == -1 || c == 0) break;
      if(strcmp(commandPointer,"<") == 0 && *inflag == 0){
         *inflag = 1;
         continue;
      }
      if(strcmp(commandPointer,">") == 0 && *outflag == 0){
         *outflag  = 1;
         strcpy(outputstyle,"w");
         continue;
      }
      if(strcmp(commandPointer,">>") == 0 && *outflag == 0){ 
         *outflag  = 2;
         strcpy(outputstyle,"a");
         continue;
      }
      if(strcmp(commandPointer,">&") == 0 && *outflag == 0){
         *outflag  = 3;
         strcpy(outputstyle,"w_");
         continue;
      }
      if(strcmp(commandPointer,">>&") == 0 && *outflag == 0){
         *outflag  = 4;
         strcpy(outputstyle,"a_");
         continue;
      }
      if(*inflag == 1){
         strcpy(inputfile,commandPointer);
         *inflag = 100;
         continue;
      }
      if(*outflag == 1){
         strcpy(outputfile,commandPointer);
         *outflag = 11;
         continue;
      }    
      if(*outflag == 2){
         strcpy(outputfile,commandPointer);
         *outflag = 22;
         continue;
      }
      if(*outflag == 3){
         strcpy(outputfile,commandPointer);
         *outflag = 33;
         continue;
      }
      if(*outflag == 4){
         strcpy(outputfile,commandPointer);
         *outflag = 44;
         continue;
      }  
      argv[(*argc)++] = commandPointer;
   }
}

void myhandler(int s){
   while(waitpid(0,NULL,WNOHANG)> 0);
}

void myhandler1(int sum){
   sleep(1);
}

int main(){
   int inputRedirect;
   pid_t pid;
   char *devNull = "/dev/null";
   char c;
   char *s;
   char *home = getenv("HOME");
   int len;
   char *argv[MAXITEM];
   char pwd[STORAGE];
   signal(SIGCHLD, myhandler);
   signal(SIGTERM, myhandler1);
   while(1){
     int inflag = 0;/* inflag = 1 means file input*/
     int outflag = 0;/* outflag = 1 means file output, outflag = 2 means file output append*/
     int argc = 0;
     int status = 0;
     int background = 0;
     char *inputfile = malloc(sizeof(char) * STORAGE);
     char *outputfile = malloc(sizeof(char) * STORAGE);
     char *outputstyle = malloc(sizeof(char) * STORAGE);
     char *commandPointer = malloc(sizeof(char) * STORAGE);
     char **argv = malloc(sizeof(char) * MAXITEM);
     int pipes[10][2];

     printf("%s","p2: ");
     getcwd(pwd, STORAGE);
     fflush(stderr);  

     if((c = getchar()) == EOF){
          break;
     }else{
         ungetc(c, stdin);
     }
     parse(argv, inputfile, outputfile, outputstyle, &argc, &inflag, &outflag);
     
     if(argc == 0){
         continue;
     }

     if(strcmp(argv[0], "exit") == 0)
         continue; 
   
     argv[argc] = NULL;
     if(strcmp(argv[0],"cd") == 0){
         if(argc == 1){
            if(chdir(home) < 0){
               printf("Unable to chidr to HOME (%s)\n", home);
            }
         }else{
            if(argv[1][0] == '/'){
               if(chdir(argv[1]) < 0){
                  printf("Unable to chidr to (%s)\n", argv[1]);  
               }
            }else{
              char tmppwd[STORAGE];
              sprintf(tmppwd, "%s/%s",pwd,argv[1]);
              if(chdir(tmppwd) < 0){
                  printf("Unable to chidr to(%s)\n", tmppwd);
              }
            }
         }
         continue; 
     }
      
     if(strcmp(argv[0],"ECHO") == 0){
           int d =1 ; 
            while(d < argc){
              printf("%s ",argv[d]);
              d++;
            }
            printf("\n");
            continue;
     }
       

    if(argc > 0 && strcmp(argv[argc-1],"&")==0){
           background = 1;
           argc--;
           argv[argc] = NULL;
    }
   
    int in_df;
    int out_df;
    int filedes[2];
    int filedes2[2];

    int pipenumber = 0;
    int err = -1;
    int end = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    char *command[256];

    while(argv[l] != NULL){
         if(strcmp(argv[l],"|") == 0){
               pipenumber++;
         }
         l++;
    }
    pipenumber++;

    if(pipenumber == 1){
       pid = fork();
       if(pid == 0){
                 if(background != 0){
                     inputRedirect = open(devNull, O_RDONLY);
                     dup2(inputRedirect, 0);
                     close(inputRedirect);
                 }
                 
                 if(inflag != 0){
                     if((in_df = open(inputfile, O_RDONLY))< 0){
                         perror("Input File Error");
                     }  
                     dup2(in_df,0);
                     close(in_df);
                 }

                 if(outflag != 0){
                      printf("%s %s\n","test",outputstyle); 
                      if(strcmp(outputstyle, "w") == 0){
                             if((out_df = open(outputfile, O_WRONLY | O_EXCL | O_CREAT, 0600)) < 0){
                                    perror("Output File Error");
                             }
                             dup2(out_df,1);
                             close(out_df);     
                       }
                      
                       if(strcmp(outputstyle, "w_") == 0){
                             if((out_df = open(outputfile, O_WRONLY | O_EXCL | O_CREAT, 0600)) < 0){
                                    perror("Output File Error");
                             }
                             dup2(out_df,1);
                             dup2(out_df,2);
                             close(out_df);
                       }
                     
                       if(strcmp(outputstyle, "a") == 0){
                             if((out_df = open(outputfile, O_WRONLY | O_APPEND )) < 0){
                                    perror("Output File Error");
                             }
                             dup2(out_df,1);
                             close(out_df);
                       }
             
                       if(strcmp(outputstyle, "a_") == 0){
                             if((out_df = open(outputfile, O_WRONLY | O_APPEND )) < 0){
                                    perror("Output File Error");
                             }
                             dup2(out_df,1);
                             dup2(out_df,2);
                             close(out_df);
                       }
                } 
               
                 if(execvp(argv[0],argv) < 0){
                     perror("Execution error") ;
                     exit(1);
                 }
    }else{
     if(background == 0){
             pid = wait(&status);
             if(WIFEXITED(status) && WEXITSTATUS(status) != 0){
                fprintf(stderr,"Exit %d\n",WEXITSTATUS(status));
             }
         }else{
             printf("%s %s %d %s\n",argv[0],"[",pid,"]");
         }
    }
   }else{
     while(argv[j] != NULL && end != 1){
           k = 0;
           while(strcmp(argv[j],"|") != 0){
              command[k] = argv[j];
              j++; 
              if(argv[j] == NULL){
                end = 1;
                k++;
                break;
              }
              k++;
           }
           
           command[k] = NULL;
           j++;

           if(i % 2 != 0){
             pipe(filedes);
           }else{
             pipe(filedes2);
           }

           pid = fork();

           if(pid == 0){
               if(i == 0){
                 if(background != 0){
                     inputRedirect = open(devNull, O_RDONLY);
                     dup2(inputRedirect, 0);
                     close(inputRedirect);
                 }
                 
                 if(inflag != 0){
                     if((in_df = open(inputfile, O_RDONLY))< 0){
                         perror("Input File Error");
                     }  
                     dup2(in_df,0);
                     close(in_df);
                 }
                 
                 dup2(filedes2[1],1);
            
              }else if(i == pipenumber - 1){
                  if(pipenumber %2 != 0){
                      dup2(filedes[0],0);
                  }else{
                      dup2(filedes2[0],0);
                  }
              
                  if(outflag != 0){
                       if(strcmp(outputstyle, "w") == 0){
                             if((out_df = open(outputfile, O_WRONLY | O_EXCL | O_CREAT, 0600)) < 0){
                                    perror("Output File Error");
                             }
                             dup2(out_df,1);
                             close(out_df);     
                       }
                      
                       if(strcmp(outputstyle, "w_") == 0){
                             if((out_df = open(outputfile, O_WRONLY | O_EXCL | O_CREAT, 0600)) < 0){
                                    perror("Output File Error");
                             }
                             dup2(out_df,1);
                             dup2(out_df,2);
                             close(out_df);
                       }
                     
                       if(strcmp(outputstyle, "a") == 0){
                             if((out_df = open(outputfile, O_WRONLY | O_APPEND )) < 0){
                                    perror("Output File Error");
                             }
                             dup2(out_df,1);
                             close(out_df);
                       }
             
                       if(strcmp(outputstyle, "a_") == 0){
                             if((out_df = open(outputfile, O_WRONLY | O_APPEND )) < 0){
                                    perror("Output File Error");
                             }
                             dup2(out_df,1);
                             dup2(out_df,2);
                             close(out_df);
                       }
                 }
           }else{
               if(i % 2 != 0){
                 dup2(filedes2[0],0);
                 dup2(filedes[1],1);
                }else{
                 dup2(filedes2[1],1);
                 dup2(filedes[0],0);
                }
           }

         /*inputRedirect = open(devNull, O_RDONLY);
           dup2(inputRedirect, STDIN_FILENO);
           close(inputRedirect);*/
         if(execvp(command[0], command) < 0){
             perror("Execution error") ;
             exit(1);
         }
       }else{
         if(i == 0){
            close(filedes2[1]);
         }else if(i == pipenumber - 1){
            if(pipenumber % 2 != 0){
               close(filedes[0]);
            }else{
               close(filedes2[0]);
            }
         }else{
            if(i % 2 != 0){
               close(filedes2[0]);
               close(filedes[1]);
            }else{
               close(filedes[0]);
               close(filedes2[1]);
            }
         }
         if(background == 0){
             pid = wait(&status);
             if(WIFEXITED(status) && WEXITSTATUS(status) != 0){
                fprintf(stderr,"Exit %d\n",WEXITSTATUS(status));
             }
         }else{
             printf("%s %s %d %s\n",argv[0],"[",pid,"]");
         }
         i++;
      }
    }
    }
   }
   killpg(getpid(), SIGTERM);
   printf("p2 terminated.\n");
   exit(0);
}












