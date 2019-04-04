#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>

#include "find_min_max.h"
#include "utils.h"
int main(int argc, char **argv) {
  int seed = -1;
  int array_size = -1;
  int pnum = -1;
  int timeout = -1;
  int status;
  bool by_files = true;

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"seed", required_argument, 0, 0},
                                      {"array_size", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"timeout", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);

    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            seed = atoi(optarg);
            if (seed <= 0) {
                printf("seed is a positive number\n");
                return 1;
            }
            // error handling
            break;
          case 1:
            array_size = atoi(optarg);
            if (array_size <= 0) {
                printf("array_size is a positive number\n");
                return 1;
            }
            // error handling
            break;
          case 2:
            pnum = atoi(optarg);
            if (array_size <= 0) {
                printf("pnum is a positive number\n");
                return 1;
            }
            // error handling
            break;
          case 3:
            timeout = atoi(optarg);
            if (array_size <= 0) {
                printf("timeout is a positive number\n");
                return 1;
            }
            break;
          case 4:
            by_files = true;
            break;
          defalut:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        by_files = true;
        break;

      case '?':
        break;

      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }

  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (seed == -1 || array_size == -1 || pnum == -1) {
    printf("Usage: %s --seed \"num\" --array_size \"num\" --pnum \"num\" \n",
           argv[0]);
    return 1;
  }

  int *array = malloc(sizeof(int) * array_size);
  GenerateArray(array, array_size, seed);
  int active_child_processes = 0;

  struct timeval start_time;
  gettimeofday(&start_time, NULL);
  FILE *fpmin,*fpmax;
  int pipefd[2];
  for (int i = 0; i < pnum; i++) {
   if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
    }
    pid_t child_pid = fork();
    if (child_pid >= 0) {
      // successful fork
      active_child_processes += 1;
      if (child_pid == 0) {
        // child process
        // parallel somehow
        if (by_files) {
          // use files here
          printf("FILES:\n");
          struct MinMax file_min_max;
          file_min_max=GetMinMax(array,0,array_size);
          fpmin=fopen("min.txt","w");
          fprintf(fpmin,"%d",file_min_max.min);
          fclose(fpmin);
          fpmax=fopen("max.txt","w");
          fprintf(fpmax,"%d",file_min_max.max);
          fclose(fpmax);
          waitpid(child_pid,&status,WNOHANG);
          //exit(0);
        } else {
          // use pipe here
          printf("PIPE:\n");
          close(pipefd[0]);
          struct MinMax pipe_min_max=GetMinMax(array,0,array_size);
          int pipe_min=pipe_min_max.min;
          int pipe_max=pipe_min_max.max;
          write(pipefd[1], &pipe_min, sizeof(int));
          write(pipefd[1], &pipe_max, sizeof(int));
          close(pipefd[1]);          /* Читатель видит EOF */   
          wait(NULL);
          //exit(EXIT_SUCCESS);
        }  
        return 0;
      }else{   
        alarm(timeout);
        kill(child_pid,SIGALRM);
      }
    } else {
      printf("Fork failed!\n");
      return 1;
    }
    
  };
  while (active_child_processes > 0) {
    // your code here
    active_child_processes -= 1;
  }

  struct MinMax min_max;
  min_max.min = INT_MAX;
  min_max.max = INT_MIN;
  for (int i = 0; i < pnum; i++) {
    int min = INT_MAX;
    int max = INT_MIN;

    if (by_files) {
      // read from files
      fpmin=fopen("min.txt","r");
      fscanf(fpmin,"%d",&min);
      fclose(fpmin);
      fpmax=fopen("max.txt","r");
      fscanf(fpmax,"%d",&max);
      fclose(fpmax);
    } else {
        // read from pipes
        read(pipefd[0], &min, sizeof(int));
        read(pipefd[0], &max, sizeof(int));
        close(pipefd[0]);
        //_exit(EXIT_SUCCESS);
    }

    if (min < min_max.min) min_max.min = min;
    if (max > min_max.max) min_max.max = max;
  }
  struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;

  free(array);

  printf("Min: %d\n", min_max.min);
  printf("Max: %d\n", min_max.max);
  printf("Elapsed time: %fms\n", elapsed_time);
  fflush(NULL);
  
  return 0;
}
