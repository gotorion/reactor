#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__

#include "InetAddress.h"
#include "SockIO.h"
#include "Socket.h"
#include <string>
using std::string;

class TcpConnection {
public:
  TcpConnection(int fd);
  ~TcpConnection();
  void send(const string &msg);
  string receive();
  // for debug
  string toString();

private:
  InetAddress getLocalAddr();
  InetAddress getPeerAddr();

private:
  Socket _sock;
  SockIO _sockIO;

  InetAddress _localAddr;
  InetAddress _peerAddr;
};

#endif