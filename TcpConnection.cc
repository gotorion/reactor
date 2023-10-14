#include "TcpConnection.h"
#include <sstream>
#include <sys/socket.h>
using std::ostringstream;

TcpConnection::TcpConnection(int fd)
    : _sock(fd), _sockIO(fd), _localAddr(getLocalAddr()),
      _peerAddr(getPeerAddr()) {}

TcpConnection::~TcpConnection() {}

void TcpConnection::send(const string &msg) {
  _sockIO.writen(msg.c_str(), msg.size());
}

string TcpConnection::receive() {
  char buf[4096];
  _sockIO.readLine(buf, sizeof(buf));
  return static_cast<string>(buf);
}

string TcpConnection::toString() {
  std::ostringstream oss;
  oss << _localAddr.ip() << ":" << _localAddr.port() << "--->" << _peerAddr.ip()
      << ":" << _peerAddr.port();
  return oss.str();
}

InetAddress TcpConnection::getLocalAddr() {
  struct sockaddr_in addr;
  socklen_t len = sizeof(struct sockaddr_in);
  int ret = getsockname(_sock.fd(), (struct sockaddr *)&addr, &len);
  if (-1 == ret) {
    ::perror("getsockname");
    exit(-1);
  }
  return InetAddress(addr); // construct a tmp InetAddress
}

InetAddress TcpConnection::getPeerAddr() {
  struct sockaddr_in addr;
  socklen_t len = sizeof(struct sockaddr_in);
  int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
  if (-1 == ret) {
    ::perror("getpeername");
    exit(-1);
  }
  return InetAddress(addr); // construct a tmp InetAddress
}
