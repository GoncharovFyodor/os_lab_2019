#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
//#define SERV_PORT 10050
//#define BUFSIZE 100
#define SADDR struct sockaddr
typedef struct {
    int         serv_port;
    int         bufsize;
} param;
int main(int argc, char **argv) {
  param p;
    p.serv_port = -1;
    p.bufsize = -1;
    //int status;
    while (true) {
        int current_optind = optind ? optind : 1;
    static struct option options[] = {{"serv_port", required_argument, 0, 0},
                                      {"bufsize", required_argument, 0, 0},
                                      {0, 0, 0, 0}};
    int option_index = 0;
    int c = getopt_long(argc, argv, "f", options, &option_index);
    if (c == -1) break;

    switch (c) {
      case 0:
        switch (option_index) {
          case 0:
            p.serv_port = atoi(optarg);
            if (p.serv_port <= 0) {
                printf("serv_port is a positive number\n");
                return 1;
            }
            // error handling
            break;
          case 1:
            p.bufsize = atoi(optarg);
            if (p.bufsize <= 0) {
                printf("bufsize is a positive number\n");
                return 1;
            }
            // error handling
            break;
          default:
            printf("Index %d is out of options\n", option_index);
        }
        break;
      case 'f':
        break;
      default:
        printf("getopt returned character code 0%o?\n", c);
    }
  }
  if (optind < argc) {
    printf("Has at least one no option argument\n");
    return 1;
  }

  if (p.serv_port == -1 || p.bufsize == -1) {
    printf("Usage: %s --serv_port 10050 --bufsize 100 \n",
           argv[0]);
    return 1;
  }  
  const size_t kSize = sizeof(struct sockaddr_in);
  int lfd, cfd;
  int nread;
  char buf[p.bufsize];
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(p.serv_port);

  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }

  while (1) {
    unsigned int clilen = kSize;

    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      exit(1);
    }
    printf("connection established\n");

    while ((nread = read(cfd, buf, p.bufsize)) > 0) {
      write(1, &buf, nread);
    }

    if (nread == -1) {
      perror("read");
      exit(1);
    }
    close(cfd);
  }
}
