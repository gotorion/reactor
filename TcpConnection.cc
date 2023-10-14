#include "TcpConnection.h"

#include <sys/socket.h>

#include <sstream>
using std::ostringstream;

TcpConnection::TcpConnection(int fd)
    : _sock(fd),
      _sockIO(fd),
      _localAddr(getLocalAddr()),
      _peerAddr(getPeerAddr()),
      _onConnect(nullptr),
      _onMessage(nullptr),
      _onClose(nullptr) {}

TcpConnection::~TcpConnection() {}

void TcpConnection::send(const string &msg) {
  _sockIO.writen(msg.c_str(), msg.size());
}

string TcpConnection::receive() {
  char buf[4096];
  _sockIO.readLine(buf, sizeof(buf));
  return static_cast<string>(buf);
}

bool TcpConnection::isExit() {
  char buf[10] = {0};
  int ret = ::recv(_sock.fd(), buf, sizeof(buf), MSG_PEEK);
  return ret == 0;
}

void TcpConnection::setConnect(const TcpCallback &cb) { _onConnect = cb; }

void TcpConnection::setMessage(const TcpCallback &cb) { _onMessage = cb; }

void TcpConnection::setClose(const TcpCallback &cb) { _onClose = cb; }

void TcpConnection::handleConnectCallback() {
  if (_onConnect) {
    _onConnect(shared_from_this());
  } else {
    ::printf("_onConnect is nullptr\n");
  }
}

void TcpConnection::handleMessageCallback() {
  if (_onMessage) {
    _onMessage(shared_from_this());
  }
}

void TcpConnection::handleCloseCallback() {
  if (_onClose) {
    _onClose(shared_from_this());
  }
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
  return InetAddress(addr);  // construct a tmp InetAddress
}

InetAddress TcpConnection::getPeerAddr() {
  struct sockaddr_in addr;
  socklen_t len = sizeof(struct sockaddr_in);
  int ret = getpeername(_sock.fd(), (struct sockaddr *)&addr, &len);
  if (-1 == ret) {
    ::perror("getpeername");
    exit(-1);
  }
  return InetAddress(addr);  // construct a tmp InetAddress
}
