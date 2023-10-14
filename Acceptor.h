#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__

#include "InetAddress.h"
#include "Socket.h"

class Acceptor {
 public:
  Acceptor(const string& ip, unsigned short port);
  ~Acceptor();
  void getReady();
  int accept();
 private:
  void setReuseAddr();
  void setReusePort();
  void setBind();
  void setListen();
 private:
  Socket _listenSock;
  InetAddress _addr;
};

#endif
