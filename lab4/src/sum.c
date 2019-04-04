#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>
#include "sum.h"
/*struct SumArgs {
  int *array;
  int begin;
  int end;
};*/

int Sum(const struct SumArgs *args) {
  int sum = 0;
  // TODO: your code here
  for (int i=args->begin;i<=args->end;i++){
      sum+=args->array[i];
  }
  //printf("%d\n",sum);
  return sum;
}