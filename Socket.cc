#include "Socket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

Socket::Socket() {
  _fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (-1 == _fd) {
    ::perror("socket");
  }
}

Socket::Socket(int fd)
  : _fd(fd) {}

Socket::~Socket() {}

int Socket::fd() const {
  return _fd;
}
