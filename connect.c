#include <dlfcn.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "connect.h"


int req_init(proxy_req *req) {
  if (req == NULL)
    return 0;

  memset(req->userid, 0, USERID_SIZE);
  strncpy(req->userid, USERID, USERID_SIZE);

  req->vn = SOCKS_VERSION;
  req->cd = CD_CONNECT;

  return 1;
}

int proxy_socket_init(connect_fn ori, struct sockaddr_in *dist) {
  int sock_fd = -1;
  struct sockaddr_in addr;
  proxy_req req;
  proxy_resp resp;

  if (ori == NULL)
    return -1;
  
  // open socket for the proxy connection
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd < 0)
    return -1;

  // initialize the address for the proxy
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PROXY_PORT);
  addr.sin_addr.s_addr = inet_addr(PROXY_HOST);

  // connect to the proxy
  if (ori(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    goto fail;

  // initialize the proxy connect request
  req_init(&req);
  req.dstport = dist->sin_port;
  req.dstip = dist->sin_addr.s_addr;

  // send connect request
  if (write(sock_fd, &req, sizeof(req)) == -1)
    goto fail;

  // receive response
  memset(&resp, 0, sizeof(resp));
  if (read(sock_fd, &resp, sizeof(resp)) == -1)
    goto fail;

  // validate proxy response
  if (resp.vn != VN_REPLY || resp.cd != RC_GRANTED)
    goto fail;

  // return the socket fd
  return sock_fd;
fail:
  close(sock_fd);
  return -1;
}

int connect(int socket, const struct sockaddr *address,
    socklen_t address_len) {

  connect_fn ori = NULL;
  struct sockaddr_in *addr = NULL;
  int proxy_socket;

  // get the original connect function memory address
  ori = dlsym(RTLD_NEXT, "connect");
  if (ori == NULL)
    return -1;

  // validate before any execute otherwise skip
  if (socket < 0 || address == NULL || address_len < 0)
    goto skip;

  // check if the address is of family ipv4 otherwise skip
  if (address->sa_family != AF_INET)
    goto skip;

  // cast the address into sockaddr_in
  addr = (struct sockaddr_in *)address;

  // initialize the proxy connection and get the socket
  proxy_socket = proxy_socket_init(ori, addr);
  if (proxy_socket < 0)
    return -1;

  // close the original socket
  // and set the proxy_socket fd to be the same as the original socket
  return dup2(proxy_socket, socket);

  // use the default connect with default address
skip:
  return ori(socket, address, address_len);
}
