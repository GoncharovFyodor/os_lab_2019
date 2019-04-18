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

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
void *function1()
    {
        printf("f1\n");
       pthread_mutex_lock(&lock1);
       pthread_mutex_lock(&lock2);
       pthread_mutex_lock(&lock2);
       pthread_mutex_lock(&lock1);
       
       printf("f1 stopped\n");
    } 
void *function2()
    {
        printf("f2\n");
       pthread_mutex_lock(&lock2);
       pthread_mutex_lock(&lock1);
       pthread_mutex_lock(&lock1);
       pthread_mutex_lock(&lock2);
       
       printf("f2 stopped\n");
    } 
  
int main()
    {
        printf("main_start\n");
        pthread_t thread1,thread2;
       pthread_create(&thread1, NULL, function1, NULL);
       pthread_create(&thread2, NULL, function1, NULL);
       printf("main\n");
       while(true)
       {
           printf("Wait\n");
         sleep(1000);  
       }
    }