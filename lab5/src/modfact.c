#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <malloc.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
int result=0;
long int modfact(int ,int);
void do_one_thing(void* );
void do_wrap_up(long int);
typedef struct {
    int         k;
    int         pnum;
    int         mod;
    long int    result;
} param;
int main(int argc, char **argv) 
{
    param p;
    p.k = -1;
    p.pnum = -1;
    p.mod = -1;
    p.result=1;
    //int status;
    bool by_files = true;

    while (true) {
        int current_optind = optind ? optind : 1;
    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"pnum", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"by_files", no_argument, 0, 'f'},
                                      {0, 0, 0, 0}};
    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);
    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            p.k = atoi(optarg);
            if (p.k <= 0) {
                printf("k is a positive number\n");
                return 1;
            }
            // error handling
            break;
          case 1:
            p.pnum = atoi(optarg);
            if (p.pnum <= 0) {
                printf("pnum is a positive number\n");
                return 1;
            }
            // error handling
            break;
          case 2:
            p.mod = atoi(optarg);
            if (p.mod <= 0) {
                printf("mod is a positive number\n");
                return 1;
            }
            // error handling
            break;
          case 3:
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

  if (p.k == -1 || p.pnum == -1 || p.mod == -1) {
    printf("Usage: %s --k \"num\" --pnum \"num\" --mod \"num\" \n",
           argv[0]);
    return 1;
  }
  struct timeval start_time;
  gettimeofday(&start_time, NULL);
  pthread_t* threads;
  threads=(pthread_t*)malloc(p.pnum*sizeof(pthread_t));
  for(int i=0;i<p.pnum;i++){
    if (pthread_create(&threads[i], NULL, (void *)do_one_thing,
    			  (void*)&p) != 0) {
      perror("pthread_create");
      exit(1);
    }
    if (pthread_join(threads[i], NULL) != 0) {
    perror("pthread_join");
    exit(1);
    }
  }

  do_wrap_up((&p)->result);
   struct timeval finish_time;
  gettimeofday(&finish_time, NULL);

  double elapsed_time = (finish_time.tv_sec - start_time.tv_sec) * 1000.0;
  elapsed_time += (finish_time.tv_usec - start_time.tv_usec) / 1000.0;
  printf("Elapsed time: %fms\n", elapsed_time);
  return 0;
}
long int modfact(int k,int mod){
    //printf("%d %d\n",k,mod);
    long int fact=1;
    int c;
    for (c = 1; c <= k; c++){
        fact = (fact * c)%mod;
        //printf("%d: %ld\n",c,fact);
    }
    return fact;
}
void do_one_thing(void *arg) {
    param* p = (param *)arg;
  int i, j, x;
  unsigned long g;
  long int work;
  for (i = 0; i < 50; i++) {      
    pthread_mutex_lock(&mut);
    printf("doing one thing\n");
    work = p->result;
    work=modfact(p->k,p->mod);
    printf("modfact = %ld\n", work);
    for (g = 0; g < 500000; g++)
      ;                 /* long cycle */
    p->result = work; /* write back */
    pthread_mutex_unlock(&mut);
  }
}
void do_wrap_up(long int result) {
  //int total;
  printf("All done, result = %ld\n", result);
}