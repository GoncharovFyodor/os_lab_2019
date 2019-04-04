#ifndef VARIABLE
#define VARIABLE
// Write your header file here.
struct SumArgs {
  int *array;
  int begin;
  int end;
};
int Sum(const struct SumArgs *args);
#endif