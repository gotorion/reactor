#include "Acceptor.h"

#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cstdio>

Acceptor::Acceptor(const string &ip, unsigned short port)
    : _listenSock(), _addr(ip, port) {}

Acceptor::~Acceptor() {}

void Acceptor::setReuseAddr() {
  int reuse = 1;
  int ret = ::setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEADDR, &reuse,
                         sizeof(reuse));
  if (-1 == ret) {
    perror("setsockopt");
  }
}

void Acceptor::setReusePort() {
  int reuse = 1;
  int ret = ::setsockopt(_listenSock.fd(), SOL_SOCKET, SO_REUSEPORT, &reuse,
                         sizeof(reuse));
  if (-1 == ret) {
    perror("setsockopt");
  }
}

void Acceptor::setBind() {
  int ret = ::bind(_listenSock.fd(), (struct sockaddr *)_addr.getAddrPtr(),
                   sizeof(struct sockaddr_in));
  if (-1 == ret) {
    perror("bind");
  }
}

void Acceptor::setListen() {
  int ret = listen(_listenSock.fd(), 512);
  if (-1 == ret) {
    perror("listen");
  }
}

void Acceptor::getReady() {
  setReuseAddr();
  setReusePort();
  setBind();
  setListen();
}

int Acceptor::accept() {
  int fd = ::accept(_listenSock.fd(), nullptr, nullptr);
  if (-1 == fd) {
    perror("accpet");
    return -1;
  }
  return fd;
}

int Acceptor::fd() const { return _listenSock.fd(); }