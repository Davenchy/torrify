#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <sys/socket.h>

#define USERID "TORRIFY"
#define USERID_SIZE 8
#define PROXY_HOST "127.0.0.1"
#define PROXY_PORT 9050

#define SOCKS_VERSION 4

// Commands Codes
#define VN_REPLY 0
#define CD_CONNECT 1

// Socks Reply Codes
#define RC_GRANTED 90
#define RC_REJECTED 91

typedef int (*connect_fn)(int, const struct sockaddr*, socklen_t);

typedef struct {
  uint8_t vn;
  uint8_t cd;
  uint16_t dstport;
  uint32_t dstip;
  char userid[USERID_SIZE];
} proxy_req;

typedef struct {
  uint8_t vn;
  uint8_t cd;
  uint16_t srcport;
  uint32_t srcip;
} proxy_resp;

int req_init(proxy_req*);
int proxy_socket_init(connect_fn, struct sockaddr_in *);

#endif // MAIN_H
