#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <errno.h>
#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "libserver.h"
#define BUFFSIZE 255

struct Server {
  char ip[255];
  int port;
};

uint64_t MultModulo(uint64_t a, uint64_t b, uint64_t mod) {
  uint64_t result = 0;
  a = a % mod;
  while (b > 0) {
    if (b % 2 == 1)
      result = (result + a) % mod;
    a = (a * 2) % mod;
    b /= 2;
  }

  return result % mod;
}

bool ConvertStringToUI64(const char *str, uint64_t *val) {
  char *end = NULL;
  unsigned long long i = strtoull(str, &end, 10);
  if (errno == ERANGE) {
    fprintf(stderr, "Out of uint64_t range: %s\n", str);
    return false;
  }

  if (errno != 0)
    return false;

  *val = i;
  return true;
}

int main(int argc, char **argv) {
  uint64_t k = -1;
  uint64_t mod = -1;
  char servers[255] = {'\0'}; // TODO: explain why 255

  while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                      {"mod", required_argument, 0, 0},
                                      {"servers", required_argument, 0, 0},
                                      {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0: {
      switch (option_index) {
      case 0:
        ConvertStringToUI64(optarg, &k);
        // TODO: your code here
        break;
      case 1:
        ConvertStringToUI64(optarg, &mod);
        // TODO: your code here
        break;
      case 2:
        // TODO: your code here
        memcpy(servers, optarg, strlen(optarg));
        break;
      default:
        printf("Index %d is out of options\n", option_index);
      }
    } break;

    case '?':
      printf("Arguments error\n");
      break;
    default:
      fprintf(stderr, "getopt returned character code 0%o?\n", c);
    }
  }

  if (k == -1 || mod == -1 || !strlen(servers)) {
    fprintf(stderr, "Using: %s --k 1000 --mod 5 --servers /path/to/file\n",
            argv[0]);
    return 1;
  }

  // TODO: for one server here, rewrite with servers from file
  // пробуем открыть файл для чтения
    FILE* in_servers = fopen(servers, "r");
    // обрабатываем ошибку открытия файла
    if ( in_servers == NULL ) {
        perror( "Ошибка при открытии исходного файла" );
        return 1;
    }
 
    unsigned int servers_num = 0; // предполагаем, что строк ноль в файле - т.е. пустой
    char *arr[255];
    char *ips[255];
    printf( "%s %s\n", "Содержимое файла", servers );
    while(!feof(in_servers))
    {
        if(fgets(arr[servers_num], BUFSIZ, in_servers) != NULL){
            //*ips[servers_num] = '\0';
           // ips[servers_num]=strtok(arr[servers_num],":");
            printf("%i -> %s",servers_num+1,arr[servers_num]);
            ++servers_num;
            }
    }
    printf("\n");
    fclose(in_servers);
    printf("servers_num = %d\n",servers_num);    
    
    /*for(int i=0;i<servers_num;i++){
        ips[0]=strtok(arr[i],":");
        //*ips[0] = '\0';
        printf("%s -> %s\n",arr[i],ips[0]);
    }*/
  struct Server *to = malloc(sizeof(struct Server) * servers_num);
  pthread_t *tid = (pthread_t*)malloc(sizeof(pthread_t) * servers_num);
  int *sck = (int*)malloc(sizeof(int) * servers_num);
  // TODO: delete this and parallel work between servers
  for(int i=0;i<servers_num;i++){
        to[i].port = 20001+i;//Пока так
        memcpy(to[i].ip, "127.0.0.1", sizeof("127.0.0.1"));//Пока так
    }
  // TODO: work continiously, rewrite to make parallel
  for (int i = 0; i < servers_num; i++) {
    
    struct hostent *hostname = gethostbyname(to[i].ip);
    if (hostname == NULL) {
      fprintf(stderr, "gethostbyname failed with %s\n", to[i].ip);
      exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(to[i].port);
    server.sin_addr.s_addr = *((unsigned long *)hostname->h_addr);

    sck[i] = socket(AF_INET, SOCK_STREAM, 0);
    if (sck[i] < 0) {
      fprintf(stderr, "Socket creation failed!\n");
      exit(1);
    }

    if (connect(sck[i], (struct sockaddr *)&server, sizeof(server)) < 0) {
      fprintf(stderr, "Connection failed\n");
      exit(1);
    }

    // TODO: for one server
    // parallel between servers
    uint64_t begin = 1;
    uint64_t end = k;

    char task[sizeof(uint64_t) * 3];
    memcpy(task, &begin, sizeof(uint64_t));
    memcpy(task + sizeof(uint64_t), &end, sizeof(uint64_t));
    memcpy(task + 2 * sizeof(uint64_t), &mod, sizeof(uint64_t));

    if (send(sck[i], task, sizeof(task), 0) < 0) {
      fprintf(stderr, "Send failed\n");
      exit(1);
    }
}
for (int i = 0; i < servers_num; i++) {
    char response[sizeof(uint64_t)];
    if (recv(sck[i], response, sizeof(response), 0) < 0) {
      fprintf(stderr, "Receive failed\n");
      exit(1);
    }
    //printf("V\n");
    // TODO: from one server
    // unite results
    uint64_t answer = 0;
    memcpy(&answer, response, sizeof(uint64_t));
    printf("answer: %llu\n", answer);

    close(sck[i]);
  }
  /*for(int i = 0; i < servers_num; i++)
    free(ips[i]);
  free(ips);*/
  /*for(int i = 0; i < servers_num; i++){
    free(arr[i]);
}*/
  //free(arr); 
  //free(to);
  return 0;
}
