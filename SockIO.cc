#include "SockIO.h"

#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

SockIO::SockIO(int fd) : _fd(fd) {}

SockIO::~SockIO() { close(_fd); }

int SockIO::readn(char *buf, int len) {
  int left = len;
  char *pstr = buf;
  int ret = 0;

  while (left > 0) {
    do {
      ret = ::recv(_fd, pstr, left, MSG_DONTWAIT);
    } while (ret == -1 && errno == EINTR);
    if (-1 == ret) {
      ::perror("recv in readn");
      return 0;
    } else if (0 == ret) {  // no data
      break;
    } else {
      pstr += ret;
      left -= ret;
    }
  }
  return len - left;
}

int SockIO::readLine(char *buf, int len) {
  int left = len - 1;
  int ret = 0, total = 0;
  char *pstr = buf;

  while (left > 0) {
    ret = ::recv(_fd, pstr, left, MSG_PEEK);
    if (-1 == ret && errno == EINTR) {
      continue;
    } else if (-1 == ret) {
      ::perror("recv in readLine");
      return 0;
    } else if (0 == ret) {
      break;
    } else {
      for (int i = 0; i < ret; ++i) {
        if (pstr[i] == '\n') {
          int sz = i + 1;
          readn(pstr, sz);
          pstr += sz;
          *pstr = '\0';
          return total + sz;
        }
      }
      readn(pstr, ret);
      total += ret;
      left -= ret;
      pstr += ret;
    }
  }
  *pstr = '\0';
  return total - left;
}

int SockIO::writen(const char *buf, int len) {
  int left = len;
  const char *pstr = buf;
  while (left > 0) {
    int ret = ::send(_fd, pstr, left, 0);
    if (-1 == ret && errno == EINTR) {
      continue;
    } else if (-1 == ret) {
      ::perror("send in writen");
      return -1;
    } else if (0 == ret) {
      break;
    } else {
      left -= ret;
      pstr += ret;
    }
  }
  return len - left;
}